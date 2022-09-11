/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ScenarioRecommutationClient.h"
#include "ManagerSession.h"

#pragma warning(disable:4355)

using namespace nsMMOEngine;

TScenarioRecommutationClient::TScenarioRecommutationClient():
mClient(this),mSlave(this),mMaster(this)
{
  mCurBehavior = NULL; 

  //AddCallBack(eSetClientKey,                     &mCBSetClientKey);
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
void TScenarioRecommutationClient::Start(unsigned int id_session_recipient,
                                         unsigned int id_client)
{
  mMaster.Start(id_session_recipient, id_client);
}
//--------------------------------------------------------------
void TScenarioRecommutationClient::DelayBegin()
{
  //unsigned int new_id_session = mListNextID_SessionSlave.front();
  //mListNextID_SessionSlave.pop_front();
  //Context()->SetID_Session(new_id_session);
  //SendFirstPacket();
}
//--------------------------------------------------------------
void TScenarioRecommutationClient::SendFirstPacket()
{
  //mClient->SetState(TClient_master::eRcm);

  TBreakPacket bp;
  //THeaderSendFirstpacketRcmM h;
  //bp.PushBack((char*)&h, sizeof(h));
  //Context()->GetMS()->Send(Context()->GetID_Session(),bp);
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
void TScenarioRecommutationClient::SetBehavior(eBehavior v)
{
  switch(v)
  {
    case eClient:
      mCurBehavior = &mClient;
      break;
    case eSlave:
      mCurBehavior = &mSlave;
      break;
    case eMaster:
      mCurBehavior = &mMaster;
      break;
  }
}
//---------------------------------------------------------------------
