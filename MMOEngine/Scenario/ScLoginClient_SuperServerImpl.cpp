/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ScLoginClient_SuperServerImpl.h"
#include "Logger.h"
#include "ContextScLoginClient.h"
#include "ManagerSession.h"
#include "Events.h"
#include "ErrorCode.h"

using namespace nsMMOEngine;
using namespace nsLoginClientStruct;

TScLoginClient_SuperServerImpl::TScLoginClient_SuperServerImpl(IScenario* pSc):
TBaseScLoginClient(pSc)
{

}
//-----------------------------------------------------------------------------
void TScLoginClient_SuperServerImpl::RecvInherit(TDescRecvSession* pDesc)
{
  THeader* pHeader = (THeader*)pDesc->data;
  switch(pHeader->from)
  {
    case eMaster:
      RecvFromMaster(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//-----------------------------------------------------------------------------
void TScLoginClient_SuperServerImpl::Work(unsigned int time_ms)
{
  if( Context()->GetTimeWait() + eTimeWait < time_ms)
  {
    // ошибка на той стороне
    TEventError event;
    event.code = LoginClientSuperServerNoAnswer;
    Context()->GetSE()->AddEventCopy(&event, sizeof(event));
    End();
  }
}
//-----------------------------------------------------------------------------
void TScLoginClient_SuperServerImpl::RecvFromMaster(TDescRecvSession* pDesc)
{
  THeader* pHeader = (THeader*)pDesc->data;
  switch(pHeader->subType)
  {
    case eRequestM2SS:
      RequestM2SS(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//--------------------------------------------------------------
void TScLoginClient_SuperServerImpl::SetIsExistClientID(bool isExist)
{
  // формирование пакета
  TBreakPacket bp;// контейнер для всего пакета
  THeaderCheckRequestSS2M h;
  h.isExistInSystem = isExist;
  bp.PushFront((char*)&h, sizeof(h));
  // отослать пакет для попытки авторизации
  unsigned int id_master = GetID_SessionMasterSS();
  Context()->GetMS()->Send(id_master, bp);

  End();
}
//--------------------------------------------------------------
void TScLoginClient_SuperServerImpl::NeedIsExistClient(unsigned int id_client)
{
  mCallBackNeedIsExistClient.Notify(id_client);
}
//--------------------------------------------------------------
void TScLoginClient_SuperServerImpl::RequestM2SS(TDescRecvSession* pDesc)
{
  if(Begin()==false)
  {
    // генерация ошибки
    GetLogger(STR_NAME_MMO_ENGINE)->
      WriteF_time("TScLoginClient_SuperServerImpl::RequestM2SS() scenario is not active.\n");
    BL_FIX_BUG();
    return;
  }
  THeaderRequestM2SS* pRequest = (THeaderRequestM2SS*)pDesc->data;
  NeedIsExistClient(pRequest->id_client);
}
//--------------------------------------------------------------
