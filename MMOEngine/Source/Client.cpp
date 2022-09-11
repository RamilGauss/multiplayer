/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
√удаков –амиль —ергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "Client.h"
#include "ManagerSession.h"
#include "ControlScenario.h"
#include "IContextScenario.h"
#include "ContainerContextSc.h"
#include "ScenarioFlow.h"
#include "ScenarioLoginClient.h"
#include "ScenarioRecommutationClient.h"
#include "Events.h"

using namespace std;
using namespace nsMMOEngine;

TClient::TClient()
{
  mControlSc->mLoginClient->SetBehavior(TScenarioLoginClient::eClient);
}
//-------------------------------------------------------------------------
TClient::~TClient()
{

}
//-------------------------------------------------------------------------
void TClient::Login(unsigned int ip, unsigned short port, void* data, int size)
{
  mControlSc->mLoginClient->TryLogin(ip, port, data, size);
}
//-------------------------------------------------------------------------
void TClient::DisconnectInherit(unsigned int id_session)
{
  // тут проблема в том, что бы различить дисконнект со стороны ћастера при 
  // отработке сценари€ LoginClient от других
  IContextScenario* pContext = mContainerUp->GetMCSc()->GetActive();
  if(pContext)// есть вообще активные сценарии
  if(pContext==mControlSc->mLoginClient->GetContext())// активен сценарий LoginClient
    // возможно это продолжение сценари€
    mControlSc->mLoginClient->DisconnectFromClient();
	else
		mID_SessionUp = INVALID_HANDLE_SESSION;
}
//-------------------------------------------------------------------------
void TClient::EndLoginClient(IScenario* pSc)
{
  // вз€ть по этому контексту, задать всем контекстам
  mID_SessionUp = pSc->GetContext()->GetID_Session();
  mContainerUp->SetID_Session(mID_SessionUp);
}
//-------------------------------------------------------------------------
void TClient::EndRcm(IScenario* pSc)
{
  // вз€ть по этому контексту, задать всем контекстам
  mID_SessionUp = pSc->GetContext()->GetID_Session();
  mContainerUp->SetID_Session(mID_SessionUp);
}
//-------------------------------------------------------------------------
void TClient::LeaveQueue()
{

}
//-------------------------------------------------------------------------
void TClient::EventSetClientKeyLoginClient(unsigned int id_client)
{
  mID = id_client;
}
//-------------------------------------------------------------------------
