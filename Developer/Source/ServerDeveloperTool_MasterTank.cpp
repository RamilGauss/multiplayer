/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
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
#include "MasterForm.h"
#include "../QtLib/IQtLib.h"

using namespace std;
using namespace nsMMOEngine;

TServerDeveloperTool_MasterTank::TServerDeveloperTool_MasterTank()
{
  mCounterClient = 0;

  mMakerObjectCommon = new TMakerObjectCommon;
  
  mMasterForm = NULL;
}
//---------------------------------------------------------------------------------
TServerDeveloperTool_MasterTank::~TServerDeveloperTool_MasterTank()
{
  delete mMakerObjectCommon;
}
//---------------------------------------------------------------------------------
void TServerDeveloperTool_MasterTank::Init(TComponentServer* pComponent, vector<string>& arg)
{
  InitLog();
  mComponent = *pComponent;

  ParseCmd(arg);

  TInputCmdDevTool::TInput input;
  mInputCmd.Get(input);

  bool resOpen = mComponent.mNet.Master->Open(&TDescOpen(input.port));
  BL_ASSERT(resOpen);

  mComponent.mNet.Master->ConnectUp(input.ip, SUPERSERVER_PORT);

  mComponent.mQtGUI->CallFromQtThreadByFunc(&TServerDeveloperTool_MasterTank::InitQtForm,this);
}
//---------------------------------------------------------------------------------
int TServerDeveloperTool_MasterTank::GetTimeRefreshMS()
{
	return 100;
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
      HandleFromMMOEngine((TBaseEvent*)pEvent->container.GetPtr());
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
void TServerDeveloperTool_MasterTank::HandleFromMMOEngine(TBaseEvent* pBE)
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
      TryLogin((TEventTryLogin*)pBE);
      break;
    case TBase::eResultLogin:
      sEvent = "ResultLogin";
      break;
    //case TBase::eCreateGroup:
    //  sEvent = "CreateGroup";
    //  break;
    //case TBase::eLeaveGroup:
    //  sEvent = "LeaveGroup";
    //  break;
    case TBase::eDestroyGroup:
      sEvent = "DestroyGroup";
      break;
  }
  GetLogger("Inner")->WriteF_time("MMOEngine: %s.\n",sEvent.data());
}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_MasterTank::InitLog()
{
  GetLogger()->Register("Inner");// ��� ����������� ���������� �������
  GetLogger()->Init("MasterTank");
}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_MasterTank::ConnectDown(TEventConnectDown* pEvent)
{
  unsigned int* pID = new unsigned int(pEvent->id_session);
  mListID_SessionAdd.Add(pID);

  mComponent.mQtGUI->CallFromQtThreadByFunc(&TServerDeveloperTool_MasterTank::AddSlaveQt,this);
}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_MasterTank::DisconnectDown(TEventDisconnectDown* pEvent)
{
  unsigned int* pID = new unsigned int(pEvent->id_session);
  mListID_SessionDelete.Add(pID);

  mComponent.mQtGUI->CallFromQtThreadByFunc(&TServerDeveloperTool_MasterTank::DeleteSlaveQt,this);
}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_MasterTank::TryLogin(TEventTryLogin* pEvent)
{
  bool resAccept = true;
  mCounterClient++;
  char result[100];
  sprintf(result,"hello, Client %u",mCounterClient);
  mComponent.mNet.Master->SetResultLogin(resAccept, 
    pEvent->id_session, mCounterClient,
    (void*)&result[0], strlen(result));
}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_MasterTank::InitQtForm()
{
  mMasterForm = new MasterForm;
  mMasterForm->show();
}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_MasterTank::HandleFromQt(nsEvent::TEvent* pEvent)
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
void TServerDeveloperTool_MasterTank::ConnectUp(TEventConnectUp* pBE)
{
  mComponent.mQtGUI->CallFromQtThreadByFunc(&TServerDeveloperTool_MasterTank::ConnectUpQt,this);
  // ###
  //char s = 'm';
  //TBreakPacket bp;
  //bp.PushBack(&s, sizeof(s));
  //mComponent.mNet.Master->SendUp(bp);
}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_MasterTank::DisconnectUp(TEventDisconnectUp* pBE)
{
  mComponent.mQtGUI->CallFromQtThreadByFunc(&TServerDeveloperTool_MasterTank::DisconnectUpQt,this);
}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_MasterTank::ConnectUpQt()
{
  mMasterForm->SetConnect(true);
}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_MasterTank::DisconnectUpQt()
{
  mMasterForm->SetConnect(false);
}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_MasterTank::ParseCmd(std::vector<std::string>& arg)
{
  TInputCmdDevTool::TInput input;
  input.ip = boost::asio::ip::address_v4::from_string(ns_getSelfIP(0)).to_ulong();
  input.port = MASTER_PORT;
 
  mInputCmd.SetDefParam(input);
  mInputCmd.SetArg(arg);
}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_MasterTank::AddSlaveQt()
{
  unsigned int** ppFirst = mListID_SessionAdd.GetFirst();
  while(ppFirst)
  {
    unsigned int ID = *(*ppFirst);

    MasterForm::TDesc desc;
    desc.id_session = ID;
    bool resInfoSession = mComponent.mNet.Master->GetInfoSession(ID, desc.ip_port);
    BL_ASSERT(resInfoSession);
    mMasterForm->Add(desc);
    // ��������� ID
    mListID_SessionAdd.Remove(ppFirst);
    ppFirst = mListID_SessionAdd.GetFirst();
  }
}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_MasterTank::DeleteSlaveQt()
{
  unsigned int** ppFirst = mListID_SessionDelete.GetFirst();
  while(ppFirst)
  {
    unsigned int ID = *(*ppFirst);

    mMasterForm->Delete(ID);
    // ��������� ID
    mListID_SessionDelete.Remove(ppFirst);
    ppFirst = mListID_SessionDelete.GetFirst();
  }
}
//---------------------------------------------------------------------------------------------