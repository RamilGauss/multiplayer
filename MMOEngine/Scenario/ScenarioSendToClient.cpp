/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ScenarioSendToClient.h"
#include "ManagerSession.h"

using namespace nsMMOEngine;

TScenarioSendToClient::TScenarioSendToClient()
{

}
//-------------------------------------------------------------------
TScenarioSendToClient::~TScenarioSendToClient()
{

}
//-------------------------------------------------------------------
void TScenarioSendToClient::Recv(TDescRecvSession* pDesc)
{
  NeedContext(pDesc->id_session);

  THeaderSendToClient* pPacket = (THeaderSendToClient*)pDesc->data;
  switch(pPacket->subType)
  {
  case eFromSuperServerToMaster:
    RecvFromSuperServerToMaster(pDesc);
    break;
  }
}
//-------------------------------------------------------------------
void TScenarioSendToClient::SendFromSuperServerToMaster(unsigned int id_client, TBreakPacket bp)
{
  THeaderFromSuperServerToMaster h;
  h.id_client = id_client;
  bp.PushFront((char*)&h, sizeof(h));

  unsigned int id_session = Context()->GetID_Session();
  Context()->GetMS()->Send(id_session, bp);
}
//-------------------------------------------------------------------
void TScenarioSendToClient::RecvFromSuperServerToMaster(TDescRecvSession* pDesc)
{
  //THeaderFromMasterToSlave h;
  //h.id_client = id_client;
  //bp.PushFront((char*)&h, sizeof(h));

  //unsigned int id_session = Context()->GetID_Session();
  //Context()->GetMS()->Send(id_session, bp);
}
//-------------------------------------------------------------------
