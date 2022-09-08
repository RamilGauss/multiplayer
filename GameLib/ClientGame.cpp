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
#include <glib/gthread.h>

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

#include "common_defs.h"
#include "BL_Debug.h"
#include "HiTimer.h"
#include "NetSystem.h"
#include "ErrorReg.h"

#include "Logger.h"
#include "NameSrcEventID.h"

#include "IControlCamera.h"
#include "file_operation.h"
#include "StorePathResources.h"
#include "MapXML_Field.h"
#include "MakerManagerStateMachine.h"

using namespace std;
using namespace nsEvent;

TClientGame::TClientGame()
{
  g_thread_init( NULL );
  err_Init();
  errSTR_Init();
  errSTD_Init();
  errSDK_Init();
  ht_Init();
  ns_Init();
}
//------------------------------------------------------------------------
TClientGame::~TClientGame()
{

}
//------------------------------------------------------------------------
void TClientGame::Work(const char* sNameDLL, const char* arg)// начало работы
{
  if(Init(sNameDLL,arg)==false)
    return;

  flgNeedStop = false;
  flgActive   = true;
  //------------------------------------------------------
  while(flgNeedStop==false)
  {
    // обработать события графического ядра - Key+Mouse, GUI, внутренние события GE.
    if(HandleGraphicEngineEvent()==false)
      break;
    // опросить объекты на наличие событий
    CollectEvent();

    // обработать события
    if(HandleExternalEvent()==false)
      break;
    // расчеты, необходимые для рендера, в зависимости от времени предыдущего расчета
    PrepareForRender();
    Render();
  }
  //------------------------------------------------------
  flgActive = false;

  Done();
}
//------------------------------------------------------------------------
bool TClientGame::Init(const char* sNameDLL, const char* arg)
{
  InitLog();
  // загрузка DLL
  CHECK_RET(LoadDLL(sNameDLL))
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
  mCClient.mGraphicEngine->SetSelfName(ID_SRC_EVENT_GRAPHIC_ENGINE);
  mCClient.mGraphicEngine->SetDstObject(this);
  mCClient.mGraphicEngine->SetTitleWindow(mClientDeveloperTool->GetTitleWindow().data());
  //------------------------------------------
  TMakerPhysicEngine makerPhysicEngine;
  mCClient.mPhysicEngine = makerPhysicEngine.New();
  mCClient.mPhysicEngine->SetSelfName(ID_SRC_EVENT_PHYSIC_ENGINE);
  mCClient.mPhysicEngine->SetDstObject(this);
  //------------------------------------------
  TMakerManagerStateMachine makerMStateManager;
  mCClient.mMStateMachine = makerMStateManager.New();
  //------------------------------------------
  //TMakerNET_Engine makerNET;
  //mNET = makerNET.New();
  //mNET->Init();// создали окно
  //mNET->SetSelfName(STR_SRC_EVENT_GRAPHIC_ENGINE);
  //mNET->SetDstObject(this);
  //------------------------------------------
  TMakerManagerObjectCommon makerMOC;
  mCClient.mMOC = makerMOC.New();
  mCClient.mMOC->Init(pMakerObjectCommon);
  mCClient.mMOC->SetSelfName(ID_SRC_EVENT_MANAGER_OBJECT_COMMON);
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
  mClientDeveloperTool->SetInitLogFunc(::GetLogger);
  mClientDeveloperTool->Init(&mCClient,arg);
  return true;
}
//------------------------------------------------------------------------
void TClientGame::Done()
{
  mClientDeveloperTool->Done();// освободить ресурсы DevTool

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

  TMakerManagerObjectCommon makerMOC;
  makerMOC.Delete(mCClient.mMOC);
  mCClient.mMOC = NULL;

  TMakerManagerTime makerMTimer;
  makerMTimer.Delete(mCClient.mMTime);
  mCClient.mMTime = NULL;
}
//------------------------------------------------------------------------
bool TClientGame::HandleExternalEvent()
{
  TEvent* pEvent = GetEvent();
  while(pEvent)
  {
    // обработка события
    if(HandleEvent(pEvent)==false)
    {
      delete pEvent;
      return false;
    }
    delete pEvent;
    pEvent = GetEvent();
  }
  return true;
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
void TClientGame::CollectEvent()
{
  // опросить интерфейсы, у которых нет своего потока
}
//------------------------------------------------------------------------
bool TClientGame::HandleEvent(TEvent* pEvent)
{
  return mClientDeveloperTool->HandleEvent(pEvent);
}
//------------------------------------------------------------------------
void TClientGame::InitLog()
{
  GetLogger()->Done();
  GetLogger()->Register("GE");
  GetLogger()->Register("PE");
  GetLogger()->Register("MOC");
  GetLogger()->Register("Form");
}
//------------------------------------------------------------------------
