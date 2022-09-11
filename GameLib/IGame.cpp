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

#include <stddef.h>

#include "IGame.h"

#include "IPhysicEngine.h"
#include "IManagerObjectCommon.h"
#include "IGraphicEngine.h"
#include "INetTransport.h"

//#include "DeveloperTool_DLL.h"
#include "MakerLoaderDLL.h"
#include "BL_Debug.h"
#include "ShareMisc.h"
//#include "ErrorReg.h"
#include "HiTimer.h"
#include "NetSystem.h"
#include "Logger.h"
#include "Base.h"
#include "NameSrcEventID.h"


IGame::IGame()
{
  TMakerLoaderDLL maker;
  mLoaderDLL = maker.New();

  mGetClientDeveloperTool = NULL;
  mGetServerDeveloperTool = NULL;
  mFreeDeveloperTool      = NULL;

  mClientDeveloperTool = NULL;
  mServerDeveloperTool = NULL;

  Init();
  InitLog();
}
//----------------------------------------------------------------------
IGame::~IGame()
{
  if(mFreeDeveloperTool)
  {
    if(mFreeDeveloperTool) 
      mFreeDeveloperTool(mClientDeveloperTool);
    if(mFreeDeveloperTool) 
      mFreeDeveloperTool(mServerDeveloperTool);
  }

  TMakerLoaderDLL maker;
  maker.Delete(mLoaderDLL);
}
//----------------------------------------------------------------------
bool IGame::LoadDLL(int variant_use, const char* sNameDLL)
{
  if(mClientDeveloperTool!=NULL||
     mServerDeveloperTool!=NULL)
  {
    GetLogger()->Get(STR_GAME)->WriteF_time("LoadDLL() warning, object was loaded.\n");
    BL_FIX_BUG();
    return true;
  }
  if(mLoaderDLL->Init(sNameDLL)==false)
  {
    GetLogger()->Get(STR_GAME)->WriteF_time("LoadDLL() FAIL init.\n");
    BL_FIX_BUG();
    return false;
  }
  mFreeDeveloperTool = (FuncFreeDeveloperTool)mLoaderDLL->Get(StrFreeDeveloperTool);
  if(mFreeDeveloperTool==NULL)
  {
    GetLogger()->Get(STR_GAME)->WriteF_time("LoadDLL() FAIL load FuncFree.\n");
    BL_FIX_BUG();
    return false;
  }
  mGetClientDeveloperTool = (FuncGetClientDeveloperTool)mLoaderDLL->Get(StrGetClientDeveloperTool);
  mGetServerDeveloperTool = (FuncGetServerDeveloperTool)mLoaderDLL->Get(StrGetServerDeveloperTool);

  if(mGetClientDeveloperTool)
    mClientDeveloperTool = mGetClientDeveloperTool(variant_use);
  if(mGetServerDeveloperTool)
    mServerDeveloperTool = mGetServerDeveloperTool(variant_use);

  return true;
}
//----------------------------------------------------------------------
void IGame::Init()
{
  //g_thread_init( NULL );

  if(ns_Init()==false)
  {
    GetLogger()->Get(STR_GAME)->WriteF_time("Error ns_Init().\n");
    BL_FIX_BUG();
  }
}
//----------------------------------------------------------------------
void IGame::InitLog()
{
	GetLogger()->Done();
	GetLogger()->Register(STR_GAME);

	GetLogger()->Register(STR_NAME_GE);
	GetLogger()->Register(STR_NAME_NET_TRANSPORT);
	GetLogger()->Register(STR_NAME_MELISSA);
	GetLogger()->Register(STR_NAME_ROBERT);
	GetLogger()->Register(STR_NAME_MOC);
}
//------------------------------------------------------------------------
void IGame::SetupNetComponent(nsMelissa::TBase* pBase)
{
	pBase->SetSelfID(ID_SRC_EVENT_NETWORK_ENGINE);
	pBase->SetDstObject(this);
	// отдать создателя транспорта
	pBase->Init(&mWrapperMakerTransport);
	// далее разработчик обязан настроить сетевой интерфейс (назначить порты и подсистему, авторизоваться)
}
//------------------------------------------------------------------------
void IGame::Stop()
{
  flgNeedStop = true;
  while(flgActive)
    ht_msleep(eWaitFeedBack);
}
//------------------------------------------------------------------------
