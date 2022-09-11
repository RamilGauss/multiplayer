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

using namespace nsMelissa;
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

}
//-----------------------------------------------------------------------------
void TScLoginClient_SuperServerImpl::RecvFromMaster(TDescRecvSession* pDesc)
{
  THeader* pHeader = (THeader*)pDesc->data;
  switch(pHeader->subType)
  {
    case eRequestM2SS:
    {
      if(Begin()==false)
      {
        // генерация ошибки
        GetLogger(STR_NAME_MELISSA)->
          WriteF_time("TScLoginClient_SuperServerImpl::RecvFromMaster() scenario is not active.\n");
        BL_FIX_BUG();
        return;
      }
    }
      break;
    default:BL_FIX_BUG();
  }
}
//--------------------------------------------------------------
void TScLoginClient_SuperServerImpl::SetResultLoginClient(bool isExist)
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

