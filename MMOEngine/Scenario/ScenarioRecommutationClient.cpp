/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ScenarioRecommutationClient.h"
#include "ManagerSession.h"

using namespace nsMMOEngine;

TScenarioRecommutationClient::TScenarioRecommutationClient()
{

}
//--------------------------------------------------------------
TScenarioRecommutationClient::~TScenarioRecommutationClient()
{

}
//--------------------------------------------------------------
void TScenarioRecommutationClient::Work()
{

}
//--------------------------------------------------------------
void TScenarioRecommutationClient::Start(unsigned int new_id_session, TClient_master* pClient)
{
  Context()->SetClient(pClient);

  if(Begin()==false)
  {
    mListNextID_SessionSlave.push_back(new_id_session);
    return;
  }
  Context()->SetID_Session(new_id_session);
  SendFirstPacket();
}
//--------------------------------------------------------------
void TScenarioRecommutationClient::DelayBegin()
{
  unsigned int new_id_session = mListNextID_SessionSlave.front();
  mListNextID_SessionSlave.pop_front();
  Context()->SetID_Session(new_id_session);
  SendFirstPacket();
}
//--------------------------------------------------------------
void TScenarioRecommutationClient::SendFirstPacket()
{
  //mClient->SetState(TClient_master::eRcm);

  TBreakPacket bp;
  //THeaderSendFirstpacketRcmM h;
  //bp.PushBack((char*)&h, sizeof(h));
  Context()->GetMS()->Send(Context()->GetID_Session(),bp);
}
//--------------------------------------------------------------
void TScenarioRecommutationClient::SaveContext(void* data, int size)
{

}
//---------------------------------------------------------------------
void TScenarioRecommutationClient::Recv(TDescRecvSession* pDesc)
{

}
//---------------------------------------------------------------------
