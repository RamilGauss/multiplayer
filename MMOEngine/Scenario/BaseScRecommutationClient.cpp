/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "BaseScRecommutationClient.h"
#include "ContextScRecommutationClient.h"
#include "ScenarioRecommutationClient.h"

using namespace nsMMOEngine;
using namespace nsRecommutationClientStruct;

//--------------------------------------------------------------
TBaseScRecommutationClient::TBaseScRecommutationClient(IScenario* pSc)
{
  mScenario = pSc;
}
//--------------------------------------------------------------
TBaseScRecommutationClient::~TBaseScRecommutationClient()
{

}
//--------------------------------------------------------------
TContextScRecommutationClient* TBaseScRecommutationClient::Context()
{
  return (TContextScRecommutationClient*)mScenario->GetContext();
}
//--------------------------------------------------------------
unsigned int TBaseScRecommutationClient::GetID_SessionClientSlave()
{
  return Context()->GetID_SessionClientSlave();
}
//--------------------------------------------------------------
void TBaseScRecommutationClient::SetID_SessionClientSlave(unsigned int id)
{
  Context()->SetID_SessionClientSlave(id);
}
//--------------------------------------------------------------
unsigned int TBaseScRecommutationClient::GetID_SessionMasterSlave()
{
  return Context()->GetID_SessionMasterSlave();
}
//--------------------------------------------------------------
void TBaseScRecommutationClient::SetID_SessionMasterSlave(unsigned int id)
{
  Context()->SetID_SessionMasterSlave(id);
}
//--------------------------------------------------------------
void TBaseScRecommutationClient::Recv(TDescRecvSession* pDesc)
{
  RecvInherit(pDesc);
}
//---------------------------------------------------------------------
bool TBaseScRecommutationClient::Begin()
{
  return mScenario->Begin();
}
//---------------------------------------------------------------------
void TBaseScRecommutationClient::End()
{
  mScenario->End();
}
//---------------------------------------------------------------------
//void TBaseScRecommutationClient::NeedContextByClientKey(unsigned int id_client)
//{
//  mScenario->NeedContextByClientKey(id_client);
//}
////---------------------------------------------------------------------
//void TBaseScLoginClient::NeedNumInQueueByClientKey(unsigned int id_client)
//{
  //mScenario->Notify<unsigned int>(TScenarioLoginClient::eNumInQueueByClientKey,id_client);
//}
////---------------------------------------------------------------------
//void TBaseScLoginClient::NeedContextByMasterSessionByClientKey(unsigned int id_session_master,unsigned int id_client)
//{
//  mScenario->Notify<unsigned int,unsigned int>(TScenarioLoginClient::eContextByMasterSessionByClientKey,
//                                               id_session_master,
//                                               id_client);
//}
////---------------------------------------------------------------------
//void TBaseScLoginClient::EventSetClientKey(unsigned int id_client)
//{
//  mScenario->Notify<unsigned int>(TScenarioLoginClient::eSetClientKey,id_client);
//}
////---------------------------------------------------------------------
//void TBaseScLoginClient::NeedContext(unsigned int id_session)
//{
//  mScenario->NeedContext(id_session);
//}
////---------------------------------------------------------------------
//void TBaseScLoginClient::NeedContextByClientSessionByClientKey(unsigned int id_session_client,
//                                                               unsigned int id_client)
//{
//  mScenario->Notify<unsigned int,unsigned int>(TScenarioLoginClient::eContextByClientSessionByClientKey,
//                                               id_session_client, id_client);
//}
//---------------------------------------------------------------------
void TBaseScRecommutationClient::SetTimeWaitForNow()
{
  //Context()->SetTimeWait(ht_GetMSCount());
}
//--------------------------------------------------------------
