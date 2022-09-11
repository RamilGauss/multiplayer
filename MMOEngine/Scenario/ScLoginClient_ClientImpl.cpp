/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ScLoginClient_ClientImpl.h"
#include "ContextScLoginClient.h"
#include "ManagerSession.h"
#include "Base.h"
#include "Logger.h"
#include "Events.h"
#include "ErrorCode.h"

using namespace nsMMOEngine;
using namespace nsLoginClientStruct;

TScLoginClient_ClientImpl::TScLoginClient_ClientImpl(IScenario* pSc):
TBaseScLoginClient(pSc)
{

}
//-----------------------------------------------------------------------------
void TScLoginClient_ClientImpl::RecvInherit(TDescRecvSession* pDesc)
{
  THeader* pHeader = (THeader*)pDesc->data;
  switch(pHeader->from)
  {
    case eSlave:
      RecvFromSlave(pDesc);
      break;
    case eMaster:
      RecvFromMaster(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//-----------------------------------------------------------------------------
void TScLoginClient_ClientImpl::Work(unsigned int time_ms)
{
  if( Context()->GetTimeWait() + eTimeWait < time_ms)
  {
    // ������ �� ��� �������
    TEventError event;
    event.code = LoginClientClientNoAnswer;
    Context()->GetSE()->AddEventCopy(&event, sizeof(event));
    End();
  }
}
//-----------------------------------------------------------------------------
void TScLoginClient_ClientImpl::TryLogin(unsigned int ip, unsigned short port, 
                                         void* data, int size)
{
  if(Begin()==false)
  {
    // ��������� ������
    GetLogger(STR_NAME_MMO_ENGINE)->
      WriteF_time("TScenarioLoginClient::TryLogin() scenario is not active.\n");
    BL_FIX_BUG();
    return;
  }
  // ������������ ������
  TBreakPacket bp;// ��������� ��� ����� ������
  bp.PushFront((char*)data,size);
  THeaderTryLoginC2M h;
  h.sizeData = size;
  bp.PushFront((char*)&h, sizeof(h));
  // �������� ����� ��� ������� �����������
  SetID_SessionClientMaster(Context()->GetMS()->Send(ip, port, bp));
  if(GetID_SessionClientMaster()==INVALID_HANDLE_SESSION)
  {
    // ��������� ������
    TEventError event;
    event.code = LoginClientClientMasterNotReady;
    Context()->GetSE()->AddEventCopy(&event, sizeof(event));
    End();
  }
  else
    SetTimeWaitForNow();
}
//-----------------------------------------------------------------------------
void TScLoginClient_ClientImpl::RecvFromSlave(TDescRecvSession* pDesc)
{
  THeader* pHeader = (THeader*)pDesc->data;
  switch(pHeader->subType)
  {
    case eCheckConnectToSlaveS2C:
      CheckConnectToSlaveS2C(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//--------------------------------------------------------------
void TScLoginClient_ClientImpl::RecvFromMaster(TDescRecvSession* pDesc)
{
  THeader* pHeader = (THeader*)pDesc->data;
  switch(pHeader->subType)
  {
    case eResultLoginM2C:
      ResultLoginM2C(pDesc);
      break;
    case eCheckLeaveQueueM2C:
      CheckLeaveQueueM2C(pDesc);
      break;
    case eInfoSlaveM2C:
      InfoSlaveM2C(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//--------------------------------------------------------------
void TScLoginClient_ClientImpl::CheckConnectToSlaveS2C(TDescRecvSession* pDesc)
{
  TEventResultLogin event;
  event.res = TMaster::eAccept;
  Context()->GetSE()->AddEventCopy(&event, sizeof(event));
  
  End();
}
//--------------------------------------------------------------
void TScLoginClient_ClientImpl::ResultLoginM2C(TDescRecvSession* pDesc)
{
  // �������� ����� �������
  SetTimeWaitForNow();
  //==============================
  THeaderResultLoginM2C* pH = (THeaderResultLoginM2C*)pDesc->data;
  switch(pH->result)
  {
    case THeaderResultLoginM2C::eAccept:
      // ��������� ���� ����
      Context()->SetIDClient(pH->id_client);
      SetIDClient(pH->id_client);
      break;
    case THeaderResultLoginM2C::eReject:
    {
      CloseSessionMaster();

      TEventResultLogin event;
      event.res = TMaster::eReject;
      // ��������� ������, ������� �������� ������� ������
      char* pDataResClient = pDesc->data + sizeof(THeaderResultLoginM2C);
      event.c.SetData( pDataResClient, pH->sizeResClient);
      Context()->GetSE()->AddEventCopy(&event, sizeof(event));
      End();
    }
      break;
    case THeaderResultLoginM2C::eQueue:
    {
      Context()->SetIDClient(pH->id_client);
      SetIDClient(pH->id_client);

      Context()->SetNumInQueue(pH->numInQueue);

      TEventEnterInQueue event;
      event.numInQueue = pH->numInQueue;
      Context()->GetSE()->AddEventCopy(&event, sizeof(event));
    }
      break;
    default:BL_FIX_BUG();
  }
}
//--------------------------------------------------------------
void TScLoginClient_ClientImpl::CheckLeaveQueueM2C(TDescRecvSession* pDesc)
{
  CloseSessionMaster();

  TEventLeaveQueue event;
  Context()->GetSE()->AddEventCopy(&event, sizeof(event));

  End();
}
//--------------------------------------------------------------
void TScLoginClient_ClientImpl::InfoSlaveM2C(TDescRecvSession* pDesc)
{
  // ���� ���������� ��������� � ������� �� Slave
  CloseSessionMaster();
  // ������� ��� ��� ��������
  THeaderInfoSlaveM2C* pInfoSlave = (THeaderInfoSlaveM2C*)pDesc->data;
  // ��������� ����� ��� Slave
  TBreakPacket bp;
  THeaderConnectToSlaveC2S h;
  h.id_client = pInfoSlave->id_client;// ��� Slave ������ ���� ID, �� �� ���� ��� ��������������
  bp.PushFront((char*)&h, sizeof(h));
  // ������� ������ �� IP:port
  unsigned int id_slave = 
    Context()->GetMS()->Send(pInfoSlave->ip_port_slave.ip,
                             pInfoSlave->ip_port_slave.port,
                             bp);
  // �������� �� ������� ���������� Slave
  if(id_slave==INVALID_HANDLE_SESSION)
  {
    // ��������� ������
    TEventError event;
    event.code = LoginClientClientNotExistSlave;
    Context()->GetSE()->AddEventCopy(&event, sizeof(event));
    End();
    return;
  }
  SetID_SessionClientSlave(id_slave);
  SetTimeWaitForNow();
  
  // ����� ��� �������, ��� �� �������������� ��� ���������
  // � ������� ����� ����� �������
  Context()->SetIDClient(pInfoSlave->id_client);
  SetIDClient(pInfoSlave->id_client);
}
//--------------------------------------------------------------
void TScLoginClient_ClientImpl::LeaveQueue()
{
  // �������� � ��������� ������� �� ������ � �������
  if(Context()->GetNumInQueue()==0)
    return;

  TBreakPacket bp;
  THeaderLeaveQueueC2M h;
  bp.PushFront((char*)&h, sizeof(h));

  unsigned int id_master = GetID_SessionClientMaster();
  if(id_master==INVALID_HANDLE_SESSION)
    return;

  Context()->GetMS()->Send(id_master,bp);
}
//--------------------------------------------------------------
void TScLoginClient_ClientImpl::CloseSessionMaster()
{
  Context()->GetMS()->CloseSession(GetID_SessionClientMaster());
  SetID_SessionClientMaster(INVALID_HANDLE_SESSION);
}
//--------------------------------------------------------------
void TScLoginClient_ClientImpl::SetIDClient(unsigned int id_client)
{
  mCallBackSetIDClient.Notify(id_client);
}
//--------------------------------------------------------------