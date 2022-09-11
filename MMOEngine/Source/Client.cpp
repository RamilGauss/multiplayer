/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
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
  // ��� �������� � ���, ��� �� ��������� ���������� �� ������� ������� ��� 
  // ��������� �������� LoginClient �� ������
  IContextScenario* pContext = mContainerUp->GetMCSc()->GetActive();
  if(pContext)// ���� ������ �������� ��������
  if(pContext==mControlSc->mLoginClient->GetContext())// ������� �������� LoginClient
    // �������� ��� ����������� ��������
    mControlSc->mLoginClient->DisconnectFromClient();
	else
		mID_SessionUp = INVALID_HANDLE_SESSION;
}
//-------------------------------------------------------------------------
void TClient::EndLoginClient(IScenario* pSc)
{
  // ����� �� ����� ���������, ������ ���� ����������
  mID_SessionUp = pSc->GetContext()->GetID_Session();
  mContainerUp->SetID_Session(mID_SessionUp);
}
//-------------------------------------------------------------------------
void TClient::EndRcm(IScenario* pSc)
{
  // ����� �� ����� ���������, ������ ���� ����������
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
