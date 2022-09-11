/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ServerDeveloperTool_SlaveTank.h"

#include <boost/asio/ip/impl/address_v4.ipp>

#include "MakerObjectCommon.h"
#include "Slave.h"
#include "NetSystem.h"
#include "GlobalParam.h"
#include "../GameLib/NameSrcEventID.h"

using namespace std;
using namespace nsMelissa;

TServerDeveloperTool_SlaveTank::TServerDeveloperTool_SlaveTank()
{
	mMakerObjectCommon = new TMakerObjectCommon;
}
//---------------------------------------------------------------------------------
TServerDeveloperTool_SlaveTank::~TServerDeveloperTool_SlaveTank()
{
  delete mMakerObjectCommon;
}
//---------------------------------------------------------------------------------
void TServerDeveloperTool_SlaveTank::Init(TComponentServer* pComponent, const char* arg)
{
  InitLog();
  mComponent = *pComponent;

  unsigned int ip = boost::asio::ip::address_v4::from_string(ns_getSelfIP(0)).to_ulong();

  bool resOpen = mComponent.mNet.Slave->Open(SLAVE_PORT);
  BL_ASSERT(resOpen);
  mComponent.mNet.Slave->ConnectUp( ip, MASTER_PORT);
}
//---------------------------------------------------------------------------------
int TServerDeveloperTool_SlaveTank::GetTimeRefreshMS()// как часто происходит вызов Refresh()
{
	return 100;
}
//---------------------------------------------------------------------------------
void TServerDeveloperTool_SlaveTank::Refresh()
{

}
//---------------------------------------------------------------------------------
IMakerObjectCommon* TServerDeveloperTool_SlaveTank::GetMakerObjectCommon()
{
	return mMakerObjectCommon;
}
//------------------------------------------------------------------------------------
string TServerDeveloperTool_SlaveTank::GetPathXMLFile()
{
	return "resources.xml";
}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_SlaveTank::Done()
{

}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_SlaveTank::Event(nsEvent::TEvent* pEvent)
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
void TServerDeveloperTool_SlaveTank::HandleFromMelissa(TBaseEvent* pBE)
{
  string sEvent;  
  switch(pBE->mType)
  {
    case TBase::eConnectDown:
      sEvent = "ConnectDown";
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
void TServerDeveloperTool_SlaveTank::InitLog()
{
  if(mFuncGetLogger)
  {
    mFuncGetLogger()->Register("Inner");// для логирования внутренних событий
    mFuncGetLogger()->Init("SlaveTank");
  }
}
//---------------------------------------------------------------------------------------------
