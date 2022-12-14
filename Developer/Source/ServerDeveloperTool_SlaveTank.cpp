/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
??????? ?????? ????????? 
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
#include "../../GameLib/ITimer.h"

#include "DevProtocol.h"

using namespace std;
using namespace nsMMOEngine;

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

  bool resOpen = mComponent.mNet.Slave->Open(&TDescOpen(input.port));
  BL_ASSERT(resOpen);
  mComponent.mNet.Slave->ConnectUp( input.ip, MASTER_PORT);

  mComponent.mQtGUI->CallFromQtThreadByFunc(&TServerDeveloperTool_SlaveTank::InitQtForm,this);

  mComponent.mTimerFirstEvent->New(0);
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
      HandleFromMMOEngine((TBaseEvent*)pEvent->container.GetPtr());
      break;
    case ID_SRC_EVENT_PHYSIC_ENGINE:
      break;
    case ID_SRC_EVENT_MANAGER_OBJECT_COMMON:
      break;
    case ID_SRC_EVENT_QT_LIB:
      HandleFromQt(pEvent);
      break;
		case ID_SRC_EVENT_TIMER_FIRST_EVENT:
			mTestControlTank.Send();
			break;
  }
}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_SlaveTank::HandleFromMMOEngine(TBaseEvent* pBE)
{
  string sEvent;  
  switch(pBE->mType)
  {
    case TBase::eConnectDown:
      sEvent = "ConnectDown";
      ConnectDown((TEventConnectDown*)pBE);
      break;
    case TBase::eDisconnectDown:
      sEvent = "DisconnectDown";
      DisconnectDown((TEventDisconnectDown*)pBE);
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
    {
      char sError[300];
      TEventError* pEr = (TEventError*)pBE;
      sprintf(sError, "Error code=%u", pEr->code);
      sEvent = sError;
    }
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
    case TBase::eDestroyGroup:
      sEvent = "DestroyGroup";
      break;
  }
  GetLogger("Inner")->WriteF_time("MMOEngine: %s.\n",sEvent.data());
}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_SlaveTank::InitLog()
{
  GetLogger()->Register("Inner");// ??? ??????????? ?????????? ???????
  GetLogger()->Init("SlaveTank");
}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_SlaveTank::InitQtForm()
{
  mSlaveForm = new SlaveForm;
  mSlaveForm->show();

	mSlaveForm->SetControlTank(&mTestControlTank);
}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_SlaveTank::HandleFromQt(nsEvent::TEvent* pEvent)
{
  nsDevProtocol::TBase* pH = (nsDevProtocol::TBase*)pEvent->container.GetPtr();
  switch(pH->type)
  {
    case nsDevProtocol::Exit:
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
  //mComponent.mNet.Slave->SendUp(bp);
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
void TServerDeveloperTool_SlaveTank::ConnectDown(TEventConnectDown* pEvent)
{
  unsigned int* pID = new unsigned int(pEvent->id_session);
  mListID_SessionAdd.Add(pID);

  mComponent.mQtGUI->CallFromQtThreadByFunc(&TServerDeveloperTool_SlaveTank::AddClientQt,this);
}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_SlaveTank::DisconnectDown(TEventDisconnectDown* pEvent)
{
  unsigned int* pID = new unsigned int(pEvent->id_session);
  mListID_SessionDelete.Add(pID);

  mComponent.mQtGUI->CallFromQtThreadByFunc(&TServerDeveloperTool_SlaveTank::DeleteClientQt,this);
}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_SlaveTank::AddClientQt()
{
  unsigned int** ppFirst = mListID_SessionAdd.GetFirst();
  while(ppFirst)
  {
    unsigned int ID = *(*ppFirst);

    SlaveForm::TDesc desc;
    desc.id_session = ID;
    bool resInfoSession = mComponent.mNet.Slave->GetInfoSession(ID, desc.ip_port);
    BL_ASSERT(resInfoSession);
    mSlaveForm->Add(desc);
    // ????????? ID
    mListID_SessionAdd.Remove(ppFirst);
    ppFirst = mListID_SessionAdd.GetFirst();
  }
}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_SlaveTank::DeleteClientQt()
{
  unsigned int** ppFirst = mListID_SessionDelete.GetFirst();
  while(ppFirst)
  {
    unsigned int ID = *(*ppFirst);

    mSlaveForm->Delete(ID);
    // ????????? ID
    mListID_SessionDelete.Remove(ppFirst);
    ppFirst = mListID_SessionDelete.GetFirst();
  }
}
//---------------------------------------------------------------------------------------------