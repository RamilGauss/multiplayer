/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
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

using namespace nsMelissa;
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
  if(Context()->GetTimeWait()<time_ms)
  {
    // ошибка на той стороне
    TEventError event;
    event.code = LoginClientNoAnswer;
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
    // генерация ошибки
    GetLogger(STR_NAME_MELISSA)->
      WriteF_time("TScenarioLoginClient::TryLogin() scenario is not active.\n");
    BL_FIX_BUG();
    return;
  }
  // формирование пакета
  TBreakPacket bp;// контейнер для всего пакета
  bp.PushFront((char*)data,size);
  THeaderTryLoginC2M h;
  h.sizeData = size;
  bp.PushFront((char*)&h, sizeof(h));
  // отослать пакет для попытки авторизации
  SetID_SessionClientMaster(Context()->GetMS()->Send(ip, port, bp));
  if(GetID_SessionClientMaster()==INVALID_HANDLE_SESSION)
  {
    // Генерация ошибки
    TEventError event;
    event.code = LoginClient_MasterNotReady;
    Context()->GetSE()->AddEventCopy(&event, sizeof(event));
    End();
  }
  else
    SetTimeWaitForNow();
}
//-----------------------------------------------------------------------------
void TScLoginClient_ClientImpl::SetTimeWaitForNow()
{
  Context()->SetTimeWait(ht_GetMSCount() + eTimeWait);
}
//--------------------------------------------------------------
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
  // обновить время таймера
  SetTimeWaitForNow();
  //==============================
  THeaderResultLoginM2C* pH = (THeaderResultLoginM2C*)pDesc->data;
  switch(pH->result)
  {
    case THeaderResultLoginM2C::eAccept:
      // все зашибись, идем дальше
      break;
    case THeaderResultLoginM2C::eReject:
    {
      TEventResultLogin event;
      event.res = TMaster::eReject;
      Context()->GetSE()->AddEventCopy(&event, sizeof(event));
      End();
    }
      break;
    case THeaderResultLoginM2C::eQueue:
    {
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
  TEventLeaveQueue event;
  Context()->GetSE()->AddEventCopy(&event, sizeof(event));

  End();
}
//--------------------------------------------------------------
void TScLoginClient_ClientImpl::InfoSlaveM2C(TDescRecvSession* pDesc)
{

}
//--------------------------------------------------------------
