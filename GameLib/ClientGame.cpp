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

#include "ClientGame.h"

#include <string>
#include <boost/thread/thread.hpp>

#include "IControlCamera.h"
#include "IGraphicEngine.h"
#include "IPhysicEngine.h"
#include "IManagerTime.h"
#include "IManagerObjectCommon.h"
#include "IGUI.h"
#include "IXML.h"

#include "MakerPhysicEngine.h"
#include "MakerGraphicEngine.h"
#include "MakerManagerObjectCommon.h"
#include "MakerManagerTime.h"
#include "MakerGUI.h"
#include "MakerControlCamera.h"
#include "MakerManagerStateMachine.h"

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

TClientGame::TClientGame()
{
	MakeVectorModule();
}
//------------------------------------------------------------------------
TClientGame::~TClientGame()
{

}
//------------------------------------------------------------------------
/*
  Для клиента кол-во потоков определяется кол-вом ядер CPU
*/
void TClientGame::Work(int variant_use, const char* sNameDLL, const char* arg)// начало работы
{
  if(Init(variant_use,sNameDLL,arg)==false)
    return;

  flgNeedStop = false;
  flgActive   = true;
  //------------------------------------------------------
  while(flgNeedStop==false)
  {
    // опросить модули движка для генерации событий
    if(MakeEventFromModule()==false)
      break;
    // обработать события
    HandleEventByDeveloper();
    // расчеты, необходимые для рендера, в зависимости от времени предыдущего расчета
    PrepareForRender();
    Render();
    if(mClientDeveloperTool->NeedExit())
      break;
  }
  //------------------------------------------------------
  flgActive = false;

  Done();
}
//------------------------------------------------------------------------
bool TClientGame::Init(int variant_use, const char* sNameDLL, const char* arg)
{
  // загрузка DLL
  CHECK_RET(LoadDLL(variant_use, sNameDLL))
  if(mGetClientDeveloperTool==NULL)// политика: нет DLL - нет движка.
    return false;
  
  // подготовить пути для ресурсов
  string sRelPathXML = mClientDeveloperTool->GetPathXMLFile();
  char sAbsPath[300];
  FindAbsPath((char*)sRelPathXML.data(),sAbsPath,sizeof(sAbsPath));
  if(GetStorePathResources()->Load(sAbsPath)==false)
    return false;

  // камера
  TMakerControlCamera makerControlCamera;
  mCClient.mControlCamera = makerControlCamera.New();

  // создатель объектов
  IMakerObjectCommon* pMakerObjectCommon = mClientDeveloperTool->GetMakerObjectCommon();

  // создать двигатели и проинициализировать менеджеры
  TMakerGraphicEngine makerGraphicEngine;
  mCClient.mGraphicEngine = makerGraphicEngine.New(mCClient.mControlCamera);
  mCClient.mGraphicEngine->Init();// создали окно
  mCClient.mGraphicEngine->SetSelfID(ID_SRC_EVENT_GRAPHIC_ENGINE);
  mCClient.mGraphicEngine->SetDstObject(this);
  mCClient.mGraphicEngine->SetTitleWindow(mClientDeveloperTool->GetTitleWindow().data());
  //------------------------------------------
  TMakerPhysicEngine makerPhysicEngine;
  mCClient.mPhysicEngine = makerPhysicEngine.New();
  mCClient.mPhysicEngine->SetSelfID(ID_SRC_EVENT_PHYSIC_ENGINE);
  mCClient.mPhysicEngine->SetDstObject(this);
  //------------------------------------------
  TMakerManagerStateMachine makerMStateManager;
  mCClient.mMStateMachine = makerMStateManager.New();
  //------------------------------------------
  mCClient.mNetClient = new nsMelissa::TClient;
	SetupNetComponent(mCClient.mNetClient);
  //------------------------------------------
  TMakerManagerObjectCommon makerMOC;
  mCClient.mMOC = makerMOC.New();
  mCClient.mMOC->Init(pMakerObjectCommon);
  mCClient.mMOC->SetSelfID(ID_SRC_EVENT_MANAGER_OBJECT_COMMON);
  mCClient.mMOC->SetDstObject(this);
  //------------------------------------------
  TMakerManagerTime makerMTime;
  mCClient.mMTime = makerMTime.New();
  //------------------------------------------
  TMakerGUI makerGUI;
  mCClient.mGUI = makerGUI.New();
  mCClient.mGraphicEngine->SetGUI(mCClient.mGUI);
  //------------------------------------------
  //------------------------------------------
  // запустить потоки, в которых будут работать модули
  StartThreadModule();
  //------------------------------------------
  mClientDeveloperTool->SetInitLogFunc(::GetLogger);
  mClientDeveloperTool->Init(&mCClient,arg);

  return true;
}
//------------------------------------------------------------------------
void TClientGame::Done()
{
  mClientDeveloperTool->Done();// освободить ресурсы DevTool
  StopThreadModule();
  // а теперь модули
  TMakerManagerStateMachine makerMStateManager;
  makerMStateManager.Delete(mCClient.mMStateMachine);
  mCClient.mMStateMachine = NULL;

  TMakerGUI makerGUI;
  makerGUI.Delete(mCClient.mGUI);
  mCClient.mGUI = NULL;
  mCClient.mGraphicEngine->ZeroGUI();

  TMakerGraphicEngine makerGE;
  makerGE.Delete(mCClient.mGraphicEngine);
  mCClient.mGraphicEngine = NULL;

  TMakerPhysicEngine makerPE;
  makerPE.Delete(mCClient.mPhysicEngine);
  mCClient.mPhysicEngine = NULL;

  delete mCClient.mNetClient;
  mCClient.mNetClient = NULL;

  TMakerManagerObjectCommon makerMOC;
  makerMOC.Delete(mCClient.mMOC);
  mCClient.mMOC = NULL;

  TMakerManagerTime makerMTimer;
  makerMTimer.Delete(mCClient.mMTime);
  mCClient.mMTime = NULL;
}
//------------------------------------------------------------------------
void TClientGame::HandleEventByDeveloper()
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
void TClientGame::PrepareForRender()
{
  mClientDeveloperTool->PrepareForRender();
}
//------------------------------------------------------------------------
void TClientGame::Render()
{
  mCClient.mGraphicEngine->Work(mCClient.mMTime->GetTime());
}
//------------------------------------------------------------------------
bool TClientGame::HandleGraphicEngineEvent()
{
  return mCClient.mGraphicEngine->HandleInternalEvent();
}
//------------------------------------------------------------------------
bool TClientGame::HandleNetEngineEvent()
{
  mCClient.mNetClient->Work();
  return true;
}
//------------------------------------------------------------------------
void TClientGame::CollectEvent()
{
  // опросить интерфейсы, которые не наследуются от TSrcEvent
}
//------------------------------------------------------------------------
void TClientGame::HandleEvent(TEvent* pEvent)
{
  mClientDeveloperTool->HandleEvent(pEvent);
}
//------------------------------------------------------------------------
bool TClientGame::MakeEventFromModule()
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
void TClientGame::MakeVectorModule()
{
  int countCore = GetCountCoreCPU();
  // обработать события графического ядра - Key+Mouse, GUI, внутренние события GE.
  mMainThreadVecModule.push_back(&TClientGame::HandleGraphicEngineEvent);
  // сетевой движок
  if(countCore==1)
    mMainThreadVecModule.push_back(&TClientGame::HandleNetEngineEvent);
  else
  {
    TDescThread dt;
    dt.pClientGame = this;
    dt.pFunc       = &TClientGame::HandleNetEngineEvent;
    dt.sleep_ms    = eSleepNE;
    mOtherThreadVecModule.push_back(dt);
  }
  // физика
  // звук
}
//------------------------------------------------------------------------
void* ThreadModule(void* p)
{
  TDescThread* pDesc = (TDescThread*)p;
  pDesc->Work();
  return NULL;
}
//------------------------------------------------------------------------
void TClientGame::StartThreadModule()
{
  int cnt = mOtherThreadVecModule.size();
  for( int i = 0 ; i < cnt ; i++ )
  {
    boost::thread thread_module(ThreadModule, &mOtherThreadVecModule[i]);

    while(mOtherThreadVecModule[i].flgActive==false)
      ht_msleep(eWaitFeedBack);
  }
}
//------------------------------------------------------------------------
void TClientGame::StopThreadModule()
{
  int cnt = mOtherThreadVecModule.size();
  for( int i = 0 ; i < cnt ; i++ )
  {
    mOtherThreadVecModule[i].flgNeedStop = true;
    while(mOtherThreadVecModule[i].flgActive==true)
      ht_msleep(eWaitFeedBack);
  }
}
//------------------------------------------------------------------------
void TDescThread::Work()
{
  flgNeedStop = false;
  flgActive   = true;
  while(flgNeedStop==false)
  {
    if((pClientGame->*pFunc)()==false)
      break;
    ht_msleep(sleep_ms);
  }
  flgActive = false;
}
//------------------------------------------------------------------------
