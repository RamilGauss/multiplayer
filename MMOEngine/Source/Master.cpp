/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "Master.h"
#include "ControlScenario.h"
#include "BL_Debug.h"
#include "Logger.h"

#include "ManagerContextMoreDownClientConnection.h"
#include "ManagerContextDownConnection_Slave.h"
#include "ManagerContextClientLogining.h"
#include "ManagerGroupClient.h"
#include "ManagerRecommutation.h"
#include "SetOrderElement.h"
#include "StatisticaClientInGroup.h"

#include "Events.h"

#include "ScenarioLoginClient.h"
#include "ScenarioLoginSlave.h"
#include "ScenarioLoginMaster.h"
#include "ScenarioFlow.h"
#include "ScenarioRecommutationClient.h"
#include "ScenarioSynchroSlave.h"
#include "ScenarioDisconnectClient.h"
#include "ScenarioSendToClient.h"

#include "ContainerContextSc.h"
#include "ErrorCode.h"

using namespace std;
using namespace nsMMOEngine;

TMaster::TMaster():
mMngContextClient(new TManagerContextMoreDownClientConnection),
mMngContextSlave (new TManagerContextDownConnection_Slave),
mMngContextClientLogining(new TManagerContextClientLogining),
mMngGroup(new TManagerGroupClient),
mSetClientKeyInQueue(new TSetOrderElement),
mStatisticaClientInGroup(new TStatisticaClientInGroup),
mMngRcm(new TManagerRecommutation)
{
  mControlSc->mLoginClient->SetBehavior(TScenarioLoginClient::eMaster);
	mControlSc->mRcm->SetBehavior(TScenarioRecommutationClient::eMaster);
}
//-------------------------------------------------------------------------
TMaster::~TMaster()
{
  Done();
}
//-------------------------------------------------------------------------
bool TMaster::TryCreateGroup(list<unsigned int>& l_id_client, unsigned int& id_group)
{
  BOOST_FOREACH(unsigned int id_client, l_id_client)
  {
    // ������ ����� � �� ������������ � ������� 
    // �������� ������� �� ������ � ������
    if(mMngGroup->FindIDByClientKey(id_client,id_group))
    {
      TEventError event;
      event.code = CreateGroup_ClientNotExist;
      AddEventCopy(&event, sizeof(event));
      return false;
    }
  }
  // ������ �������� �� Slave
  return EvalCreateGroupNow(l_id_client,id_group);
}
//-------------------------------------------------------------------------
void TMaster::DestroyGroup(unsigned int id_group)
{
  unsigned int id_session_slave;
  if(mMngGroup->FindSessionByID(id_group, id_session_slave)==false)
    return;
  mStatisticaClientInGroup->DeleteBySlaveSession(id_session_slave);
  //----------------------------------------------------------------
  mMngGroup->DeleteByID(id_group);
}
//-------------------------------------------------------------------------
void TMaster::LeaveGroup(unsigned int id_client)
{
  unsigned int id_group;
  if(mMngGroup->FindIDByClientKey(id_client,id_group)==false)
    return;

  unsigned int id_session_slave;
  if(mMngGroup->FindSessionByID(id_group, id_session_slave))
    mStatisticaClientInGroup->DeleteByClientKey(id_session_slave,id_client);

  mMngGroup->DeleteClientKey(id_client);
}
//-------------------------------------------------------------------------
void TMaster::GetListForGroup(unsigned int id_group, std::list<unsigned int>& lClientKey)
{
  int countClient = mMngGroup->GetCountClientKey(id_group);

  for( int i = 0 ; i < countClient ; i++)
  {
    unsigned int id_client;
    mMngGroup->GetClientKeyByIndex(id_group, i, id_client);
    lClientKey.push_back(id_client);
  }
}
//-------------------------------------------------------------------------
void TMaster::SetResultLogin(bool res, unsigned int id_session_client, 
                             unsigned int id_client, 
                             void* resForClient, int sizeResClient)
{
  TContainerContextSc* pC = mMngContextClientLogining->FindContextBySession(id_session_client);
  if(pC==NULL)
    return;
  mControlSc->mLoginClient->SetContext(&pC->mLoginClient);
  if(res==false)
  {
		// �����
    mControlSc->mLoginClient->Reject(resForClient, sizeResClient);
    return;
  }
  // �������� �� ������������� ����� � ��������
  unsigned int id_session_temp;
  if((mMngContextClient->FindContextByClientKey(id_client))||
     (mMngContextClientLogining->FindSessionByClientKey(id_client,id_session_temp)))
  {
    TEventError event;
    event.code = LoginClientMaster_KeyBusy;
    AddEventCopy(&event,sizeof(event)); 
    return;
  }
  // ������ ������ � ����� �������
  mMngContextClientLogining->AddKeyBySession(id_session_client, id_client);
  // ������ �� ����� Slave �������� �������
  // ��� ������ ����� Slave, ������ �� ���� � ������������, �� �������� �� ����������
  // ������ �� �� ����������, �����, �� ����� �� �����������, ������ ��� ������ �� �� ���� � ������ ��������
  // � ��� ������������, ������ ������ � ����

  // ������ � ������?
  bool resAdd = false;
  unsigned int id_session_slave;
  unsigned int id_group;
  if(mMngGroup->FindIDByClientKey(id_client, id_group))
    resAdd = TryAddClientByGroup(id_client, id_group, id_session_slave);
  else
    resAdd = TryAddClient(id_client, id_session_slave);
  if(resAdd)
    AddClientBySlaveSession(id_client,id_session_slave,resForClient,sizeResClient);
  else
    AddInQueue(id_client, resForClient, sizeResClient);
}
//-------------------------------------------------------------------------
bool TMaster::FindSlaveSessionByGroup(unsigned int id_group, 
                                      unsigned int& id_session)
{
  return mMngGroup->FindSessionByID(id_group,id_session);
}
//-------------------------------------------------------------------------
void TMaster::DisconnectInherit(unsigned int id_session)
{
  if(DisconnectSuperServer(id_session))
    return;
  if(DisconnectClientWait(id_session))
    return;
  if(DisconnectSlave(id_session))
    return;
  //GetLogger(STR_NAME_MMO_ENGINE)->
  //  WriteF_time("TMaster::DisconnectInherit() session not exist id=%u.\n",id_session);
  //BL_FIX_BUG();
}
//-------------------------------------------------------------------------
int TMaster::GetCountDown()
{
	return mMngContextSlave->GetCountSession();
}
//-------------------------------------------------------------------------
bool TMaster::GetDescDown(int index, void* pDesc, int& sizeDesc)
{
  if(sizeDesc<sizeof(TDescDownMaster))
  {
    GetLogger(STR_NAME_MMO_ENGINE)->
      WriteF_time("TMaster::GetDescDown() size of buffer less then size of structure.\n");
    return false;
  }
  unsigned int id_session;
  if(mMngContextSlave->GetSessionByIndex(index, id_session)==false)
    return false;

  TDescDownMaster* pDescDownSlave = (TDescDownMaster*)pDesc;
  pDescDownSlave->id_session = id_session;
  sizeDesc = sizeof(TDescDownMaster);
  return true;
}
//-------------------------------------------------------------------------
void TMaster::ConnectUp(unsigned int ip, unsigned short port, unsigned char subNet)
{
  mControlSc->mLoginMaster->ConnectToSuperServer(ip, port, subNet);
  // ��� �� �������� �� ��������� ������
  mID_SessionUp = mControlSc->mLoginMaster->GetContext()->GetID_Session();
}
//-------------------------------------------------------------------------
void TMaster::EndLoginMaster(IScenario* pSc)
{
  // ����� �� ����� ���������, ������ ���� ����������
  mID_SessionUp = pSc->GetContext()->GetID_Session();
  mContainerUp->SetID_Session(mID_SessionUp);

  flgConnectUp = mContainerUp->mLoginMaster.IsConnect();
  if(flgConnectUp)
  {
    // ���� � ������� ��������
    TEventConnectUp event;
    event.id_session = mID_SessionUp;
    AddEventCopy(&event, sizeof(event));
  }
}
//-------------------------------------------------------------------------
void TMaster::EndLoginSlave(IScenario* pSc)
{
  
}
//-------------------------------------------------------------------------
void TMaster::SendDown(unsigned int id_session, TBreakPacket bp, bool check)
{
  TContainerContextSc* pC = mMngContextSlave->FindContextBySession(id_session);
  if(pC==NULL)
    return;
  mControlSc->mFlow->SetContext(&pC->mFlow);
  mControlSc->mFlow->SendDown(bp,check);
}
//-------------------------------------------------------------------------
void TMaster::WorkInherit()
{
}
//-------------------------------------------------------------------------
void TMaster::EndLoginClient(IScenario* pSc)
{
  // �������� ��������
  TContextScLoginClient* pContextLoginClient = (TContextScLoginClient*)pSc->GetContext();
  // ����������� ���������� �� mMngContextClientLogining � mMngContextClient
  // ���� �� ������ 
  unsigned int id_session_client = pContextLoginClient->GetID_SessionClientMaster();
  TContainerContextSc* pContainer = 
    mMngContextClientLogining->FindContextBySession(id_session_client);
  // ������� �� ������ ��������������
  mMngContextClientLogining->DeleteBySession(id_session_client);
  if(pContextLoginClient->IsReject())
  {
    if(pContextLoginClient->IsTimeWaitElapsed())
    {
      if(pContainer)
      {
        vector<unsigned int> vecID_Client;
        vecID_Client.push_back(pContextLoginClient->GetIDClient());
        mControlSc->mDisClient->SetContext(&pContainer->mDisClient);
        pContainer->mDisClient.SetID_Session(mID_SessionUp);
        mControlSc->mDisClient->DisconnectFromMaster(vecID_Client);
      }
    }
    return;
  }
  // ��� ���� ����������� ����������� ������
  if(pContextLoginClient->IsAccept())
  {
    // ID �������
    unsigned int id_client = pContextLoginClient->GetIDClient();
    // ����� ������ Slave
    unsigned int id_session_slave = pContextLoginClient->GetID_SessionMasterSlave();
    mMngContextClient->AddContext(id_client,id_session_slave);

    mStatisticaClientInGroup->AddBySlaveSessionClientKey(id_session_slave,id_client);
  }
}
//-------------------------------------------------------------------------
void TMaster::Done()
{
  
}
//-------------------------------------------------------------------------
bool TMaster::DisconnectSuperServer(unsigned int id_session)
{
  if(id_session!=mID_SessionUp)
    return false;

  mID_SessionUp = INVALID_HANDLE_SESSION;
  flgConnectUp  = false;

  TEventDisconnectUp event;
  event.id_session = mID_SessionUp;
  AddEventCopy(&event, sizeof(event));
  return true;
}
//-------------------------------------------------------------------------
bool TMaster::DisconnectClientWait(unsigned int id_session)
{
  TContainerContextSc* pC = mMngContextClientLogining->FindContextBySession(id_session);
  if(pC==NULL)
    return false;
  
  unsigned int id_client;
  if(mMngContextClientLogining->FindClientKeyBySession(id_session,id_client))
  {
		// �������� ������ � �������, ������ �� ������ ������
		mSetClientKeyInQueue->DeleteKey(id_client);
    // ��������� � �������� �� Slave
    mControlSc->mLoginClient->SetContext(&pC->mLoginClient);
    mControlSc->mLoginClient->DisconnectFromMaster();
    // �������� SuperServer ���������� � ����������� �������
    mControlSc->mDisClient->SetContext(&pC->mDisClient);
    vector<unsigned int> vecID;
    vecID.push_back(id_client);
    mControlSc->mDisClient->DisconnectFromMaster(vecID);
  }

  mMngContextClientLogining->DeleteBySession(id_session);
  return true;
}
//-------------------------------------------------------------------------
bool TMaster::DisconnectSlave(unsigned int id_session)
{
  if(mMngContextSlave->FindContextBySession(id_session)==NULL)
    return false;
  // ��������� ������ ��������, ������� ������ �� Slave
  int countClient;
  if(mMngContextSlave->GetCountClientKey(id_session, countClient)==false)
    return false;

  vector<unsigned int> vID_client;  
  for(int i = 0 ; i < countClient ; i++)
  {
    unsigned int id_client;
    if(mMngContextSlave->GetClientKeyByIndex(id_session, i, id_client))
      vID_client.push_back(id_client);
  }
  // �������� SuperServer ������ ��������
  if((mID_SessionUp!=INVALID_HANDLE_SESSION)&& // ���� ���� ����� � SuperServer
     (vID_client.size()                    ))
  {
    // ������ �����-�� ��������
    TContainerContextSc* pC = mMngContextClient->FindContextByClientKey(vID_client[0]);
    if(pC)
    {
      mControlSc->mDisClient->SetContext(&pC->mDisClient);
      pC->mDisClient.SetID_Session(mID_SessionUp);
      mControlSc->mDisClient->DisconnectFromMaster(vID_client);
    }
  }
  BOOST_FOREACH(unsigned int id_client, vID_client)
    mMngContextClient->DeleteByKey(id_client);
  // !!! �� ����� ������ ������� id_client, ������� ����� ������ � id_session
  int countGroup = mMngGroup->GetCountID();
  for( int i = 0 ; i < countGroup ; i++ )
  {
    // ���������� ��� ������ � ���� � id_session
    unsigned int id_group;
    if(mMngGroup->GetIDByIndex(i, id_group))
    {
      unsigned int id_session_in_group;
      if(mMngGroup->FindSessionByID(id_group, id_session_in_group))
        if(id_session_in_group==id_session)
        {
          mMngGroup->DeleteByID(id_group);
          // 
          TEventDestroyGroup eventDestroyGroup;
          eventDestroyGroup.id_group = id_group;
          AddEventCopy(&eventDestroyGroup, sizeof(eventDestroyGroup));
        }
    }
  }
  // ������� Slave
  mMngContextSlave->DeleteContextBySession(id_session);
  mStatisticaClientInGroup->DeleteBySlaveSession(id_session);
  // ��������� � �������
  TEventDisconnectDown event;
  event.id_session = id_session;
  AddEventCopy(&event, sizeof(event));
  return true;
}
//-------------------------------------------------------------------------
bool TMaster::EvalCreateGroupNow(list<unsigned int>& l_id_client, unsigned int& id_group)
{
  // ���� ������� Slave, ������� �������� ������� ��������, ��������� � ������
  unsigned int id_session_slave;
  if(mStatisticaClientInGroup->FindSlaveSessionByMinimumClient(id_session_slave)==false)
    return false;
  // ���������� ��������, ������� ����� � ������� �� Slave, ���� ����� ������ ����������� ��������, �� ����� �������
  if(LoadInFutureLessLimit(id_session_slave, l_id_client)==false)
    return false;
  // ������� ������
  id_group = mMngGroup->AddGroup( id_session_slave);

  // ������� � ����������
  mStatisticaClientInGroup->AddSlave(id_session_slave);
  // ��� �� �������, ������� ������� � ����� ������ ������, ��������� � ����������
  BOOST_FOREACH(unsigned int id_client, l_id_client)
  {
    // ���� ������������ � �������
    if(mMngContextClient->FindContextByClientKey(id_client))
      mStatisticaClientInGroup->AddBySlaveSessionClientKey(id_session_slave,id_client);
    // ��������� ��������� ���������� �� ���� ���������� �� ��� � �������
    mMngGroup->AddClientKey(id_group, id_client);
  }
  //================================================================================
  // ���������� ��� ������� �� ����� ����� ������� �� ������ Slave
  // � ������ ������������� ����� ������� �� ����� Slave
  BOOST_FOREACH(unsigned int id_client, l_id_client)
  {
    TContainerContextSc* pC = mMngContextClient->FindContextByClientKey(id_client);
    if(pC)
      SolveExchangeClient(id_client, pC, id_session_slave);
  }
  return true;
}
//-------------------------------------------------------------------------
void TMaster::NeedContextLoginSlave(unsigned int id_session)
{
  TContainerContextSc* pC = mMngContextSlave->FindContextBySession(id_session);
  if(pC)
  {
    // ���������� ������
    GetLogger(STR_NAME_MMO_ENGINE)->
      WriteF_time("TMaster::NeedContextLoginSlave() against try authorized.\n");
    return;
  }
  pC = mMngContextSlave->AddContext(id_session);
  mControlSc->mLoginSlave->SetContext(&pC->mLoginSlave);

  mStatisticaClientInGroup->AddSlave(id_session);
  // ������� ������
  TEventConnectDown event;
  event.id_session = id_session;
  AddEventCopy(&event, sizeof(event));
}
//-------------------------------------------------------------------------
void TMaster::NeedContextSynchroSlave(unsigned int id_session)
{
	TContainerContextSc* pC = mMngContextSlave->FindContextBySession(id_session);
  if(pC==NULL)
    return;
  mControlSc->mSynchroSlave->SetContext(&pC->mSynchroSlave);
}
//-------------------------------------------------------------------------
void TMaster::EndSynchroSlave(IScenario* pSc)
{
  TContextScSynchroSlave* pContextSynchroSlave = (TContextScSynchroSlave*)pSc->GetContext();
	mMngContextSlave->SetLoadBySession(pContextSynchroSlave->GetID_Session(), 
		                                 pContextSynchroSlave->GetLoadProcent());
}
//-------------------------------------------------------------------------
void TMaster::SendByClientKey(list<unsigned int>& lKey, TBreakPacket bp)
{
	mControlSc->mSendToClient->SendFromSlave(lKey, bp);
}
//-------------------------------------------------------------------------
void TMaster::NeedContextLoginClientBySession(unsigned int id_session)
{
  // ���� ��� ��������� �����������, �� Begin �� ������� ��������� �������
  // ������ false � � �������� ��������� ������ �� �����������
	TContainerContextSc* pC = mMngContextClientLogining->FindContextBySession(id_session);
  if(pC)
  {
    // ���������� ������
    GetLogger(STR_NAME_MMO_ENGINE)->
      WriteF_time("TMaster::NeedContextLoginClientBySession() against try authorized.\n");
  }
  else
  {
		pC = mMngContextClientLogining->AddContext(id_session);
  }
  // ��������� ��������
  mControlSc->mLoginClient->SetContext(&pC->mLoginClient);
}
//-------------------------------------------------------------------------
void TMaster::NeedContextLoginClientByClientKey(unsigned int id_key_client)
{
  unsigned int id_session_client;
  if(mMngContextClientLogining->FindSessionByClientKey(id_key_client,id_session_client))
  {
    TContainerContextSc* pC = mMngContextClientLogining->FindContextBySession(id_session_client);
    if(pC)
      mControlSc->mLoginClient->SetContext(&pC->mLoginClient);
    else
    {
      mControlSc->mLoginClient->SetContext(NULL);
      BL_FIX_BUG();
    }
  }
  else
  {
    mControlSc->mLoginClient->SetContext(NULL);
    //BL_FIX_BUG();
  }
}
//-------------------------------------------------------------------------
void TMaster::NeedNumInQueueLoginClient(unsigned int id_session)
{
  TContainerContextSc* pC = mMngContextClientLogining->FindContextBySession(id_session);
  if(pC==NULL)
  {
    BL_FIX_BUG();
    return;
  }
  unsigned int id_client;
  if(mMngContextClientLogining->FindClientKeyBySession(id_session, id_client)==false)
  {
    BL_FIX_BUG();
    return;
  }
  int index;
  if(mSetClientKeyInQueue->FindIndexByClientKey(id_client,index)==false)
  {
    BL_FIX_BUG();
    return;
  }
  pC->mLoginClient.SetNumInQueue(index + 1);
}
//-------------------------------------------------------------------------
void TMaster::NeedContextDisconnectClient(unsigned int id_client)
{
  bool client_is_logining = false;
  unsigned int id_session_client;
  // ��������� ���� �� ������ ����� ������
  TContainerContextSc* pC = mMngContextClient->FindContextByClientKey(id_client);
  if(pC==NULL)
  {
    if(mMngContextClientLogining->FindSessionByClientKey(id_client,id_session_client)==false)
      return;
    TContainerContextSc* pC = mMngContextClientLogining->FindContextBySession(id_session_client);
    if(pC==NULL)
      return;
    client_is_logining = true;
  }
  // ��������� ������ SuperServer
  if(mID_SessionUp!=INVALID_HANDLE_SESSION)
  {
    vector<unsigned int> vecID;
    vecID.push_back(id_client);
    mControlSc->mDisClient->SetContext(&pC->mDisClient);
    pC->mDisClient.SetID_Session(mID_SessionUp);
    mControlSc->mDisClient->DisconnectFromMaster(vecID);
  }
  // ������� �� ��������
  if(client_is_logining)
    mMngContextClientLogining->DeleteBySession(id_session_client);
  else
  {
    mMngContextClient->DeleteByKey(id_client);

    unsigned int id_session_slave;
    if(mMngContextClient->FindSessionByClientKey(id_client, id_session_slave))
    {
      // ������� ���������� �� ������ ������(!), �.�. ������� ����� � �� ����, �� �� ����� ���� � ������

			mMngContextSlave->DeleteByClientKey(id_session_slave, id_client);
    }
  }

	TryAddClientFromQueue();
}
//-------------------------------------------------------------------------
void TMaster::EndDisconnectClient(IScenario* pSc)
{

}
//-------------------------------------------------------------------------
void TMaster::TryAddClientFromQueue()
{
  unsigned int id_client;
  unsigned int id_session_slave;
  if(TryFindClientForAdd(id_client, id_session_slave)==false)
    return;
  unsigned int id_session_client;// ��� ��������
  if(mMngContextClientLogining->FindSessionByClientKey(id_client, id_session_client)==false)
  {
    BL_FIX_BUG();
    return;
  }
  TContainerContextSc* pC = mMngContextClientLogining->FindContextBySession(id_session_client);
  if(pC==NULL)
  {
    BL_FIX_BUG();
    return;
  }

  void* resForClient = pC->mLoginClient.GetSaveQueueDataPtr();
	int sizeResClient  = pC->mLoginClient.GetSaveQueueDataSize();
	// ������ ������� �����������
  mControlSc->mLoginClient->SetContext(&pC->mLoginClient);
  AddClientBySlaveSession(id_client,id_session_slave,resForClient,sizeResClient);
}
//-------------------------------------------------------------------------
unsigned char TMaster::GetLimitLoadProcentByKey(unsigned int id_client)
{
  // ������� ��������: ���� ������ ������� � ������ � � ���� ���������
  // ����������, �� �� ������ ����� � ������� �� ������� � ����� ������,
  // �.�. ��� ��������� ���� ��� � ������ ��������
  unsigned int id_group;
  if(mMngGroup->FindIDByClientKey(id_client, id_group))
    return eLimitLoadProcentOnSlaveForAdd_ClientInGroup;
  return eLimitLoadProcentOnSlaveForAdd;
}
//-------------------------------------------------------------------------
bool TMaster::TryAddClientByGroup(unsigned int id_client, unsigned int id_group, 
                                  unsigned int& id_session_slave)
{
  if(mMngGroup->FindSessionByID(id_group, id_session_slave)==false)
  {
    BL_FIX_BUG();
    return false;
  }
  unsigned char load_procent;
  if(mMngContextSlave->FindLoadBySession(id_session_slave, load_procent)==false)
  {
    BL_FIX_BUG();
    return false;
  }
  if(load_procent < GetLimitLoadProcentByKey(id_client))
    return true;
  return false;
}
//-------------------------------------------------------------------------
bool TMaster::TryAddClient(unsigned int id_client, 
                           unsigned int& id_session_slave)
{
  unsigned char load_procent;
  if(mMngContextSlave->FindMinimumLoad(id_session_slave,load_procent)==false)
  {
  	// ��������� ������
  	GetLogger(STR_NAME_MMO_ENGINE)->
  		WriteF_time("TMaster::SetResultLogin() count slave = 0.\n");
  	BL_FIX_BUG();
  	return false;
  }
  unsigned char limitLoadProcent = GetLimitLoadProcentByKey(id_client);
  if(load_procent<limitLoadProcent)
    return true;
  return false;
}
//-------------------------------------------------------------------------
void TMaster::AddClientBySlaveSession(unsigned int id_client,
                                      unsigned int id_session_slave, 
                                      void* resForClient, int sizeResClient)
{
  // ������ ������� �����������
  mMngContextSlave->AddClientKey(id_session_slave, id_client);
  mControlSc->mLoginClient->Accept(id_client,resForClient,sizeResClient,
                                   id_session_slave,mID_SessionUp);
}
//-------------------------------------------------------------------------
void TMaster::AddInQueue(unsigned int id_client, void* resForClient, int sizeResClient)
{
  // � ������� �� �������� ���������� �����
  mSetClientKeyInQueue->AddKeyAtEnd(id_client);
  int index;
  if(mSetClientKeyInQueue->FindIndexByClientKey(id_client,index))
    mControlSc->mLoginClient->Queue(index + 1,resForClient,sizeResClient);
}
//-------------------------------------------------------------------------
bool TMaster::TryFindClientForAdd(unsigned int& id_client, unsigned int& id_session_slave)
{
  // ������ �� ����� Slave
  // ������� ��������, ������� ����� � �������
  while(1)
  {
    // ���� �� ��� ���� �� ���������� ������� � ������� ��� �� ������
    if(mSetClientKeyInQueue->DeleteFirst(id_client)==false)
      return false;// �����
    unsigned int id_session_client;
    if(mMngContextClientLogining->FindSessionByClientKey(id_client,id_session_client)==false)
      continue;// ������� ���, ������� ����

    unsigned int id_group;
    if(mMngGroup->FindIDByClientKey(id_client, id_group))
    {
      // ��������� � ������, ������ ����� ������ Slave
      if(mMngGroup->FindSessionByID(id_group, id_session_slave)==false)
      {
        BL_FIX_BUG();
        return false;
      }
      return true;
    }
    else
    {
      unsigned char load_procent;
      if(mMngContextSlave->FindMinimumLoad(id_session_slave,load_procent)==false)
      {
        // ��������� ������
        GetLogger(STR_NAME_MMO_ENGINE)->
          WriteF_time("TMaster::SetResultLogin() count slave = 0.\n");
        BL_FIX_BUG();
        return false;
      }
      return true;
    }
  }
}
//-------------------------------------------------------------------------
bool TMaster::IsClientRecommutation(unsigned int id_client)
{
  TContainerContextSc* pC = mMngContextClient->FindContextByClientKey(id_client);
  if(pC==NULL)
    return false;
  if(pC->IsRcmActive())
    return true;
  return false;
}
//-------------------------------------------------------------------------
bool TMaster::LoadInFutureLessLimit(unsigned int id_session_slave, 
                                    list<unsigned int>& l_id_client)
{
  unsigned char load_procent;// ������� ��������
  if(mMngContextSlave->FindLoadBySession(id_session_slave, load_procent)==false)
    return false;
  // ���� �������� ���, �� �����
  if(load_procent==0)
    return true;
  // ����� ���-�� �������� �� Slave
  int countAllClient;
  if(mMngContextSlave->GetCountClientKey(id_session_slave, countAllClient)==false)
    return false;
  if(countAllClient>0)
  {
    int countClientInGroup;// ���-�� ��������, ��������� � �������
    if(mStatisticaClientInGroup->FindCountClientBySlaveSession(id_session_slave, countClientInGroup)==false)
      return false;
    // ���� ���-�� ����������� � ���� ������ ������ ���-�� ��������� � �������,
    // �� ������ ����� ���-�� �� ���������, �� ��������� � ��������,
    // �� ��������� ������ ��� ����� ���������� ����� (��. SolveExchangeClient)
    int countFreeClient = countAllClient - countClientInGroup;
    if(int(l_id_client.size())>countFreeClient)
      return true;
    int countGroupClientInFuture  = countClientInGroup;
    countGroupClientInFuture     += l_id_client.size() - countFreeClient;
    float load_procent_future = (float(load_procent)*countGroupClientInFuture)/countAllClient;
    if(load_procent_future>eLimitLoadProcentOnSlaveForAdd_ClientInGroup)
      return false;
  }
  return true;
}
//-------------------------------------------------------------------------
void TMaster::SolveExchangeClient(unsigned int id_client, 
                                  TContainerContextSc* pC_ClientInGroup, 
                                  unsigned int id_session_slave_recipient)
{
  // ���� ������� �������� ������� ��������� � �������� ��������������,
  // �� ���������� ���������� ��� �� ��� ��� �����, ���������� ������
  if(pC_ClientInGroup->IsRcmActive())
  {
    RcmByClientKeyContextSlaveSessionRecipient(id_client,
      &pC_ClientInGroup->mRcm, 
      id_session_slave_recipient);
    return;
  }
  //===========================================
  // ���� ��� Slave ��������� � �����, �� �����
  unsigned int id_session_slave_donor;
  if(mMngContextClient->FindSessionByClientKey(id_client, id_session_slave_donor)==false)
  {
    BL_FIX_BUG();
    return;
  }
  if(id_session_slave_donor==id_session_slave_recipient)
    return;
  //===========================================
  RcmByClientKeyContextSlaveSessionRecipient(id_client,
    &pC_ClientInGroup->mRcm, 
    id_session_slave_recipient);
  //===========================================
  // ������ ���� ������ �� ���������� ������� ������ ����������
  // ���� ����� ������
  int countClientOnSlaveRecipient;
  if(mMngContextSlave->GetCountClientKey(id_session_slave_recipient, countClientOnSlaveRecipient)==false)
  {
    BL_FIX_BUG();
    return;
  }
  // ������� ���� �������� ����������
  for( int i = 0 ; i < countClientOnSlaveRecipient ; i++ )
  {
    unsigned int id_client_on_recipient;
    if(mMngContextSlave->GetClientKeyByIndex(id_session_slave_recipient, i, id_client_on_recipient)==false)
    {
      BL_FIX_BUG();
      return;
    }
    // �� ������ ���� ��������, ��� ������
    unsigned int id_group;
    if(mMngGroup->FindIDByClientKey(id_client_on_recipient,id_group)==false)
    {
      TContainerContextSc* pC_client_on_recipient = mMngContextClient->FindContextByClientKey(id_client_on_recipient);
      if(pC_client_on_recipient==NULL)
      {
        BL_FIX_BUG();
        return;
      }
      // ���� ������ ��������� � �������� ��������������, �� �� ��� �� ��������, ���� ����������
      if(pC_client_on_recipient->IsRcmActive())
        continue;
      RcmByClientKeyContextSlaveSessionRecipient(id_client_on_recipient,
        &pC_client_on_recipient->mRcm, 
        id_session_slave_donor);
      return;
    }
  }
}
//-------------------------------------------------------------------------
void TMaster::RcmByClientKeyContextSlaveSessionRecipient(unsigned int id_client,
                                           TContextScRecommutationClient* pCRCM, 
                                           unsigned int id_session_slave_recipient)
{
  mControlSc->mRcm->SetContext(pCRCM);
  mControlSc->mRcm->Start(id_session_slave_recipient, id_client);
}
//-------------------------------------------------------------------------
void TMaster::NeedContextSendToClient(unsigned int id_client)
{
	TContainerContextSc* pContext = mMngContextClient->FindContextByClientKey(id_client);
	if(pContext)
		mControlSc->mSendToClient->SetContext(&pContext->mSendToClient);
	else
		mControlSc->mSendToClient->SetContext(NULL);
}
//-------------------------------------------------------------------------
void TMaster::EndRcm(IScenario* pSc)
{
  TContextScRecommutationClient* pContext = (TContextScRecommutationClient*)pSc->GetContext();
  unsigned int key = pContext->GetClientKey();
  mMngRcm->DeleteByClientKey(key);

  // ����������� � ������ Slave �� ������
  unsigned int id_session_recipient = pContext->GetSessionRecipient();
  bool res = mMngContextClient->SetSessionByClientKey(key,id_session_recipient);
  BL_ASSERT(res);
}
//-------------------------------------------------------------------------
void TMaster::NeedContextByClientKeyRcm(unsigned int key)
{
  TContainerContextSc* pC = mMngContextClient->FindContextByClientKey(key);
  if(pC)
    mControlSc->mRcm->SetContext(&pC->mRcm);
  else
    mControlSc->mRcm->SetContext(NULL);
}
//-------------------------------------------------------------------------
void TMaster::NeedSlaveSessionDonorRcm(IScenario* pSc)
{
  TContextScRecommutationClient* pContext = (TContextScRecommutationClient*)pSc->GetContext();
  unsigned int key = pContext->GetClientKey();
  unsigned int id_session_donor;
  if(mMngContextClient->FindSessionByClientKey(key, id_session_donor))
    pContext->SetSessionDonor(id_session_donor);
  else
  {
    GetLogger(STR_NAME_MMO_ENGINE)->
      WriteF_time("TMaster::NeedSlaveSessionDonor() key = %u not found donor.\n", key);
    BL_FIX_BUG();
  }
}
//-------------------------------------------------------------------------
void TMaster::ActivateRcm(IScenario* pSc)
{
  TContextScRecommutationClient* pContext = (TContextScRecommutationClient*)pSc->GetContext();
  unsigned int key = pContext->GetClientKey();
  unsigned int id_session_donor     = pContext->GetSessionDonor(),
    id_session_recipient = pContext->GetSessionRecipient();

  mMngRcm->AddClientKey(key,id_session_donor, id_session_recipient);
}
//-------------------------------------------------------------------------
