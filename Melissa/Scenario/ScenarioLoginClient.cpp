/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ScenarioLoginClient.h"
#include "HiTimer.h"
#include "ManagerSession.h"
#include "Logger.h"
#include "Base.h"
#include "Events.h"
#include "ErrorCode.h"


using namespace nsMelissa;

TScenarioLoginClient::TScenarioLoginClient()
{

}
//--------------------------------------------------------------
TScenarioLoginClient::~TScenarioLoginClient()
{

}
//--------------------------------------------------------------
void TScenarioLoginClient::Work()
{
  
}
//--------------------------------------------------------------
void TScenarioLoginClient::Connect(unsigned int ip, unsigned short port, 
                                   void* data, int size)
{
  if(Begin()==false)
  {
    // ��������� ������
    GetLogger(STR_NAME_MELISSA)->
      WriteF_time("TScenarioLoginClient::Connect() scenario is not active.\n");
    BL_FIX_BUG();
    return;
  }
  // ������������ ������
  TBreakPacket bp;// ��������� ��� ����� ������
  bp.PushFront((char*)data,size);
  THeaderLoginClient h;
  bp.PushFront((char*)&h, sizeof(h));
  // �������� ����� ��� ������� �����������
  Context()->SetID_Session(Context()->GetMS()->Send(ip, port, bp));
  if(Context()->GetID_Session()==INVALID_HANDLE_SESSION)
  {
    // ��������� ������
    TEventError event;
    event.code = LoginClient_MasterNotReady;
    Context()->GetSE()->AddEventCopy(&event, sizeof(event));
    End();
  }
  else
    Context()->SetTimeWait(ht_GetMSCount() + eTimeWait);
}
//--------------------------------------------------------------
void TScenarioLoginClient::Recv(TDescRecvSession* pDesc)
{
  NeedContext(pDesc->id_session);
  THeaderLoginClient* pPacket = (THeaderLoginClient*)pDesc->data;
  switch(pPacket->subType)
  {
    case eToMaster:
      break;
    case eAcceptFromMaster:
      break;
    case eRejectFromMaster:
      break;
  }
}
//--------------------------------------------------------------
void TScenarioLoginClient::Start()
{
  if(Begin()==false)
  {
    // ��������� ������
    GetLogger(STR_NAME_MELISSA)->
      WriteF_time("TScenarioLoginClientByMaster1::Start() scenario is not active.\n");

    BL_FIX_BUG();
    return;
  }
  // ��������� ������� � ������� �����������
  TEventTryLogin event;
  event.id_session = Context()->GetID_Session();
  Context()->GetSE()->AddEventCopy(&event,sizeof(event));
}
//--------------------------------------------------------------
void TScenarioLoginClient::Reject(void* resForClient, int sizeResClient)
{
  Context()->Reject();

  TBreakPacket bp;
  THeaderRejectFromMaster h;
  //h.Set(resForClient, sizeResClient);
  bp.PushFront((char*)&h, sizeof(h));
  Context()->GetMS()->Send(Context()->GetID_Session(), bp);
}
//--------------------------------------------------------------
void TScenarioLoginClient::Accept(unsigned int key, void* resForClient, int sizeResClient, 
                                  TIP_Port& ip_port_Slave)
{
  Context()->Accept();

  THeaderAcceptFromMaster h;
  h.key = key;
  //h.Set(resForClient, sizeResClient);
  TBreakPacket bp;
  bp.PushFront((char*)&h, sizeof(h));
  Context()->GetMS()->Send(Context()->GetID_Session(), bp);
}
//--------------------------------------------------------------
