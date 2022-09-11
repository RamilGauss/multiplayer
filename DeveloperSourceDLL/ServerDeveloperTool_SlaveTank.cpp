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
#include "../QtLib/IQtLib.h"
#include "SlaveForm.h"

using namespace std;
using namespace nsMelissa;

TServerDeveloperTool_SlaveTank::TServerDeveloperTool_SlaveTank()
{
	mMakerObjectCommon = new TMakerObjectCommon;
  mSlaveForm = NULL;
}
//---------------------------------------------------------------------------------
TServerDeveloperTool_SlaveTank::~TServerDeveloperTool_SlaveTank()
{
  delete mMakerObjectCommon;
}
//---------------------------------------------------------------------------------
void TServerDeveloperTool_SlaveTank::Init(TComponentServer* pComponent, vector<string>& arg)
{
  InitLog();
  mComponent = *pComponent;

  ParseCmd(arg);

  TInputCmdDevTool::TInput input;
  mInputCmd.Get(input);

  bool resOpen = mComponent.mNet.Slave->Open(input.port);
  BL_ASSERT(resOpen);
  mComponent.mNet.Slave->ConnectUp( input.ip, MASTER_PORT);

  mComponent.mQtGUI->CallFromQtThreadByFunc(&TServerDeveloperTool_SlaveTank::InitQtForm,this);
}
//---------------------------------------------------------------------------------
int TServerDeveloperTool_SlaveTank::GetTimeRefreshMS()
{
	return 100;
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
    case ID_SRC_EVENT_QT_LIB:
      HandleFromQt(pEvent);
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
      ConnectUp((TEventConnectUp*)pBE);
      break;
    case TBase::eDisconnectUp:
      sEvent = "DisconnectUp";
      DisconnectUp((TEventDisconnectUp*)pBE);
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
  GetLogger("Inner")->WriteF_time("Melissa: %s.\n",sEvent.data());
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
void TServerDeveloperTool_SlaveTank::InitQtForm()
{
  mSlaveForm = new SlaveForm;
  mSlaveForm->show();
}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_SlaveTank::HandleFromQt(nsEvent::TEvent* pEvent)
{
  int type = *((int*)pEvent->container.GetPtr());
  switch(type)
  {
    case 0:
      Exit();
      break;
    default:;
  }
}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_SlaveTank::ParseCmd(std::vector<std::string>& arg)
{
  TInputCmdDevTool::TInput input;
  input.ip = boost::asio::ip::address_v4::from_string(ns_getSelfIP(0)).to_ulong();
  input.port = SLAVE_PORT;

  mInputCmd.SetDefParam(input);
  mInputCmd.SetArg(arg);
}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_SlaveTank::ConnectUp(TEventConnectUp* pBE)
{
  mComponent.mQtGUI->CallFromQtThreadByFunc(&TServerDeveloperTool_SlaveTank::ConnectUpQt,this);
  // ###
  //char s = 'm';
  //TBreakPacket bp;
  //bp.PushBack(&s, sizeof(s));
  //mComponent.mNet.Master->SendUp(bp);
}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_SlaveTank::DisconnectUp(TEventDisconnectUp* pBE)
{
  mComponent.mQtGUI->CallFromQtThreadByFunc(&TServerDeveloperTool_SlaveTank::DisconnectUpQt,this);
}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_SlaveTank::ConnectUpQt()
{
  mSlaveForm->SetConnect(true);
}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_SlaveTank::DisconnectUpQt()
{
  mSlaveForm->SetConnect(false);
}
//---------------------------------------------------------------------------------------------
