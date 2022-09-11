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

using namespace nsMelissa;

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
    // ������� ���������� ������ ����������� ������� �������� - ������ �� ������ ����
    // ��������� ������
    GetLogger()->Get(STR_NAME_MELISSA)->
      WriteF_time("TScenarioLoginMaster::ConnectToSuperServer() �������� ���������.\n");
    BL_FIX_BUG();
    return;
  }
  Context()->GetMS()->CloseSession(Context()->GetID_Session());
  // ������� �����
  TBreakPacket bp;
  THeaderLoginMaster h;
  bp.PushBack((char*)&h, sizeof(h));
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
void TScenarioLoginMaster::RecvFromSuperServer()
{
  Context()->SetConnect(true);
  End();
}
//-------------------------------------------------------------------------------------
void TScenarioLoginMaster::RecvFromMaster()
{
  TBreakPacket bp;
  THeaderAnswerFromSS h;
  bp.PushBack((char*)&h, sizeof(h));
  Context()->GetMS()->Send(Context()->GetID_Session(), bp);
}
//-------------------------------------------------------------------------------------
void TScenarioLoginMaster::Recv(TDescRecvSession* pDesc)
{
  NeedContext(pDesc->id_session);
  THeaderLoginMaster* pPacket = (THeaderLoginMaster*)pDesc->data;
  switch(pPacket->subType)
  {
    case eFromMaster:
      RecvFromMaster();
      break;
    case eAnswerFromSS:
      RecvFromSuperServer();
      break;
    default:BL_FIX_BUG();
  }
}
//-------------------------------------------------------------------------------------
