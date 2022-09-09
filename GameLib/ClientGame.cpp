/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012
===========================================================================
                        Common Information
"Tanks" GPL Source Code

This file is part of the "Tanks" GPL Source Code.

"Tanks" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"Tanks" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "Tanks" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "Tanks" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "Tanks" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 

#include <stddef.h>
#include <string>

#include "BL_Debug.h"
#include "ClientGame.h"
#include "MakerPhysicEngine.h"
#include "IGraphicEngine.h"
#include "IManagerTime.h"
#include "IManagerObjectCommon.h"
#include "MakerGraphicEngine.h"
#include "StrNameSrcEvent.h"
#include "MakerManagerObjectCommon.h"

#include "HiTimer.h"
#include "NetSystem.h"
#include "ErrorReg.h"
#include <glib/gthread.h>
#include "IXML.h"
#include "common_defs.h"
#include "MakerManagerTime.h"
#include "..\GUI\MakerGUI.h"
#include "IGUI.h"
#include "Logger.h"


using namespace std;

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
  // загрузка DLL
  CHECK_RET(LoadDLL(sNameDLL))
  if(mGetClientDeveloperTool==NULL)
    return false;
  // политика: нет DLL - нет движка.

  // создатель объектов
  IMakerObjectCommon* pMakerObjectCommon = mClientDeveloperTool->GetMakerObjectCommon();

  // создать двигатели и проинициализировать менеджеры
  TMakerGraphicEngine makerGraphicEngine;
  mGraphicEngine = makerGraphicEngine.New();
  mGraphicEngine->Init();// создали окно
  mGraphicEngine->SetSelfName(STR_SRC_EVENT_GRAPHIC_ENGINE);
  mGraphicEngine->SetDstObject(this);
  mGraphicEngine->SetTitleWindow(mClientDeveloperTool->GetTitleWindow().data());
  //------------------------------------------
  TMakerPhysicEngine makerPhysicEngine;
  mPhysicEngine = makerPhysicEngine.New();
  mPhysicEngine->SetSelfName(STR_SRC_EVENT_PHYSIC_ENGINE);
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
  mMOC->SetSelfName(STR_SRC_EVENT_MANAGER_OBJECT_COMMON);
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
  mClientDeveloperTool->SetInitLogFunc(::InitLogger);
  IClientDeveloperTool::TComponentClient components;
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
  delete mGUI;
  mGUI = NULL;
  mGraphicEngine->ZeroGUI();
  delete mGraphicEngine;
  mGraphicEngine = NULL;
  delete mPhysicEngine;
  mPhysicEngine = NULL;
  delete mMOC;
  mMOC = NULL;
  delete mMTime;
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
