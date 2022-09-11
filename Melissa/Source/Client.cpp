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
using namespace nsMelissa;

TClient::TClient()
{

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

}
//-------------------------------------------------------------------------
void TClient::EndLoginClient(IScenario* pSc)
{
  // вз€ть по этому контексту, задать всем контекстам
  mID_SessionUp = pSc->GetContext()->GetID_Session();
  mContainerUp->SetID_Session(mID_SessionUp);

	TEventResultLogin event;
  event.res = mContainerUp->mLoginClient.IsReject()?
                                  TMaster::eReject: 
                                  TMaster::eAccept;
	AddEventCopy(&event, sizeof(event));
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
