/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "Slave.h"

#include <boost/foreach.hpp>

#include "Logger.h"
#include "ControlScenario.h"
#include "ManagerSession.h"
#include "Events.h"

#include "ManagerContextClient_slave.h"
#include "ContainerContextSc.h"

#include "ScenarioDisconnectClient.h"
#include "ScenarioFlow.h"
#include "ScenarioLoginClient.h"
#include "ScenarioLoginSlave.h"
#include "ScenarioRecommutationClient.h"
#include "ScenarioSendToClient.h"
#include "ScenarioSynchroSlave.h"

using namespace nsMMOEngine;
using namespace std;

TSlave::TSlave():
mMngContextClient(new TManagerContextClient_slave)
{
  mTimeNeedSendSynchro = 0;
  mControlSc->mLoginClient->SetBehavior(TScenarioLoginClient::eSlave);
	mControlSc->mRcm->SetBehavior(TScenarioRecommutationClient::eSlave);
}
//-------------------------------------------------------------------------
TSlave::~TSlave()
{

}
//-------------------------------------------------------------------------
void TSlave::SaveContext(unsigned int id_session, void* data, int size)
{
  TContainerContextSc* pC = mMngContextClient->FindContextBySession(id_session);
  if(pC && pC->IsRcmActive()) // �������� �������� � ��������
  {
    mControlSc->mRcm->SetContext(&pC->mRcm);
    mControlSc->mRcm->SaveContext(data, size);// ��� ��� ����� ������� ��������
  }
}
//-------------------------------------------------------------------------
bool TSlave::FindClientKeyBySession(unsigned int id_session, unsigned int &id)
{
  return mMngContextClient->FindKeyBySession(id_session, id);
}
//-------------------------------------------------------------------------
bool TSlave::FindSessionByClientKey(unsigned int id, unsigned int& id_session)
{
  return mMngContextClient->FindSessionByKey(id, id_session);
}
//-------------------------------------------------------------------------
void TSlave::DisconnectInherit(unsigned int id_session)
{
  // ���� ����� ����� � ������� ����������� - ����� �� ����, ��������� ���������
  if(id_session==mID_SessionUp)
  {
    flgConnectUp = false;
    DisconnectAllClient();// ���������� ���� ��������
    mID_SessionUp = INVALID_HANDLE_SESSION;
    
    TEventDisconnectUp event;
    event.id_session = id_session;
    AddEventCopy(&event, sizeof(event));
    return;
  }
  // ���� ����� � ����� �� �������� - ���������� �������
  // ���������� ����� ��� �������� ��� �������
  unsigned int id_client;
  if(mMngContextClient->FindKeyBySession(id_session,id_client)==false)
    return;

  TContainerContextSc* pC = mMngContextClient->FindContextBySession(id_session);
  // ���� ���� �������� ��������, �� ��������� � ������ �������� �����������
  mControlSc->mDisClient->SetContext(&pC->mDisClient);
  pC->mDisClient.SetID_Session(mID_SessionUp);
  mControlSc->mDisClient->DisconnectFromSlave(id_client);

  mMngContextClient->DeleteByKey(id_client);
  
  TEventDisconnectDown event;
  event.id_session = id_session;
  AddEventCopy(&event, sizeof(event));
}
//-------------------------------------------------------------------------
int TSlave::GetCountDown()
{
	return mMngContextClient->GetCountSession();
}
//-------------------------------------------------------------------------
bool TSlave::GetDescDown(int index, void* pDesc, int& sizeDesc)
{
  if(sizeDesc<sizeof(TDescDownSlave))
  {
    GetLogger(STR_NAME_MMO_ENGINE)->
      WriteF_time("TSlave::GetDescDown() size of buffer less then size of structure.\n");
    return false;
  }

  unsigned int id_session;
  if(mMngContextClient->GetSessionByIndex(index, id_session)==false)
    return false;

  TDescDownSlave* pDescDownSlave = (TDescDownSlave*)pDesc;
  pDescDownSlave->id_session = id_session;
  sizeDesc = sizeof(TDescDownSlave);
	return true;
}
//-------------------------------------------------------------------------
void TSlave::ConnectUp(unsigned int ip, unsigned short port, unsigned char subNet)
{
  // ���� ������ ����, �� ������ ���� �����������, ���� �����������
  if(mID_SessionUp!=INVALID_HANDLE_SESSION)
    return;

  mControlSc->mLoginSlave->ConnectToMaster(ip, port, subNet);
  // ��� �� �������� �� ��������� ������
  mID_SessionUp = mContainerUp->mLoginSlave.GetID_Session();
}
//-------------------------------------------------------------------------
void TSlave::WorkInherit()
{
  // ���� ��� ����� � �������� - ������������� �� ���������
  if(flgConnectUp==false)
    return;
  //-------------------------------------------------------
  unsigned int now_ms = ht_GetMSCount();
  if(mTimeNeedSendSynchro<now_ms)
  {
    mControlSc->mSynchroSlave->SendSynchro(mLoadProcent);
    mTimeNeedSendSynchro = now_ms + eDeltaSynchro;
  }
}
//-------------------------------------------------------------------------
void TSlave::DisconnectAllClient()
{
  unsigned int id;
  // ����������� ���� �������� � ������ �� �����
  while(mMngContextClient->GetFirstKey(id))
  {
    // �� ����� ���� ������
    unsigned int id_session;
    if(mMngContextClient->FindSessionByKey(id, id_session))
    {
      // ��������� ������
      mManagerSession->CloseSession(id_session);
      // ������� �� �����
      mMngContextClient->DeleteByKey(id);
      // ��������� ������� � �����������
      TEventDisconnectDown event;
      event.id_session = id_session;
      AddEventCopy(&event, sizeof(event));
    }
  }
}
//-------------------------------------------------------------------------
void TSlave::SendDown(unsigned int id_session, TBreakPacket bp, bool check)
{
  TContainerContextSc* pC = mMngContextClient->FindContextBySession(id_session);
  if(pC)
  {
    mControlSc->mFlow->SetContext(&pC->mFlow);
    mControlSc->mFlow->SendDown(bp, check);
  }
}
//-------------------------------------------------------------------------
void TSlave::EndLoginClient(IScenario* pSc)
{
  TContextScLoginClient* pContext = (TContextScLoginClient*)pSc->GetContext();
  if(pContext->IsAccept())
  {
    // ��������� ������ �������
    TContainerContextSc* pC = mMngContextClient->FindContextByKey(pContext->GetIDClient());
    BL_ASSERT(pC);
    pC->SetID_Session(pContext->GetID_SessionClientSlave());
    return;
  }

  mMngContextClient->DeleteByKey(pContext->GetIDClient());
}
//-------------------------------------------------------------------------
void TSlave::EndLoginSlave(IScenario* pSc)
{
  // ����� �� ����� ���������, ������ ���� ����������
  mID_SessionUp = pSc->GetContext()->GetID_Session();
  mContainerUp->SetID_Session(mID_SessionUp);

  flgConnectUp = mContainerUp->mLoginSlave.IsConnect();
  if(flgConnectUp)
  {
    // ���� � ������� ��������
    TEventConnectUp event;
    event.id_session = mID_SessionUp;
    AddEventCopy(&event, sizeof(event));
  }
}
//-------------------------------------------------------------------------
void TSlave::EndRcm(IScenario* pSc)
{
  TContextScRecommutationClient* pContext = (TContextScRecommutationClient*)pSc->GetContext();
  unsigned int key = pContext->GetClientKey();
  if(pContext->IsDonor())
  {
    // ������ �������  
    mMngContextClient->DeleteByKey(key);
    return;
  }
  if(pContext->IsRecipient()==false)
  {
    GetLogger(STR_NAME_MMO_ENGINE)->
      WriteF_time("TSlave::EndRcm() Undef state.\n");
    BL_FIX_BUG();
    return;
  }
  // ����������� �� ���������� ��������� � ����������
  mMngContextClientSlaveRecipient->DeleteByKey(key);

  unsigned int id_session = pContext->GetID_SessionClientSlave();
  mMngContextClient->AddContextByKey(key);
  mMngContextClient->AddSessionByKey(key,id_session);
}
//-------------------------------------------------------------------------
void TSlave::NeedContextSendToClient(unsigned int id_client)
{
  // ������ �� �������� ������-�� �������
  TContainerContextSc* pContext = mMngContextClient->FindContextByKey(id_client);
	if(pContext)
		mControlSc->mSendToClient->SetContext(&pContext->mSendToClient);
	else
		mControlSc->mSendToClient->SetContext(NULL);
}
//-------------------------------------------------------------------------
void TSlave::SendByClientKey(list<unsigned int>& lKey, TBreakPacket bp)
{
	mControlSc->mSendToClient->SendFromSlave(lKey, bp);
}
//-------------------------------------------------------------------------
void TSlave::NeedContextLoginClientByClientSessionByKeyClient(unsigned int id_session_client,
                                                              unsigned int id_client)
{
  TContainerContextSc* pC = mMngContextClient->FindContextByKey(id_client);
  if(pC==NULL)
  {
    mControlSc->mLoginClient->SetContext(NULL);
    return;
  }

  mMngContextClient->AddSessionByKey(id_client, id_session_client);
  mControlSc->mLoginClient->SetContext(&pC->mLoginClient);
}
//-------------------------------------------------------------------------
void TSlave::NeedContextLoginClientByClientKey(unsigned int id_client)
{
  TContainerContextSc* pC = mMngContextClient->FindContextByKey(id_client);  
  if(pC==NULL)
    pC = mMngContextClient->AddContextByKey(id_client);

  mControlSc->mLoginClient->SetContext(&pC->mLoginClient);
}
//-------------------------------------------------------------------------
void TSlave::NeedContextByClientForSlaveKeyRcm(unsigned int key, bool donor)
{
  if(donor)
  {
    TContainerContextSc* pC = mMngContextClient->FindContextByKey(key);
    if(pC)
      mControlSc->mRcm->SetContext(&pC->mRcm);
    else
      mControlSc->mRcm->SetContext(NULL);
  }
  else
  {
    TContainerContextSc* pC = mMngContextClientSlaveRecipient->FindContextByKey(key);
    if(pC==NULL)
      pC = mMngContextClientSlaveRecipient->AddContextByKey(key);
    mControlSc->mRcm->SetContext(&pC->mRcm);
  }
}
//-------------------------------------------------------------------------
void TSlave::DisconnectClientRcm(unsigned int key)
{

}
//-------------------------------------------------------------------------

