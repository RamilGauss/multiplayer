/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ScLoginClient_SlaveImpl.h"
#include "ContextScLoginClient.h"
#include "ManagerSession.h"
#include "Base.h"
#include "Logger.h"
#include "Events.h"
#include "ErrorCode.h"

using namespace nsMMOEngine;
using namespace nsLoginClientStruct;

TScLoginClient_SlaveImpl::TScLoginClient_SlaveImpl(IScenario* pSc):
TBaseScLoginClient(pSc)
{

}
//-----------------------------------------------------------------------------
void TScLoginClient_SlaveImpl::RecvInherit(TDescRecvSession* pDesc)
{
  THeader* pHeader = (THeader*)pDesc->data;
  switch(pHeader->from)
  {
    case eClient:
      RecvFromClient(pDesc);
      break;
    case eMaster:
      RecvFromMaster(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//-----------------------------------------------------------------------------
void TScLoginClient_SlaveImpl::Work(unsigned int time_ms)
{

}
//-----------------------------------------------------------------------------
void TScLoginClient_SlaveImpl::RecvFromClient(TDescRecvSession* pDesc)
{
  THeader* pHeader = (THeader*)pDesc->data;
  switch(pHeader->subType)
  {
    case eConnectToSlaveC2S:
      ConnectToSlaveC2S(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//--------------------------------------------------------------
void TScLoginClient_SlaveImpl::RecvFromMaster(TDescRecvSession* pDesc)
{
  THeader* pHeader = (THeader*)pDesc->data;
  switch(pHeader->subType)
  {
    case eInfoClientM2S:
      InfoClientM2S(pDesc);
      break;
    case eCheckClientConnectM2S:
      CheckClientConnectM2S(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//--------------------------------------------------------------
void TScLoginClient_SlaveImpl::ConnectToSlaveC2S(TDescRecvSession* pDesc)
{
  THeaderConnectToSlaveC2S* pHeader = (THeaderConnectToSlaveC2S*)pDesc->data;
  // ���������� �� ������ ������ � ������ ID,
  // �� ���� ��� �� �������� �� �������� �� ������� ������ ID
  NeedIsExistClientID(pHeader->id_client);
}
//--------------------------------------------------------------
void TScLoginClient_SlaveImpl::SetIsExistClientID(bool isExist, unsigned int id_client)
{
  if(isExist==false)
  {
    // ��������� ������
    GetLogger(STR_NAME_MMO_ENGINE)->
      WriteF_time("TScLoginClient_SlaveImpl::SetIsExistClientID() id client is not exist.\n");
    BL_FIX_BUG();
    return;
  }
  // ��������� �������� ��� ������
  NeedContextByKeyClient(id_client);

  // ��������� ������� � ������� �� �������
  THeaderClientConnectS2M h;
  h.id_client = id_client;// ������� ����� ������ ������� �����������
  TBreakPacket bp;
  bp.PushFront((char*)&h, sizeof(h));

  Context()->GetMS()->Send(GetID_SessionMasterSlave(),bp);
}
//--------------------------------------------------------------
void TScLoginClient_SlaveImpl::InfoClientM2S(TDescRecvSession* pDesc)
{
  // ������ ��������
  if(Begin()==false)
  {
    // ��������� ������
    GetLogger(STR_NAME_MMO_ENGINE)->
      WriteF_time("TScLoginClient_SlaveImpl::InfoClientM2S() scenario is not active.\n");
    BL_FIX_BUG();
    return;
  }
  // ��������� ������
  SetID_SessionMasterSlave(pDesc->id_session);
  
  THeaderInfoClientM2S* pHeader = (THeaderInfoClientM2S*)pDesc->data;
  // ������������ ���������
  TBreakPacket bp;
  THeaderCheckInfoClientS2M h;
  h.id_client = pHeader->id_client;
  bp.PushFront((char*)&h, sizeof(h));

  Context()->GetMS()->Send(GetID_SessionMasterSlave(), bp);
}
//--------------------------------------------------------------
void TScLoginClient_SlaveImpl::CheckClientConnectM2S(TDescRecvSession* pDesc)
{
  // ������� ����������� Developer Slave ������� Connect
  TEventDisconnectDown event;
  event.id_session = pDesc->id_session;
  Context()->GetSE()->AddEventCopy(&event, sizeof(event));
  // �������� ������� �����������
  THeaderCheckConnectToSlaveS2C h;
  TBreakPacket bp;
  bp.PushFront((char*)&h, sizeof(h));

  Context()->GetMS()->Send(GetID_SessionClientSlave(),bp);
  End();
}
//--------------------------------------------------------------
