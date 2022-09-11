/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ScLoginClient_MasterImpl.h"
#include "ContextScLoginClient.h"
#include "ManagerSession.h"
#include "Base.h"
#include "Logger.h"
#include "Events.h"
#include "ErrorCode.h"

using namespace std;
using namespace nsMMOEngine;
using namespace nsLoginClientStruct;

TScLoginClient_MasterImpl::TScLoginClient_MasterImpl(IScenario* pSc):
TBaseScLoginClient(pSc)
{

}
//-----------------------------------------------------------------------------
void TScLoginClient_MasterImpl::RecvInherit(TDescRecvSession* pDesc)
{
  THeader* pHeader = (THeader*)pDesc->data;
  switch(pHeader->from)
  {
    case eClient:
      RecvFromClient(pDesc);
      break;
    case eSlave:
      RecvFromSlave(pDesc);
      break;
    case eSuperServer:
      RecvFromSuperServer(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//-----------------------------------------------------------------------------
void TScLoginClient_MasterImpl::Work(unsigned int time_ms)
{
  // ���� ������ � �������, �������� �����
  if(Context()->GetNumInQueue())
  {
    NeedNumInQueue(Context()->GetID_SessionClientMaster());
    return;
  }
  // 
  if(Context()->GetTimeWait() + eTimeWait > ht_GetMSCount())
  {
    // ������ �� ��� �������
    // ��������� � ��� ����, �� ������ ����
    TEventError event;
    event.code = LoginClientMasterClientNotActive;
    Context()->GetSE()->AddEventCopy(&event, sizeof(event));
    End();
  }
}
//-----------------------------------------------------------------------------
void TScLoginClient_MasterImpl::Reject(void* resForClient, int sizeResClient)
{
  Context()->Reject();

  // ������������ ����� ��� �������
  THeaderResultLoginM2C h;
  h.result = THeaderResultLoginM2C::eReject;
  h.sizeResClient = sizeResClient;

  TBreakPacket bp;
  bp.PushFront((char*)resForClient, sizeResClient);
  bp.PushFront((char*)&h, sizeof(h));

  Context()->GetMS()->Send(GetID_SessionClientMaster(), bp);

  End();
}
//--------------------------------------------------------------
void TScLoginClient_MasterImpl::Accept(unsigned int key, void* resForClient, int sizeResClient, 
                                       unsigned int id_session_slave, 
                                       unsigned int id_superserver)
{
  SetID_SessionMasterSS(id_superserver);
  // ��������� �� �������
  SetID_SessionMasterSlave(id_session_slave);
  // ��������� ��������� ���������
  Context()->Accept();

  // ���� SuperServer ����������, �� �������� ������ ���
  // ���� ���, �� �������� ����� ������� �����
  if(GetID_SessionMasterSS()==INVALID_HANDLE_SESSION)
  {
    // ����� ����� ������� � Slave
    SendResultAccept2ClientAndSlave(key,
                                    resForClient, sizeResClient);
    return;
  }
  
  Context()->SaveAcceptData(resForClient, sizeResClient);
  // SuperServer
  TBreakPacket bp;
  THeaderRequestM2SS h;
  h.id_client = key;
  bp.PushFront((char*)&h, sizeof(h));
  Context()->GetMS()->Send(GetID_SessionMasterSS(), bp);
}
//--------------------------------------------------------------
void TScLoginClient_MasterImpl::Queue(int num)
{
  Context()->SetNumInQueue(num);

  TBreakPacket bp;
  THeaderResultLoginM2C h;
  h.result     = THeaderResultLoginM2C::eQueue;
  h.numInQueue = num;
  bp.PushFront((char*)&h, sizeof(h));
  Context()->GetMS()->Send(GetID_SessionClientMaster(), bp);
}
//--------------------------------------------------------------
void TScLoginClient_MasterImpl::NeedNumInQueue(unsigned int id_Session)
{
  mCallBackNeedNumInQueue.Notify(id_Session);
}
//---------------------------------------------------------------------
void TScLoginClient_MasterImpl::RecvFromClient(TDescRecvSession* pDesc)
{
  THeader* pHeader = (THeader*)pDesc->data;
  switch(pHeader->subType)
  {
    case eTryLoginC2M:
      TryLoginC2M(pDesc);
      break;
    case eLeaveQueueC2M:
      LeaveQueueC2M(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//--------------------------------------------------------------
void TScLoginClient_MasterImpl::RecvFromSlave(TDescRecvSession* pDesc)
{
  THeader* pHeader = (THeader*)pDesc->data;
  switch(pHeader->subType)
  {
    case eClientConnectS2M:
      ClientConnectS2M(pDesc);
      break;
    case eCheckInfoClientS2M:
      CheckInfoClientS2M(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//--------------------------------------------------------------
void TScLoginClient_MasterImpl::RecvFromSuperServer(TDescRecvSession* pDesc)
{
  THeader* pHeader = (THeader*)pDesc->data;
  switch(pHeader->subType)
  {
    case eCheckRequestSS2M:
      CheckRequestSS2M(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//--------------------------------------------------------------
void TScLoginClient_MasterImpl::CheckRequestSS2M(TDescRecvSession* pDesc)
{
  THeaderCheckRequestSS2M* pHeader = (THeaderCheckRequestSS2M*)pDesc->sizeData;
  // ������ ������ SuperServer-�
  if(pHeader->isExistInSystem)
  {
    Context()->Reject();
    // ������ ��� ���� � �������, ��������� �����������
    string sReject = "Reject login. Client was been authorized.";
    // ������������ ����� ��� �������
    THeaderResultLoginM2C h;
    h.result = THeaderResultLoginM2C::eReject;
    h.sizeResClient = sReject.length();
    
    TBreakPacket bp;
    bp.PushFront((char*)sReject.data(), sReject.length());
    bp.PushFront((char*)&h, sizeof(h));

    Context()->GetMS()->Send(GetID_SessionClientMaster(), bp);
    End();
    return;
  }
  //
  void* resForClient  = Context()->GetSaveAcceptDataPtr();
  int   sizeResClient = Context()->GetSaveAcceptDataSize();

  Context()->Accept();

  SendResultAccept2ClientAndSlave(pHeader->id_client,
                                  resForClient,sizeResClient);
}
//--------------------------------------------------------------
void TScLoginClient_MasterImpl::TryLoginC2M(TDescRecvSession* pDesc)
{
  if(Begin()==false)
  {
    // ��������� ������
    GetLogger(STR_NAME_MMO_ENGINE)->
      WriteF_time("TScLoginClient_MasterImpl::TryLoginC2M() scenario is not active.\n");
    BL_FIX_BUG();
    return;
  }
  SetTimeWaitForNow();
  // ����� ������, ���������
  SetID_SessionClientMaster(pDesc->id_session);
  // � ������, ������� ��������, ���������� ��������� � ����
  // ������ ����� �������� � ���������
  THeaderTryLoginC2M* pPacket = (THeaderTryLoginC2M*)pDesc->data;
  char* data = pDesc->data + sizeof(THeaderTryLoginC2M);
  // ��������� ������� � ������� �����������
  TEventTryLogin event;
  event.id_session = GetID_SessionClientMaster();
  event.c.SetData(data,pPacket->sizeData);
  Context()->GetSE()->AddEventCopy(&event,sizeof(event));
}
//--------------------------------------------------------------
void TScLoginClient_MasterImpl::LeaveQueueC2M(TDescRecvSession* pDesc)
{
  if(Context()->GetNumInQueue()==0)
    return;

  NeedLeaveFromQueue(pDesc->id_session);

  THeaderCheckLeaveM2C h;
  TBreakPacket bp;
  bp.PushFront((char*)&h, sizeof(h));
  Context()->GetMS()->Send(GetID_SessionClientMaster(),bp);

  End();
}
//--------------------------------------------------------------
void TScLoginClient_MasterImpl::ClientConnectS2M(TDescRecvSession* pDesc)
{
  THeaderClientConnectS2M* pHeader = (THeaderClientConnectS2M*)pDesc->data;
  // ��������� � �������
  THeaderCheckClientConnectM2S h;
  h.id_client = pHeader->id_client;
  TBreakPacket bp;
  bp.PushFront((char*)&h, sizeof(h));
  Context()->GetMS()->Send(GetID_SessionMasterSlave(), bp);

  // ����� ��������
  End();
}
//--------------------------------------------------------------
void TScLoginClient_MasterImpl::CheckInfoClientS2M(TDescRecvSession* pDesc)
{
  // ip � port Slave ��� �������
  TIP_Port ip_port_slave;
  Context()->GetMS()->GetInfo(GetID_SessionMasterSlave(), ip_port_slave);

  // �������� ���������� � Slave
  THeaderInfoSlaveM2C h;
  h.ip_port_slave = ip_port_slave;
  TBreakPacket bp;
  bp.PushFront((char*)&h, sizeof(h));
  Context()->GetMS()->Send(GetID_SessionClientMaster(), bp);
}
//--------------------------------------------------------------
void TScLoginClient_MasterImpl::SendResultAccept2ClientAndSlave(unsigned int key, 
                                                                void* resForClient, 
                                                                int sizeResClient)
{
  // Client
  TBreakPacket bp;
  bp.PushFront((char*)resForClient, sizeResClient);
  THeaderResultLoginM2C hForClient;
  hForClient.result        = THeaderResultLoginM2C::eAccept;
  hForClient.sizeResClient = sizeResClient;
  bp.PushFront((char*)&hForClient, sizeof(hForClient));
  Context()->GetMS()->Send(GetID_SessionClientMaster(), bp);
  bp.UnlinkPart();// �������� �� ������
  // Slave
  THeaderInfoClientM2S hForSlave;
  hForSlave.id_client = key;
  bp.PushFront((char*)&hForClient, sizeof(hForClient));
  Context()->GetMS()->Send(GetID_SessionMasterSlave(), bp);
}
//--------------------------------------------------------------
void TScLoginClient_MasterImpl::NeedLeaveFromQueue(unsigned int id_session)
{
  mCallBackNeedLeaveFromQueue.Notify(id_session);
}
//--------------------------------------------------------------