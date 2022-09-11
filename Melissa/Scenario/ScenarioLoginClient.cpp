/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
√удаков –амиль —ергеевич 
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
#include "BL_Debug.h"

using namespace nsMelissa;
using namespace std;

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
  unsigned int now = ht_GetMSCount();
  if(Context()->GetTimeWait()<now)
  {
    // ошибка на той стороне
    TEventError event;
    event.code = LoginClientNoAnswer;
    Context()->GetSE()->AddEventCopy(&event, sizeof(event));
    End();
  }
}
//--------------------------------------------------------------
void TScenarioLoginClient::TryLogin(unsigned int ip, unsigned short port, 
                                   void* data, int size)
{
  if(Begin()==false)
  {
    // генераци€ ошибки
    GetLogger(STR_NAME_MELISSA)->
      WriteF_time("TScenarioLoginClient::TryLogin() scenario is not active.\n");
    BL_FIX_BUG();
    return;
  }
  // формирование пакета
  TBreakPacket bp;// контейнер дл€ всего пакета
  bp.PushFront((char*)data,size);
  THeaderTryLoginC2M h;
  h.sizeData = size;
  bp.PushFront((char*)&h, sizeof(h));
  // отослать пакет дл€ попытки авторизации
  SetID_SessionClientMaster(Context()->GetMS()->Send(ip, port, bp));
  if(GetID_SessionClientMaster()==INVALID_HANDLE_SESSION)
  {
    // √енераци€ ошибки
    TEventError event;
    event.code = LoginClient_MasterNotReady;
    Context()->GetSE()->AddEventCopy(&event, sizeof(event));
    End();
  }
  else
    SetTimeWaitForNow();
}
//--------------------------------------------------------------
void TScenarioLoginClient::Recv(TDescRecvSession* pDesc)
{
  THeader* pPacket = (THeader*)pDesc->data;
  SetupContext(pPacket, pDesc->id_session);
  switch(pPacket->subType)
  {
    case eTryLoginC2M:
      TryLoginC2M(pDesc);
      break;
    case eRejectM2C:
      RejectM2C(pDesc);
      break;
    case eAcceptM2S:
      AcceptM2S(pDesc);
      break;
    case eQueueM2C:
      QueueM2C(pDesc);
      break;
    case eLeaveQueueC2M:
      LeaveQueueC2M(pDesc);
      break;
    case eCheckLeaveQueueM2C:
      CheckLeaveQueueM2C(pDesc);
      break;
    //------------------------------------------
    case eCheckAddContentS2M:
      CheckAddContentS2M(pDesc);
      break;
    case eInfoSlaveM2C:
      InfoSlaveM2C(pDesc);
      break;
    case eLoginContentC2S:
      LoginContentC2S(pDesc);
      break;
    case eClientLoginS2M:
      ClientLoginS2M(pDesc);
      break;
    case eClientInClusterM2SS:
      ClientInClusterM2SS(pDesc);
      break;
    case eCheckClientInClusterSS2M:
      CheckClientInClusterSS2M(pDesc);
      break;
    case eCheckClientLoginM2S:
      CheckClientLoginM2S(pDesc);
      break;
    case eClientInClusterS2C:
      ClientInClusterS2C(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//--------------------------------------------------------------
void TScenarioLoginClient::TryLoginC2M(TDescRecvSession* pDesc)// Master
{
  if(Begin()==false)
  {
    // генераци€ ошибки
    GetLogger(STR_NAME_MELISSA)->
      WriteF_time("TScenarioLoginClient::TryLoginC2M() scenario is not active.\n");

    BL_FIX_BUG();
    return;
  }
  SetTimeWaitForNow();
  // нова€ сесси€, сохранить
  SetID_SessionClientMaster(pDesc->id_session);
  // в буфере, который передали, содержитс€ заголовок и блок
  // размер блока прописан в заголовке
  THeaderTryLoginC2M* pPacket = (THeaderTryLoginC2M*)pDesc->data;
  char* data = pDesc->data + sizeof(THeaderTryLoginC2M);
  // генераци€ событи€ о попытке авторизации
  TEventTryLogin event;
  event.id_session = GetID_SessionClientMaster();
  event.c.SetData(data,pPacket->sizeData);
  Context()->GetSE()->AddEventCopy(&event,sizeof(event));
}
//--------------------------------------------------------------
void TScenarioLoginClient::Reject(void* resForClient, int sizeResClient)// Master
{
  Context()->Reject();

  TBreakPacket bp;
  bp.PushFront((char*)resForClient, sizeResClient);
  THeaderRejectM2C h;
  h.sizeResClient = sizeResClient;
  bp.PushFront((char*)&h, sizeof(h));
  Context()->GetMS()->Send(GetID_SessionClientMaster(), bp);
  End();
}
//--------------------------------------------------------------
void TScenarioLoginClient::Accept(unsigned int key, void* resForClient, int sizeResClient, 
                                  unsigned int id_session_slave)// Master
{
  SetID_SessionMasterSlave(id_session_slave);
  Context()->Accept();

  TBreakPacket bp;
  bp.PushFront((char*)resForClient, sizeResClient);
  THeaderAcceptM2S h;
  h.key_client = key;
  h.sizeResClient = sizeResClient;
  bp.PushFront((char*)&h, sizeof(h));
  Context()->GetMS()->Send(GetID_SessionMasterSlave(), bp);
}
//--------------------------------------------------------------
void TScenarioLoginClient::Queue(int num)// Master
{
  Context()->SetNumInQueue(num);

  TBreakPacket bp;
  THeaderQueueM2C h;
  h.numInQueue = num;
  bp.PushFront((char*)&h, sizeof(h));
  Context()->GetMS()->Send(GetID_SessionClientMaster(), bp);
}
//--------------------------------------------------------------
unsigned int TScenarioLoginClient::GetID_SessionClientMaster()
{
  return Context()->GetID_SessionClientMaster();
}
//--------------------------------------------------------------
void TScenarioLoginClient::SetID_SessionClientMaster(unsigned int id)
{
  Context()->SetID_SessionClientMaster(id);
}
//--------------------------------------------------------------
unsigned int TScenarioLoginClient::GetID_SessionClientSlave()
{
  return Context()->GetID_SessionClientSlave();
}
//--------------------------------------------------------------
void TScenarioLoginClient::SetID_SessionClientSlave(unsigned int id)
{
  Context()->SetID_SessionClientSlave(id);
}
//--------------------------------------------------------------
unsigned int TScenarioLoginClient::GetID_SessionMasterSlave()
{
  return Context()->GetID_SessionMasterSlave();
}
//--------------------------------------------------------------
void TScenarioLoginClient::SetID_SessionMasterSlave(unsigned int id)
{
  Context()->SetID_SessionMasterSlave(id);
}
//--------------------------------------------------------------
unsigned int TScenarioLoginClient::GetID_SessionMasterSS()
{
  return Context()->GetID_SessionMasterSS();
}
//--------------------------------------------------------------
void TScenarioLoginClient::SetID_SessionMasterSS(unsigned int id)
{
  Context()->SetID_SessionMasterSS(id);
}
//--------------------------------------------------------------
void TScenarioLoginClient::SetTimeWaitForNow()
{
  Context()->SetTimeWait(ht_GetMSCount() + eTimeWait);
}
//--------------------------------------------------------------
void TScenarioLoginClient::NeedContextByKeyClient(unsigned int key)
{
  mCallBackNeedContextForKeyClient.Notify(key);
}
//---------------------------------------------------------------------
/*—начала, когда приходит пакет от клиента мастеру, мастер отличает одного клиента от другого
  по ID_session, но потом он различает клиентов по key_client.
  Ќапример, ћастер отослал пакет Slave и указал что дело касаетс€ клиента с ключом key.
  Slave же отошлет мастеру ответ, в котором пропишет этот же ключ.
  “о же самое при обмене пакетов между мастером и супер-сервером. 
   лиент же, при получении пакета от ћастера, не нуждаетс€ в идентификации контекста,
  потому что верхнее соединение всегда одно, соответственно контекст тоже всегда один.
  */
void TScenarioLoginClient::SetupContext(THeader* pPacket, unsigned int id_session)
{
  switch(pPacket->subType)
  {
    case eTryLoginC2M:
    // ниже вообще по фиг куда, все равно верхнее соединение не мен€ет контекст в клиенте    
    case eQueueM2C:
    case eRejectM2C:
    case eInfoSlaveM2C:
    case eClientInClusterS2C:
    case eLeaveQueueC2M:
    case eCheckLeaveQueueM2C:
      NeedContext(id_session);
      break;
    case eAcceptM2S:
    case eCheckAddContentS2M:
    case eLoginContentC2S:
    case eClientLoginS2M:
    case eClientInClusterM2SS:
    case eCheckClientInClusterSS2M:
    case eCheckClientLoginM2S:
      NeedContextByKeyClient(pPacket->key_client);
      break;
    default:BL_FIX_BUG();
  }
}
//---------------------------------------------------------------------
void TScenarioLoginClient::AcceptM2S(TDescRecvSession* pDesc)// Slave
{
  THeaderAcceptM2S* pPacket = (THeaderAcceptM2S*)pDesc->data;
  // согласие, отослать пакет ћастеру
  TBreakPacket bp;
  THeaderCheckAddContentS2M h;
  h.key_client = pPacket->key_client;
  bp.PushFront((char*)&h, sizeof(h));
  
  Context()->GetMS()->Send(GetID_SessionMasterSlave(), bp);
}
//--------------------------------------------------------------
void TScenarioLoginClient::RejectM2C(TDescRecvSession* pDesc)// Client
{
  // отказ, учесть и закончить сценарий
  Context()->Reject();
  End();
}
//--------------------------------------------------------------
void TScenarioLoginClient::QueueM2C(TDescRecvSession* pDesc)// Client
{
  // обновить врем€ таймера
  SetTimeWaitForNow();
  THeaderQueueM2C* pPacket = (THeaderQueueM2C*)pDesc->data;

  TEventEnterInQueue event;
  event.numInQueue = pPacket->numInQueue;
  Context()->GetSE()->AddEventCopy(&event, sizeof(event));
}
//--------------------------------------------------------------
void TScenarioLoginClient::LeaveQueueC2M(TDescRecvSession* pDesc)// Master
{

}
//---------------------------------------------------------------------
void TScenarioLoginClient::CheckLeaveQueueM2C(TDescRecvSession* pDesc)// Client
{

}
//---------------------------------------------------------------------
void TScenarioLoginClient::CheckAddContentS2M(TDescRecvSession* pDesc)// Master
{

}
//---------------------------------------------------------------------
void TScenarioLoginClient::InfoSlaveM2C(TDescRecvSession* pDesc)// Client
{

}
//---------------------------------------------------------------------
void TScenarioLoginClient::LoginContentC2S(TDescRecvSession* pDesc)// Slave
{

}
//---------------------------------------------------------------------
void TScenarioLoginClient::ClientLoginS2M(TDescRecvSession* pDesc)// Master
{

}
//---------------------------------------------------------------------
void TScenarioLoginClient::ClientInClusterM2SS(TDescRecvSession* pDesc)// SuperServer
{
  THeaderClientInClusterM2SS* pPacket = (THeaderClientInClusterM2SS*)pDesc->data;

  TBreakPacket bp;
  THeaderCheckClientInClusterSS2M h;
  h.key_client = pPacket->key_client;
  bp.PushFront((char*)&h, sizeof(h));
  Context()->GetMS()->Send(GetID_SessionMasterSS(), bp);

  End();
}
//---------------------------------------------------------------------
void TScenarioLoginClient::CheckClientInClusterSS2M(TDescRecvSession* pDesc)// Master
{
  End();
}
//---------------------------------------------------------------------
void TScenarioLoginClient::CheckClientLoginM2S(TDescRecvSession* pDesc)// Slave
{
  End();
}
//---------------------------------------------------------------------
void TScenarioLoginClient::ClientInClusterS2C(TDescRecvSession* pDesc)// Client
{
  Context()->Reject();
  End();
}
//---------------------------------------------------------------------
void TScenarioLoginClient::NeedNumInQueue()
{
  mCallBackNeedNumInQueue.Notify();
}
//---------------------------------------------------------------------
