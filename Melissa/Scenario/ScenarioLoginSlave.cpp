/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
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

using namespace nsMelissa;

TScenarioLoginSlave::TScenarioLoginSlave()
{

}
//-------------------------------------------------------------------------------------
void TScenarioLoginSlave::ConnectToMaster( unsigned int ip, unsigned short port )
{
  Context()->SetConnect(false);
  if(Begin()==false)
  {
    // верхнее соединение занято выполнением другого сценария - такого не должно быть
    // генерация ошибки
    GetLogger(STR_NAME_MELISSA)->
      WriteF_time("TScenarioLoginSlave::ConnectToMaster() scenario is not active.\n");
    BL_FIX_BUG();
    return;
  }
  // закрыть соединение
  Context()->GetMS()->CloseSession(Context()->GetID_Session());
  TBreakPacket bp;
  THeaderFromSlave h;
  bp.PushFront((char*)&h, sizeof(h));
  Context()->SetID_Session( Context()->GetMS()->Send(ip, port, bp) );
  if(Context()->GetID_Session()==INVALID_HANDLE_SESSION)
  {
    // Генерация ошибки
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
    // Генерация ошибки
    TEventError event;
    event.code = LoginSlave_NoAnswerFromMaster;
    Context()->GetSE()->AddEventCopy(&event, sizeof(event));
    End();
  }
}
//-------------------------------------------------------------------------------------
void TScenarioLoginSlave::RecvFromMaster()
{
  Context()->SetConnect(true);
  End();
}
//-------------------------------------------------------------------------------------
void TScenarioLoginSlave::RecvFromSlave()
{
  TBreakPacket bp;
  THeaderAnswerMaster h;
  bp.PushFront((char*)&h, sizeof(h));
  Context()->GetMS()->Send(Context()->GetID_Session(), bp);
}
//--------------------------------------------------------------------------
void TScenarioLoginSlave::Recv(TDescRecvSession* pDesc)
{
  NeedContext(pDesc->id_session);
  THeaderLoginSlave* pPacket = (THeaderLoginSlave*)pDesc->data;
  switch(pPacket->subType)
  {
    case eFromSlave:
      RecvFromSlave();
      break;
    case eAnswerMaster:
      RecvFromMaster();
      break;
    default:BL_FIX_BUG();
  }
}
//--------------------------------------------------------------------------  
