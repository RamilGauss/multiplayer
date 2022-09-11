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
  // ������ �� ����
  if(pDesc->sizeData<sizeof(THeader))
    return;
  //=======================================
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
  if(Context()->GetTimeWait() + eTimeWait < ht_GetMSCount())
  {
    // ������ �� ��� �������
    // ��������� � ��� ����, �� ������ ����
    TEventError event;
    event.code = LoginClientSlaveNoAnswer;
    Context()->GetSE()->AddEventCopy(&event, sizeof(event));
    End();
  }
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
    case eDisconnectClientM2S:
      DisconnectClientM2S(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//--------------------------------------------------------------
void TScLoginClient_SlaveImpl::ConnectToSlaveC2S(TDescRecvSession* pDesc)
{
  // ������ �� ����
  if(pDesc->sizeData!=sizeof(THeaderConnectToSlaveC2S))
    return;
  
  THeaderConnectToSlaveC2S* pHeader = (THeaderConnectToSlaveC2S*)pDesc->data;
  // ���������� �� ������ ������ � ������ ������,
  // �� ���� ��� �� �������� �� �������� �� ������� ������ ������
  // ��������� �������� ��� ������
  NeedContextByClientSessionByClientKey(pDesc->id_session,
                                        pHeader->id_client);

  if(Context()==NULL)
  {
    // ��������� ������
    GetLogger(STR_NAME_MMO_ENGINE)->
      WriteF_time("TScLoginClient_SlaveImpl::SetIsExistClientID() id client is not exist.\n");
    BL_FIX_BUG();
    return;
  }
  // ��������� ������ �������
  SetID_SessionClientSlave(pDesc->id_session);

  // ��������� ������� � ������� �� �������
  THeaderClientConnectS2M h;
  h.id_client = Context()->GetIDClient();// ������� ����� ������ ������� �����������
  TBreakPacket bp;
  bp.PushFront((char*)&h, sizeof(h));

  Context()->GetMS()->Send(GetID_SessionMasterSlave(),bp);
}
//--------------------------------------------------------------
void TScLoginClient_SlaveImpl::InfoClientM2S(TDescRecvSession* pDesc)
{
  THeaderInfoClientM2S* pHeader = (THeaderInfoClientM2S*)pDesc->data;
  NeedContextByClientKey(pHeader->id_client);
  if(Context()==NULL)
  {
    BL_FIX_BUG();
    return;
  }
  //--------------------------------------------
  // ������ ��������
	if(Context()->WasBegin()==false)
	{
		// ���������� �������
		Context()->SetWasBegin();
		if(Begin()==false)
		{
			// ��������� ������
			GetLogger(STR_NAME_MMO_ENGINE)->
				WriteF_time("TScLoginClient_SlaveImpl::InfoClientM2S() scenario is not active.\n");
			BL_FIX_BUG();
			return;
		}
	}
	// ���� WasBegin==true - ����� ��� ���, ������ �� �������� ������, ��������� � ���� ��������
	// ����� ������ ��� ��� ��������� ��������������, � Salve ��� ��� ����, �.�. ��� ���
	// ������ ������� �����. ��� � ��������� �����������.

	SetTimeWaitForNow();
	// ��������� ������
  SetID_SessionMasterSlave(pDesc->id_session);
  
	Context()->SetIDClient(pHeader->id_client);
  // ������������ ���������
  TBreakPacket bp;
  THeaderCheckInfoClientS2M h;
  h.id_client = Context()->GetIDClient();
  bp.PushFront((char*)&h, sizeof(h));

  Context()->GetMS()->Send(GetID_SessionMasterSlave(), bp);
}
//--------------------------------------------------------------
void TScLoginClient_SlaveImpl::CheckClientConnectM2S(TDescRecvSession* pDesc)
{
  THeaderCheckClientConnectM2S* pHeader = (THeaderCheckClientConnectM2S*)pDesc->data;
  NeedContextByClientKey(pHeader->id_client);
  if(Context()==NULL)
  {
    BL_FIX_BUG();
    return;
  }
  //--------------------------------------------
	// ������� ����������� Developer Slave ������� Connect
  TEventConnectDown event;
  event.id_session = GetID_SessionClientSlave();
  Context()->GetSE()->AddEventCopy(&event, sizeof(event));
  // �������� ������� �����������
  THeaderCheckConnectToSlaveS2C h;
  TBreakPacket bp;
  bp.PushFront((char*)&h, sizeof(h));

  Context()->GetMS()->Send(GetID_SessionClientSlave(),bp);
  Context()->Accept();
  End();
}
//--------------------------------------------------------------
void TScLoginClient_SlaveImpl::DisconnectClientM2S(TDescRecvSession* pDesc)
{
  THeaderDisconnectClientM2S* pH = (THeaderDisconnectClientM2S*)pDesc->data;
  NeedContextByClientKey(pH->id_client);
  
  if(Context()==NULL)
  {
    BL_FIX_BUG();
    return;
  }
  Context()->Reject();
  Context()->GetMS()->CloseSession(Context()->GetID_SessionClientSlave());
  End();  
}
//--------------------------------------------------------------
