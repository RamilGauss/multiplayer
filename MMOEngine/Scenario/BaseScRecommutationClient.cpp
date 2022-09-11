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
void TBaseScRecommutationClient::SetTimeWaitForNow()
{
  Context()->SetTimeWait(ht_GetMSCount());
}
//--------------------------------------------------------------
void TBaseScRecommutationClient::NeedContextByClientKeyForSlave(unsigned int key, bool donor)
{
  mScenario->Notify<unsigned int,bool>(
    TScenarioRecommutationClient::eNeedContextByClientKeyForSlave,
    key,donor);
}
//--------------------------------------------------------------
void TBaseScRecommutationClient::NeedSessionDonorByClientKey(unsigned int key)
{
  mScenario->Notify<unsigned int>(
    TScenarioRecommutationClient::eNeedSessionDonor,
    key);
}
//--------------------------------------------------------------
void TBaseScRecommutationClient::EventActivate()
{
  mScenario->Notify<IScenario*>(
    TScenarioRecommutationClient::eEventActivate, mScenario);
}
//--------------------------------------------------------------
void TBaseScRecommutationClient::EventDisconnectClient(unsigned int key)
{
  mScenario->Notify<unsigned int>(
    TScenarioRecommutationClient::eEventDisconnectClient, key);
}
//--------------------------------------------------------------

