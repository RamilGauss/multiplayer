/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012
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
you may contact in writing [ramil2085@gmail.com].
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
void TClientGame::Work(const char* sNameDLL)// начало работы
{
  if(Init(sNameDLL)==false)
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
    Calc();
    Render();
  }
  //------------------------------------------------------
  flgActive = false;

  Done();
}
//------------------------------------------------------------------------
bool TClientGame::Init(const char* sNameDLL)
{
  InitLog();
  // загрузка DLL
  CHECK_RET(LoadDLL(sNameDLL))
  if(mGetClientDeveloperTool==NULL)
    return false;
  // политика: нет DLL - нет движка.

  // камера
  TMakerControlCamera makerControlCamera;
  mControlCamera = makerControlCamera.New();

  // создатель объектов
  IMakerObjectCommon* pMakerObjectCommon = mClientDeveloperTool->GetMakerObjectCommon();

  // создать двигатели и проинициализировать менеджеры
  TMakerGraphicEngine makerGraphicEngine;
  mGraphicEngine = makerGraphicEngine.New();
  mGraphicEngine->Init();// создали окно
  mGraphicEngine->SetSelfName(ID_SRC_EVENT_GRAPHIC_ENGINE);
  mGraphicEngine->SetDstObject(this);
  mGraphicEngine->SetTitleWindow(mClientDeveloperTool->GetTitleWindow().data());
  //------------------------------------------
  TMakerPhysicEngine makerPhysicEngine;
  mPhysicEngine = makerPhysicEngine.New();
  mPhysicEngine->SetSelfName(ID_SRC_EVENT_PHYSIC_ENGINE);
  mPhysicEngine->SetDstObject(this);
  //------------------------------------------
  //TMakerNET_Engine makerNET;
  //mNET = makerNET.New();
  //mNET->Init();// создали окно
  //mNET->SetSelfName(STR_SRC_EVENT_GRAPHIC_ENGINE);
  //mNET->SetDstObject(this);
  //------------------------------------------
  TMakerManagerObjectCommon makerMOC;
  mMOC = makerMOC.New();
  mMOC->Init(pMakerObjectCommon);
  mMOC->SetSelfName(ID_SRC_EVENT_MANAGER_OBJECT_COMMON);
  mMOC->SetDstObject(this);
  //------------------------------------------
  TMakerManagerTime makerMTime;
  mMTime = makerMTime.New();
  //------------------------------------------
  TMakerGUI makerGUI;
  mGUI = makerGUI.New();
  mGraphicEngine->SetGUI(mGUI);
  //------------------------------------------
  //------------------------------------------
  mClientDeveloperTool->SetInitLogFunc(::GetLogger);
  IClientDeveloperTool::TComponentClient components;
  components.mControlCamera = mControlCamera;
  components.mGUI           = mGUI;
  components.mGraphicEngine = mGraphicEngine;
  components.mMTime         = mMTime;
  components.mMOC           = mMOC;
  components.mPhysicEngine  = mPhysicEngine;
  mClientDeveloperTool->Init(&components);
  return true;
}
//------------------------------------------------------------------------
void TClientGame::Done()
{
  mClientDeveloperTool->Done();// освободить ресурсы DevTool

  // а теперь модули
  TMakerGUI makerGUI;
  makerGUI.Delete(mGUI);
  mGUI = NULL;
  mGraphicEngine->ZeroGUI();

  TMakerGraphicEngine makerGE;
  makerGE.Delete(mGraphicEngine);
  mGraphicEngine = NULL;

  TMakerPhysicEngine makerPE;
  makerPE.Delete(mPhysicEngine);
  mPhysicEngine = NULL;

  TMakerManagerObjectCommon makerMOC;
  makerMOC.Delete(mMOC);
  mMOC = NULL;

  TMakerManagerTime makerMTimer;
  makerMTimer.Delete(mMTime);
  mMTime = NULL;
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
void TClientGame::Calc()
{
  mClientDeveloperTool->Calc();
}
//------------------------------------------------------------------------
void TClientGame::Render()
{
  mGraphicEngine->Work(mMTime->GetTime());
}
//------------------------------------------------------------------------
bool TClientGame::HandleGraphicEngineEvent()
{
  return mGraphicEngine->HandleInternalEvent();
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
