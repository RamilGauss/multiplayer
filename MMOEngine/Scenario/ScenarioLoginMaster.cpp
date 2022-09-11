/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/


#include "ScenarioLoginMaster.h"
#include "Logger.h"
#include "ManagerSession.h"
#include "Events.h"
#include "ErrorCode.h"

using namespace nsMMOEngine;

TScenarioLoginMaster::TScenarioLoginMaster()
{

}
//--------------------------------------------------------------
TScenarioLoginMaster::~TScenarioLoginMaster()
{

}
//--------------------------------------------------------------
void TScenarioLoginMaster::ConnectToSuperServer(unsigned int ip, unsigned short port)
{
  Context()->SetConnect(false);
  if(Begin()==false)
  {
    End();
    // ������� ���������� ������ ����������� ������� �������� - ������ �� ������ ����
    // ���������� ������
    GetLogger(STR_NAME_MMO_ENGINE)->
      WriteF_time("TScenarioLoginMaster::ConnectToSuperServer() scenario is not active.\n");
    BL_FIX_BUG();
    return;
  }
  Context()->GetMS()->CloseSession(Context()->GetID_Session());
  // ������� �����
  TBreakPacket bp;
  THeaderFromMaster h;
  bp.PushFront((char*)&h, sizeof(h));
  Context()->SetID_Session( Context()->GetMS()->Send(ip, port, bp));
  if(Context()->GetID_Session()==INVALID_HANDLE_SESSION)
  {
    // ��������� ������
    TEventError event;
    event.code = LoginMaster_SSNotReady;
    Context()->GetSE()->AddEventCopy(&event, sizeof(event));
    End();
  }
  else
    Context()->SetTimeWait(ht_GetMSCount() + eTimeoutWait_ms);
}
//--------------------------------------------------------------
void TScenarioLoginMaster::Work()
{
  unsigned int now = ht_GetMSCount();
  if(Context()->GetTimeWait()<now)
  {
    // ������ �� ��� �������
    TEventError event;
    event.code = LoginMaster_NoAnswerFromSS;
    Context()->GetSE()->AddEventCopy(&event, sizeof(event));
    End();
  }
}
//--------------------------------------------------------------
void TScenarioLoginMaster::RecvFromSuperServer(TDescRecvSession* pDesc)
{
  Context()->SetConnect(true);
  End();
}
//-------------------------------------------------------------------------------------
void TScenarioLoginMaster::RecvFromMaster(TDescRecvSession* pDesc)
{
  if(Begin()==false)
  {
    End();
    // ������� ���������� ������ ����������� ������� �������� - ������ �� ������ ����
    // ���������� ������
    GetLogger(STR_NAME_MMO_ENGINE)->
      WriteF_time("TScenarioLoginMaster::RecvFromMaster() scenario is not active.\n");
    BL_FIX_BUG();
    return;
  }
  Context()->SetID_Session(pDesc->id_session);

  TBreakPacket bp;
  THeaderAnswerFromSS h;
  bp.PushFront((char*)&h, sizeof(h));
  Context()->GetMS()->Send(Context()->GetID_Session(), bp);
  End();
}
//-------------------------------------------------------------------------------------
void TScenarioLoginMaster::Recv(TDescRecvSession* pDesc)
{
  NeedContext(pDesc->id_session);
  THeaderLoginMaster* pPacket = (THeaderLoginMaster*)pDesc->data;
  switch(pPacket->subType)
  {
    case eFromMaster:
      RecvFromMaster(pDesc);
      break;
    case eAnswerFromSS:
      RecvFromSuperServer(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//-------------------------------------------------------------------------------------
