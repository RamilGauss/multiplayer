/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include <stddef.h>

#include "IGame.h"

#include "IPhysicEngine.h"
#include "IManagerObjectCommon.h"
#include "IGraphicEngine.h"
#include "INetTransport.h"

#include "MakerLoaderDLL.h"
#include "BL_Debug.h"
#include "ShareMisc.h"
#include "HiTimer.h"
#include "NetSystem.h"
#include "Logger.h"
#include "Base.h"
#include "NameSrcEventID.h"
#include "IQtLib.h"

using namespace std;
using namespace nsEvent;

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
    mFreeDeveloperTool(mClientDeveloperTool);
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
    GetLogger(STR_GAME)->WriteF_time("LoadDLL() warning, object was loaded.\n");
    BL_FIX_BUG();
    return true;
  }
  if(mLoaderDLL->Init(sNameDLL)==false)
  {
    GetLogger(STR_GAME)->WriteF_time("LoadDLL() FAIL init.\n");
    BL_FIX_BUG();
    return false;
  }
  mFreeDeveloperTool = (FuncFreeDeveloperTool)mLoaderDLL->Get(StrFreeDeveloperTool);
  if(mFreeDeveloperTool==NULL)
  {
    GetLogger(STR_GAME)->WriteF_time("LoadDLL() FAIL load FuncFree.\n");
    BL_FIX_BUG();
    return false;
  }
  SetupFuncAndDevToolByType(variant_use);
  // � ������������ �������� ������, ���� ������� �����
  MakeVectorModule();
  return true;
}
//----------------------------------------------------------------------
void IGame::Init()
{
  if(ns_Init()==false)
  {
    GetLogger(STR_GAME)->WriteF_time("Error ns_Init().\n");
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
	GetLogger()->Register(STR_NAME_MMO_ENGINE);
	GetLogger()->Register(STR_NAME_PHYSIC_ENGINE);
	GetLogger()->Register(STR_NAME_MOC);
  GetLogger()->Register(STR_NAME_QT_LIB);
}
//------------------------------------------------------------------------
void IGame::Work(int variant_use, const char* sNameDLL, vector<string>& arg)// ������ ������
{
  if(Init(variant_use,sNameDLL,arg)==false)
    return;

  flgNeedStop = false;
  flgActive   = true;
  //------------------------------------------------------
  while(flgNeedStop==false)
    if(Work()==false)
      break;
  //------------------------------------------------------
  flgActive = false;
  Done();
}
//------------------------------------------------------------------------
void IGame::SetupNetComponent(nsMMOEngine::TBase* pBase)
{
	pBase->SetSelfID(ID_SRC_EVENT_NETWORK_ENGINE);
	pBase->SetDstObject(this);
	// ������ ��������� ����������
	pBase->Init(&mWrapperMakerTransport);
	// ����� ����������� ������ ��������� ������� ��������� (��������� ����� � ����������, ��������������)
}
//------------------------------------------------------------------------
void IGame::Stop()
{
  flgNeedStop = true;
  while(flgActive)
    ht_msleep(eWaitFeedBack);
}
//------------------------------------------------------------------------
bool IGame::MakeEventFromModule()
{
  int cnt = mMainThreadVecModule.size();
  for( int i = 0 ; i < cnt ; i++ )
  {
    // ��������� ������� �������
    // ��������� ������ ����� ������� false
    RET_FALSE(mMainThreadVecModule[i]())
  }
  // �������� ������� �� ������� �������
  CollectEvent();
  return true;
}
//------------------------------------------------------------------------
void IGame::HandleEventByDeveloper()
{
  TEvent* pEvent = GetEvent();
  while(pEvent)
  {
    // ��������� �������
    HandleEvent(pEvent);
    delete pEvent;
    pEvent = GetEvent();
  }
}
//------------------------------------------------------------------------
void IGame::SetupFuncAndDevToolByType(int variant_use)
{
  // ���������� ����� �������
  switch(mType)
  {
    case eClient:
      mGetClientDeveloperTool = 
        (FuncGetClientDeveloperTool)mLoaderDLL->Get(StrGetClientDeveloperTool);
      break;
    case eSlave:
      mGetServerDeveloperTool = 
        (FuncGetServerDeveloperTool)mLoaderDLL->Get(StrGetSlaveDeveloperTool);
      break;
    case eMaster:
      mGetServerDeveloperTool = 
        (FuncGetServerDeveloperTool)mLoaderDLL->Get(StrGetMasterDeveloperTool);
      break;
    case eSuperServer:
      mGetServerDeveloperTool = 
        (FuncGetServerDeveloperTool)mLoaderDLL->Get(StrGetSuperServerDeveloperTool);
      break;
  }
  // ���� ����� ����������, �� ������� ������
  if(mGetClientDeveloperTool)
    mClientDeveloperTool = mGetClientDeveloperTool(variant_use);
  if(mGetServerDeveloperTool)
    mServerDeveloperTool = mGetServerDeveloperTool(variant_use);
}
//------------------------------------------------------------------------
void TDescThread::Work()
{
  flgNeedStop = false;
  flgActive   = true;
  while(flgNeedStop==false)
  {
    if(pFunc()==false)
      break;
    ht_msleep(sleep_ms);
  }
  flgActive = false;
}
//------------------------------------------------------------------------
