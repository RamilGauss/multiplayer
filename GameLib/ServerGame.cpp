/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ServerGame.h"

#include <string>
#include <functional>

#include "IPhysicEngine.h"
#include "IManagerTime.h"
#include "IManagerObjectCommon.h"
#include "IManagerScene.h"
#include "IXML.h"
#include "IControlCamera.h"
#include "IGraphicEngine.h"
#include "IGUI.h"

#include "MakerPhysicEngine.h"
#include "MakerManagerObjectCommon.h"
#include "MakerManagerTime.h"
#include "MakerManagerStateMachine.h"
#include "MakerManagerScene.h"
#include "MakerManagerConnectClient.h"
#include "MakerControlCamera.h"
#include "MakerGraphicEngine.h"
#include "MakerGUI.h"

#include "common_defs.h"
#include "BL_Debug.h"
#include "HiTimer.h"
#include "Logger.h"
#include "NameSrcEventID.h"
#include "file_operation.h"
#include "StorePathResources.h"
#include "MapXML_Field.h"
#include "ShareMisc.h"

using namespace std;
using namespace nsEvent;

TServerGame::TServerGame(eTypeRealize type):mStatLoad(30)
{
	mLogLoad.ReOpen(".\\serverLoad.xls");

	mType = type;
}
//------------------------------------------------------------------------
TServerGame::~TServerGame()
{

}
//------------------------------------------------------------------------
bool TServerGame::Work()
{
	mStartTime = ht_GetMSCount();// запомнить время старта
	// опросить модули движка для генерации событий
  RET_FALSE(MakeEventFromModule())
  // обработать события
  HandleEventByDeveloper();
	mServerDeveloperTool->Refresh();
  
  Render();
  if(mServerDeveloperTool->NeedExit())
    return false;
	// расчет нагрузки
	CalcAndWaitRestTime();
  return true;
}
//------------------------------------------------------------------------
bool TServerGame::Init(int variant_use, const char* sNameDLL, const char* arg)
{
  // загрузка DLL
  RET_FALSE(LoadDLL(variant_use,sNameDLL))
  if(mGetServerDeveloperTool==NULL)// политика: нет DLL - нет движка.
    return false;
  // подготовить пути для ресурсов
  string sRelPathXML = mServerDeveloperTool->GetPathXMLFile();
  char sAbsPath[300];
  FindAbsPath((char*)sRelPathXML.data(),sAbsPath,sizeof(sAbsPath));
  if(GetStorePathResources()->Load(sAbsPath)==false)
    return false;
  //------------------------------------------
  TMakerControlCamera makerControlCamera;
  mCServer.mControlCamera = makerControlCamera.New();
  //------------------------------------------
  TMakerGraphicEngine makerGraphicEngine;
  mCServer.mGraphicEngine = makerGraphicEngine.New(mCServer.mControlCamera);
  mCServer.mGraphicEngine->Init();// создали окно
  mCServer.mGraphicEngine->SetSelfID(ID_SRC_EVENT_GRAPHIC_ENGINE);
  mCServer.mGraphicEngine->SetDstObject(this);
  //------------------------------------------
  TMakerGUI makerGUI;
  mCServer.mGUI = makerGUI.New();
  mCServer.mGraphicEngine->SetGUI(mCServer.mGUI);
  //------------------------------------------
  TMakerManagerScene makerManagerScene;
  mCServer.mManagerScene = makerManagerScene.New();
  //------------------------------------------
  TMakerManagerConnectClient makerManagerConnectClient;
  mCServer.mManagerCClient = makerManagerConnectClient.New();
  //------------------------------------------
  string sTitle;
	switch(mType)
	{
		case eSlave:
			mCServer.mNet.Base = new nsMelissa::TSlave;
      sTitle = "Slave";
			break;
		case eMaster:
			mCServer.mNet.Base = new nsMelissa::TMaster;
      sTitle = "Master";
			break;
		case eSuperServer:
			mCServer.mNet.Base = new nsMelissa::TSuperServer;
      sTitle = "SuperServer";
			break;
	}	
	SetupNetComponent(mCServer.mNet.Base);
  mCServer.mGraphicEngine->SetTitleWindow(sTitle.data());
  //------------------------------------------
  mServerDeveloperTool->SetInitLogFunc(::GetLogger);
  mServerDeveloperTool->Init(&mCServer,arg);

  return true;
}
//------------------------------------------------------------------------
void TServerGame::Done()
{
  mServerDeveloperTool->Done();// освободить ресурсы DevTool
  // а теперь модули
  TMakerGUI makerGUI;
  makerGUI.Delete(mCServer.mGUI);
  mCServer.mGUI = NULL;
  mCServer.mGraphicEngine->ZeroGUI();

  TMakerGraphicEngine makerGE;
  makerGE.Delete(mCServer.mGraphicEngine);
  mCServer.mGraphicEngine = NULL;

  delete mCServer.mNet.Base;
  mCServer.mNet.Base = NULL;

  TMakerManagerScene makerManagerScene;
  makerManagerScene.Delete(mCServer.mManagerScene);
  mCServer.mManagerScene = NULL;
  //------------------------------------------
  TMakerManagerConnectClient makerManagerConnectClient;
  makerManagerConnectClient.Delete(mCServer.mManagerCClient);
  mCServer.mManagerCClient = NULL;

  // камера
  TMakerControlCamera makerControlCamera;
  makerControlCamera.Delete(mCServer.mControlCamera);
  mCServer.mControlCamera = NULL;
}
//------------------------------------------------------------------------
bool TServerGame::HandleNetEngineEvent()
{
  mCServer.mNet.Base->Work();
  return true;
}
//------------------------------------------------------------------------
bool TServerGame::HandleGraphicEngineEvent()
{
  return mCServer.mGraphicEngine->HandleInternalEvent();
}
//------------------------------------------------------------------------
bool TServerGame::HandleSceneEvent()
{
  mCServer.mManagerScene->Work();
  return true;
}
//------------------------------------------------------------------------
void TServerGame::CollectEvent()
{

}
//------------------------------------------------------------------------
void TServerGame::HandleEvent(TEvent* pEvent)
{
  mServerDeveloperTool->Event(pEvent);
}
//------------------------------------------------------------------------
void TServerGame::MakeVectorModule()
{
  // обработать события графического ядра - Key+Mouse, GUI, внутренние события GE.
  FuncHandleEvent fGraphicEngineEvent = boost::bind(&TServerGame::HandleGraphicEngineEvent, this);
  mMainThreadVecModule.push_back(fGraphicEngineEvent);
  // сетевой движок
  FuncHandleEvent fNetEngineEvent = boost::bind(&TServerGame::HandleNetEngineEvent, this);
  mMainThreadVecModule.push_back(fNetEngineEvent);
  // обработка событий сцены
  switch(mType)
  {
    case eSlave:
      {
        FuncHandleEvent fSceneEvent = boost::bind(&TServerGame::HandleSceneEvent, this);
        mMainThreadVecModule.push_back(fSceneEvent);
      }
      break;
    case eMaster:
    case eSuperServer:
      break;
  }	
}
//------------------------------------------------------------------------
void TServerGame::SetLoad()
{
	mCServer.mNet.Base->SetLoad(int(mStatLoad.aver()));
}
//------------------------------------------------------------------------
void TServerGame::CalcAndWaitRestTime()
{
	unsigned int now = ht_GetMSCount();
	unsigned int refresh_time = mServerDeveloperTool->GetTimeRefreshMS();// сколько можно потратить

	unsigned int work_time = now - mStartTime;// потрачено времени
	double loadProcent = (work_time*100.0)/refresh_time;// расчет нагрузки
	mStatLoad.add(loadProcent);// занести в статистику
	SetLoad();                 // задать в сетевой модуль

	mLogLoad.WriteF("%d\n",int(mStatLoad.aver()));
	// спать остаток времени
	if(now>refresh_time+mStartTime) return;
	unsigned int time_sleep = mStartTime + refresh_time - now;
	ht_msleep(time_sleep);
}
//------------------------------------------------------------------------
void TServerGame::Render()
{
  mCServer.mGraphicEngine->Work(0);
}
//------------------------------------------------------------------------
