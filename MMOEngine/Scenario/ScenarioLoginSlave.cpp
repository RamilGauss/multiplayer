/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ScenarioLoginSlave.h"
#include "BL_Debug.h"
#include "ManagerSession.h"
#include "Logger.h"
#include "HiTimer.h"
#include "Events.h"
#include "ErrorCode.h"

using namespace nsMMOEngine;

TScenarioLoginSlave::TScenarioLoginSlave()
{

}
//-------------------------------------------------------------------------------------
void TScenarioLoginSlave::ConnectToMaster( unsigned int ip, unsigned short port )
{
  Context()->SetConnect(false);
  if(Begin()==false)
  {
    // ������� ���������� ������ ����������� ������� �������� - ������ �� ������ ����
    // ��������� ������
    GetLogger(STR_NAME_MMO_ENGINE)->
      WriteF_time("TScenarioLoginSlave::ConnectToMaster() scenario is not active.\n");
    BL_FIX_BUG();
    return;
  }
  // ������� ����������
  Context()->GetMS()->CloseSession(Context()->GetID_Session());
  TBreakPacket bp;
  THeaderFromSlave h;
  bp.PushFront((char*)&h, sizeof(h));
  Context()->SetID_Session( Context()->GetMS()->Send(ip, port, bp) );
  if(Context()->GetID_Session()==INVALID_HANDLE_SESSION)
  {
    // ��������� ������
    TEventError event;
    event.code = LoginSlave_MasterNotReady;
    Context()->GetSE()->AddEventCopy(&event, sizeof(event));
    End();
  }
  else
    Context()->SetTimeWait(ht_GetMSCount() + eTimeoutWait_ms);
}
//-------------------------------------------------------------------------------------
void TScenarioLoginSlave::Work()
{
  unsigned int now = ht_GetMSCount();
  if(Context()->GetTimeWait()<now)
  {
    // ��������� ������
    TEventError event;
    event.code = LoginSlave_NoAnswerFromMaster;
    Context()->GetSE()->AddEventCopy(&event, sizeof(event));
    End();
  }
}
//-------------------------------------------------------------------------------------
void TScenarioLoginSlave::RecvFromMaster(TDescRecvSession* pDesc)
{
  Context()->SetConnect(true);
  End();
}
//-------------------------------------------------------------------------------------
void TScenarioLoginSlave::RecvFromSlave(TDescRecvSession* pDesc)
{
  if(Begin()==false)
  {
    End();
    // ������� ���������� ������ ����������� ������� �������� - ������ �� ������ ����
    // ���������� ������
    GetLogger(STR_NAME_MMO_ENGINE)->
      WriteF_time("TScenarioLoginSlave::RecvFromSlave() scenario is not active.\n");
    BL_FIX_BUG();
    return;
  }
  Context()->SetID_Session(pDesc->id_session);

  TBreakPacket bp;
  THeaderAnswerMaster h;
  bp.PushFront((char*)&h, sizeof(h));
  Context()->GetMS()->Send(Context()->GetID_Session(), bp);

  End();
}
//--------------------------------------------------------------------------
void TScenarioLoginSlave::Recv(TDescRecvSession* pDesc)
{
  NeedContext(pDesc->id_session);
  THeaderLoginSlave* pPacket = (THeaderLoginSlave*)pDesc->data;
  switch(pPacket->subType)
  {
    case eFromSlave:
      RecvFromSlave(pDesc);
      break;
    case eAnswerMaster:
      RecvFromMaster(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//--------------------------------------------------------------------------  
