/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ServerDeveloperTool_MasterTank.h"

#include <boost/asio/ip/impl/address_v4.ipp>

#include "MakerObjectCommon.h"
#include "../GameLib/NameSrcEventID.h"
#include "Master.h"
#include "NetSystem.h"
#include "GlobalParam.h"
#include "../GUI/IGUI.h"

using namespace std;
using namespace nsMelissa;

TServerDeveloperTool_MasterTank::TServerDeveloperTool_MasterTank()
{
  mCounterClient = 0;

  mMakerObjectCommon = new TMakerObjectCommon;
}
//---------------------------------------------------------------------------------
TServerDeveloperTool_MasterTank::~TServerDeveloperTool_MasterTank()
{
  delete mMakerObjectCommon;
}
//---------------------------------------------------------------------------------
void TServerDeveloperTool_MasterTank::Init(TComponentServer* pComponent, const char* arg)
{
  InitLog();
  mComponent = *pComponent;

  unsigned int ip = boost::asio::ip::address_v4::from_string(ns_getSelfIP(0)).to_ulong();

  bool resOpen = mComponent.mNet.Master->Open(MASTER_PORT);
  BL_ASSERT(resOpen);

  // подстроиться
  mComponent.mGUI->Resize();
}
//---------------------------------------------------------------------------------
int TServerDeveloperTool_MasterTank::GetTimeRefreshMS()// как часто происходит вызов Refresh()
{
	return 100;
}
//---------------------------------------------------------------------------------
void TServerDeveloperTool_MasterTank::Refresh()
{

}
//---------------------------------------------------------------------------------
IMakerObjectCommon* TServerDeveloperTool_MasterTank::GetMakerObjectCommon()
{
	return mMakerObjectCommon;
}
//------------------------------------------------------------------------------------
string TServerDeveloperTool_MasterTank::GetPathXMLFile()
{
	return "resources.xml";
}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_MasterTank::Done()
{

}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_MasterTank::Event(nsEvent::TEvent* pEvent)
{
  switch(pEvent->from)
  {
    case ID_SRC_EVENT_NETWORK_ENGINE:
      HandleFromMelissa((TBaseEvent*)pEvent->container.GetPtr());
      break;
    case ID_SRC_EVENT_PHYSIC_ENGINE:
      break;
    case ID_SRC_EVENT_MANAGER_OBJECT_COMMON:
      break;
  }
}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_MasterTank::HandleFromMelissa(TBaseEvent* pBE)
{
  string sEvent;  
  switch(pBE->mType)
  {
    case TBase::eConnectDown:
      sEvent = "ConnectDown";
      TryLogin((TEventTryLogin*)pBE);
      break;
    case TBase::eDisconnectDown:
      sEvent = "DisconnectDown";
      break;
    case TBase::eConnectUp:
      sEvent = "ConnectUp";
      break;
    case TBase::eDisconnectUp:
      sEvent = "DisconnectUp";
      break;
    case TBase::eError:
      sEvent = "Error";
      break;
    case TBase::eRecvFromDown:
      sEvent = "RecvFromDown";
      break;
    case TBase::eRecvFromUp:
      sEvent = "RecvFromUp";
      break;
    case TBase::eSaveContext:
      sEvent = "SaveContext";
      break;
    case TBase::eRestoreContext:
      sEvent = "RestoreContext";
      break;
    case TBase::eTryLogin:
      sEvent = "TryLogin";
      break;
    case TBase::eResultLogin:
      sEvent = "ResultLogin";
      break;
    case TBase::eCreateGroup:
      sEvent = "CreateGroup";
      break;
    case TBase::eLeaveGroup:
      sEvent = "LeaveGroup";
      break;
    case TBase::eDestroyGroup:
      sEvent = "DestroyGroup";
      break;
  }
  GetLogger()->Get("Inner")->WriteF_time("Melissa: %s.\n",sEvent.data());
}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_MasterTank::InitLog()
{
  if(mFuncGetLogger)
  {
    mFuncGetLogger()->Register("Inner");// для логирования внутренних событий
    mFuncGetLogger()->Init("MasterTank");
  }
}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_MasterTank::TryLogin(TEventTryLogin* pEvent)
{
  mCounterClient++;
  char s[100];
  sprintf(s,"hello, Client %u",mCounterClient);
  string result = s;
  mComponent.mNet.Master->SetResultLogin(true, 
                                         pEvent->id_session, 
                                         mCounterClient,
                                         (void*)result.data(),result.length());
}
//---------------------------------------------------------------------------------------------
