/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012, 2013
===========================================================================
                        Common Information
"TornadoEngine" GPL Source Code

This file is part of the "TornadoEngine" GPL Source Code.

"TornadoEngine" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"TornadoEngine" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "TornadoEngine" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "TornadoEngine" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "TornadoEngine" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 

#include "ServerGame.h"

#include <string>

#include "IPhysicEngine.h"
#include "IManagerTime.h"
#include "IManagerObjectCommon.h"
#include "IManagerScene.h"
#include "IXML.h"

#include "MakerPhysicEngine.h"
#include "MakerManagerObjectCommon.h"
#include "MakerManagerTime.h"
#include "MakerManagerStateMachine.h"
#include "MakerManagerScene.h"
#include "MakerManagerConnectClient.h"

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
  MakeVectorModule();
}
//------------------------------------------------------------------------
TServerGame::~TServerGame()
{

}
//------------------------------------------------------------------------
void TServerGame::Work(int variant_use, const char* sNameDLL, const char* arg)// начало работы
{
  if(Init(variant_use,sNameDLL,arg)==false)
    return;

  flgNeedStop = false;
  flgActive   = true;
  //------------------------------------------------------
  while(flgNeedStop==false)
  {
		mStartTime = ht_GetMSCount();// запомнить время старта
		// опросить модули движка для генерации событий
    if(MakeEventFromModule()==false)
      break;
    // обработать события
    HandleEventByDeveloper();
		mServerDeveloperTool->Refresh();
    
    if(mServerDeveloperTool->NeedExit())
      break;
		// расчет нагрузки
		CalcAndWaitRestTime();
  }
  //------------------------------------------------------
  flgActive = false;

  Done();
}
//------------------------------------------------------------------------
bool TServerGame::Init(int variant_use, const char* sNameDLL, const char* arg)
{
  // загрузка DLL
  CHECK_RET(LoadDLL(variant_use,sNameDLL))
  if(mGetServerDeveloperTool==NULL)// политика: нет DLL - нет движка.
    return false;
  
  // подготовить пути для ресурсов
  string sRelPathXML = mClientDeveloperTool->GetPathXMLFile();
  char sAbsPath[300];
  FindAbsPath((char*)sRelPathXML.data(),sAbsPath,sizeof(sAbsPath));
  if(GetStorePathResources()->Load(sAbsPath)==false)
    return false;
  //------------------------------------------
  TMakerManagerScene makerManagerScene;
  mCServer.mManagerScene = makerManagerScene.New();
  switch(mType)
  {
    case eSlave:
      mMainThreadVecModule.push_back(&TServerGame::HandleSceneEvent);
      break;
    case eMaster:
    case eSuperServer:
      break;
  }	
  //------------------------------------------
  TMakerManagerConnectClient makerManagerConnectClient;
  mCServer.mManagerCClient = makerManagerConnectClient.New();
  //------------------------------------------
	switch(mType)
	{
		case eSlave:
			mCServer.mNet.Base = new nsMelissa::TSlave;
			break;
		case eMaster:
			mCServer.mNet.Base = new nsMelissa::TMaster;
			break;
		case eSuperServer:
			mCServer.mNet.Base = new nsMelissa::TSuperServer;
			break;
	}	
	SetupNetComponent(mCServer.mNet.Base);
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
  delete mCServer.mNet.Base;
  mCServer.mNet.Base = NULL;

  TMakerManagerScene makerManagerScene;
  makerManagerScene.Delete(mCServer.mManagerScene);
  mCServer.mManagerScene = NULL;
  //------------------------------------------
  TMakerManagerConnectClient makerManagerConnectClient;
  makerManagerConnectClient.Delete(mCServer.mManagerCClient);
  mCServer.mManagerCClient = NULL;
}
//------------------------------------------------------------------------
void TServerGame::HandleEventByDeveloper()
{
  TEvent* pEvent = GetEvent();
  while(pEvent)
  {
    // обработка события
    HandleEvent(pEvent);
    delete pEvent;
    pEvent = GetEvent();
  }
}
//------------------------------------------------------------------------
bool TServerGame::HandleNetEngineEvent()
{
  mCServer.mNet.Base->Work();
  return true;
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
  // опросить интерфейсы, которые не наследуются от TSrcEvent
}
//------------------------------------------------------------------------
void TServerGame::HandleEvent(TEvent* pEvent)
{
  mServerDeveloperTool->Event(pEvent);
}
//------------------------------------------------------------------------
bool TServerGame::MakeEventFromModule()
{
  int cnt = mMainThreadVecModule.size();
  for( int i = 0 ; i < cnt ; i++ )
  {
    if(mMainThreadVecModule[i])
      CHECK_RET((this->*mMainThreadVecModule[i])())
  }
  // опросить объекты на наличие событий
  CollectEvent();
  return true;
}
//------------------------------------------------------------------------
void TServerGame::MakeVectorModule()
{
  // сетевой движок
  mMainThreadVecModule.push_back(&TServerGame::HandleNetEngineEvent);
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
