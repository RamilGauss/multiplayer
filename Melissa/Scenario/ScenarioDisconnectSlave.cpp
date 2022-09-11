/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ScenarioDisconnectSlave.h"

using namespace nsMelissa;
using namespace std;

TScenarioDisconnectSlave::TScenarioDisconnectSlave()
{

}
//---------------------------------------------------------------
void TScenarioDisconnectSlave::Recv(TDescRecvSession* pDesc)
{
  NeedContext(pDesc->id_session);
  THeaderFromMasterToSuperServer* pPacket = (THeaderFromMasterToSuperServer*)pDesc->data;
  switch(pPacket->subType)
  {
    case eFromMasterToSuperServer:
      RecvFromMaster();
      break;
    default:BL_FIX_BUG();
  }
}
//---------------------------------------------------------------
void TScenarioDisconnectSlave::SendFromMasterToSuperServer(vector<unsigned int>& vID_client)
{

}
//---------------------------------------------------------------
void TScenarioDisconnectSlave::RecvFromMaster()
{
  //if(Begin()==false)
  //{
  //  End();
  //  // верхнее соединение занято выполнением другого сценария - такого не должно быть
  //  // внутренняя ошибка
  //  GetLogger(STR_NAME_MELISSA)->
  //    WriteF_time("TScenarioLoginMaster::RecvFromMaster() scenario is not active.\n");
  //  BL_FIX_BUG();
  //  return;
  //}
  //TBreakPacket bp;
  //THeaderAnswerFromSS h;
  //bp.PushFront((char*)&h, sizeof(h));
  //Context()->GetMS()->Send(Context()->GetID_Session(), bp);
  //End();
}
//-------------------------------------------------------------------------------------
