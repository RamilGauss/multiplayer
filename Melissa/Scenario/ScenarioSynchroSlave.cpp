/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ScenarioSynchroSlave.h"
#include "ManagerSession.h"

using namespace nsMelissa;

TScenarioSynchroSlave::TScenarioSynchroSlave()
{

}
//---------------------------------------------------------------------
TScenarioSynchroSlave::~TScenarioSynchroSlave()
{

}
//---------------------------------------------------------------------
void TScenarioSynchroSlave::SendSynchro(int loadProcent)
{
  TBreakPacket bp;
  THeaderSynchroSlave h;
  h.loadProcent = loadProcent;
  bp.PushFront((char*)&h, sizeof(h));
  Context()->GetMS()->Send(Context()->GetID_Session(), bp);
}
//---------------------------------------------------------------------
void TScenarioSynchroSlave::Recv(TDescRecvSession* pDesc)
{
  NeedContext(pDesc->id_session);
  THeaderSynchroSlave* pPacket = (THeaderSynchroSlave*)pDesc->data;
  switch(pPacket->subType)
  {
    case eFromSlave:
      RecvFromSlave(pPacket);
      break;
    default:BL_FIX_BUG();
  }
}
//---------------------------------------------------------------------
void TScenarioSynchroSlave::RecvFromSlave(THeaderSynchroSlave* pPacket)
{
  bool res = Begin();
  BL_ASSERT(res);
  Context()->SetLoadProcent(pPacket->loadProcent);
  End();
}
//--------------------------------------------------------------------------
