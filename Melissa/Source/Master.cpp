/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "Master.h"
#include "ControlScenario.h"

#include "Logger.h"
#include "ClientForMasterPrivate.h"
#include "SlavePrivate.h"
#include "BL_Debug.h"
#include "GroupClientPrivate.h"
#include "Events.h"
#include "ScenarioLoginClient.h"
#include "ScenarioLoginSlave.h"
#include "ScenarioLoginMaster.h"
#include "ScenarioFlow.h"
#include "ScenarioRecommutationClient.h"
#include "ScenarioSynchroSlave.h"


using namespace std;
using namespace nsMelissa;

TMaster::TMaster()
{
  mCurCounterGroup = 0;
}
//-------------------------------------------------------------------------
TMaster::~TMaster()
{
  Done();
}
//-------------------------------------------------------------------------
bool TMaster::TryCreateGroup(list<unsigned int>& l_id_client, unsigned int& id_group)
{
  // �������� �� ������������� ���� ��������
  BOOST_FOREACH(unsigned int id, l_id_client)
  {
    TClientForMasterPrivate* pClient = GetClientByKey(id);
    if(pClient==NULL)
      return false;
    // ���� ������ ����������, �� �������� ������� �� �� � ������
    if(pClient->GetGroupID()!=-1)
    {
      // ��������� ������
      GetLogger()->Get(STR_NAME_MELISSA)->
        WriteF_time("TMaster::TryCreateGroup() ������(%u) ��� ��������� � ������ id=%u.\n",
                    id,pClient->GetGroupID());
      return false;
    }
    // ������ ���������� � ������ ���� ������ ������������
    if(pClient->GetState()==TClientForMasterPrivate::eLogin)
    {
      // ��������� ������
      GetLogger()->Get(STR_NAME_MELISSA)->
        WriteF_time("TMaster::TryCreateGroup() ������(%u) ��������� � �������� �����������.\n",id);
      return false;
    }
  }
  // ������ �������� �� Slave
  return EvalCreateGroupNow(l_id_client,id_group);
}
//-------------------------------------------------------------------------
void TMaster::DestroyGroup(unsigned int id_group)
{
  TGroupClientPrivate* pGroup = GetGroupByKey(id_group);
  if(pGroup==NULL)
    return;
  
  BOOST_FOREACH(TClientForMasterPrivate* pClient, pGroup->GetClient())
  {
    pClient->SetGroupID(-1);
    pClient->GetSlave()->AddFreeClient(1);
  }

  mMapIDGroupClient.erase(id_group);
  delete pGroup;
  // ��������� �������
  TEventDestroyGroup event;
  event.id_group = id_group;
  AddEventCopy(&event,sizeof(event)); 
}
//-------------------------------------------------------------------------
void TMaster::LeaveGroup(unsigned int id_client)
{
  TClientForMasterPrivate* pClient = GetClientByKey(id_client);
  if(pClient==NULL)
    return;
  unsigned int id_group = pClient->GetGroupID();
  if(id_group==-1)
  {
    GetLogger()->Get(STR_NAME_MELISSA)->
      WriteF_time("TMaster::LeaveGroup() ������ id=%u �� ������� � ������.\n",id_client);
    return;
  }
  pClient->GetSlave()->AddFreeClient(1);
  pClient->SetGroupID(-1);// �� ������� �� � ����� ������

  TGroupClientPrivate* pGroup = GetGroupByKey(id_group);
  if(pGroup==NULL)
    return;
  pGroup->GetClient().erase(pClient);

  TEventLeaveGroup event;
  event.id_client = id_group;
  AddEventCopy(&event,sizeof(event)); 
}
//-------------------------------------------------------------------------
void TMaster::GetListForGroup(unsigned int id_group, std::list<unsigned int>& l)
{
  TGroupClientPrivate* pGroup = GetGroupByKey(id_group);
  if(pGroup==NULL)
    return;

  BOOST_FOREACH(TClientForMasterPrivate* pClient, pGroup->GetClient())
    l.push_back(pClient->GetKey());
}
//-------------------------------------------------------------------------
void TMaster::SetResultLogin(bool res, unsigned int id_session, 
                             unsigned int id_client, 
                             void* resForClient, int sizeResClient)
{
  TClientForMasterPrivate* pClient = GetClientByIDSession(id_session);
  if(pClient==NULL)
    return;
  mControlSc->mLoginClient->SetContext(&pClient->GetC()->mLoginClient);
  if(res==false)
  {
    mControlSc->mLoginClient->Reject(resForClient, sizeResClient);
    return;
  }
  if(GetClientByKey(id_client))
  {
    // ��������� ������
    GetLogger()->Get(STR_NAME_MELISSA)->
      WriteF_time("TMaster::SetResultLogin() ����������� ������� �� ��� ��������� �����=%u.\n",id_client);
    return;
  }
  mMapID_SessionClientWait.erase(id_session);
  mMapKeyClient.insert(TMapUintClientPtr::value_type(id_client,pClient));
  mIDSessionKey.insert(TMapUintUint::value_type(id_session,id_client));
  pClient->SetState(TClientForMasterPrivate::eLogin);

  // ������ �� ����� Slave �������� �������
  //mControlSc->mLoginClient->Accept(id_client,resForClient,sizeResClient);
}
//-------------------------------------------------------------------------
unsigned int TMaster::GetSlaveSessionByGroup(unsigned int id_group)
{
  TGroupClientPrivate* pGroup = GetGroupByKey(id_group);
  if(pGroup==NULL)
    return -1;
  if(pGroup->GetClient().size()==0)
  {
    GetLogger()->Get(STR_NAME_MELISSA)->
      WriteF_time("TMaster::GetSlaveSessionByGroup() ������ ������ id=%u.\n",id_group);
    return -1;
  }
  TClientForMasterPrivate* pClient = *(pGroup->GetClient().begin());
  return pClient->GetSlave()->GetID_Session();
}
//-------------------------------------------------------------------------
void TMaster::SendToClient(unsigned int id_client, void* data, int size, bool check )
{
  TClientForMasterPrivate* pClient = GetClientByKey(id_client);
  if(pClient==NULL)
    return;

  if(pClient->GetSlave()==NULL)
  {
    BL_FIX_BUG();
    return;
  }
  pClient->GetSlave()->SendToClient(id_client, data, size, check);
}
//-------------------------------------------------------------------------
void TMaster::DisconnectInherit(unsigned int id_session)
{
  if(id_session==mID_SessionUp)
  {
    DisconnectSuperServer(); 
    return;
  }
  TClientForMasterPrivate* pClient = GetClientByIDSessionWait(id_session);
  if(pClient)
  {
    DisconnectClientWait(pClient);
    return;
  }
  pClient = GetClientByIDSession(id_session);
  if(pClient)
  {
    DisconnectClient(pClient);
    return;
  }
  TSlavePrivate* pSlave = GetSlaveByIDSession(id_session);
  if(pSlave)
  {
    DisconnectSlave(pSlave);
    return;
  }
  GetLogger()->Get(STR_NAME_MELISSA)->
    WriteF_time("TMaster::DisconnectInherit() ��� ����� ������ id=%u.\n",id_session);
  BL_FIX_BUG();
}
//-------------------------------------------------------------------------
int TMaster::GetCountDown()
{
	return mMapID_SessionSlave.size();
}
//-------------------------------------------------------------------------
bool TMaster::GetDescDown(int index, void* pDesc, int& sizeDesc)
{
  if(sizeDesc<sizeof(TDescDownMaster))
  {
    GetLogger()->Get(STR_NAME_MELISSA)->
      WriteF_time("TMaster::GetDescDown() ������ ������ ������ ������� ���������.\n");
    return false;
  }
  if(index>=GetCountDown())
  {
    GetLogger()->Get(STR_NAME_MELISSA)->
      WriteF_time("TMaster::GetDescDown() ������ ��� �������.\n");
    return false;
  }
  TMapUintSlavePtrIt it = mMapID_SessionSlave.begin();
  for(int i = 0 ; i < index ; i++)
    it++;
  TDescDownMaster* pD = (TDescDownMaster*)pDesc;
  pD->id_session = it->second->GetID_Session();
  sizeDesc = sizeof(TDescDownMaster);
  return true;
}
//-------------------------------------------------------------------------
void TMaster::ConnectUp(unsigned int ip, unsigned short port)
{
  mControlSc->mLoginMaster->ConnectToSuperServer(ip, port);
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
  TSlavePrivate* pSlave = GetSlaveByIDSession(id_session);
  if(pSlave==NULL)
    return;
  mControlSc->mFlow->SetContext(&pSlave->GetC()->mFlow);
  mControlSc->mFlow->SendDown(bp,check);
}
//-------------------------------------------------------------------------
TClientForMasterPrivate* TMaster::GetClientByIDSession(unsigned int id)
{
  return GetClientByID(mMapID_SessionClientWait,id);
}
//-------------------------------------------------------------------------
TClientForMasterPrivate* TMaster::GetClientByIDSessionWait(unsigned int id)
{
  TMapUintUintIt fit = mIDSessionKey.find(id);
  if(fit==mIDSessionKey.end())
  {
    GetLogger()->Get(STR_NAME_MELISSA)->
      WriteF_time("TMaster::GetClientByIDSessionWait() ������ id=%u �� ������.\n",id);
    return NULL;
  }
  return GetClientByID(mMapID_SessionClientWait, fit->second);
}
//-------------------------------------------------------------------------
TClientForMasterPrivate* TMaster::GetClientByKey(unsigned int key)
{
  return GetClientByID(mMapKeyClient,key);
}
//-------------------------------------------------------------------------
TClientForMasterPrivate* TMaster::GetClientByID(TMapUintClientPtr& m, unsigned int id)
{
  TMapUintClientPtrIt fit = m.find(id);
  if(fit!=m.end())
  {
    GetLogger()->Get(STR_NAME_MELISSA)->
      WriteF_time("TMaster::GetClientByID() ������ id=%u �� ������.\n",id);
    return fit->second;
  }
  return NULL;
}
//-------------------------------------------------------------------------
void TMaster::WorkInherit()
{
}
//-------------------------------------------------------------------------
void TMaster::EndLoginClient(IScenario* pSc)
{
  unsigned int id_session = pSc->GetContext()->GetID_Session();

  if(((TContextScLoginClient*)pSc->GetContext())->IsReject())
  {
    // ������� 
    TClientForMasterPrivate* pClient = GetClientByIDSessionWait(id_session);
    if(pClient==NULL)
      return;
    mMapID_SessionClientWait.erase(id_session);
    delete pClient;
  }
}
//-------------------------------------------------------------------------
void TMaster::EndLoginClientBySlave(IScenario* pSc)
{
  //TScenarioLoginClientByMaster1* pLCM1 = (TScenarioLoginClientByMaster1*)pSc;
  //unsigned int id_session = pLCM1->GetID_Session();
  //if(pLCM1->IsReject())
  //{
  //  // ������� 
  //  TClientForMasterPrivate* pClient = GetClientByIDSessionWait(id_session);
  //  if(pClient==NULL)
  //    return;
  //  mMapID_SessionClientWait.erase(id_session);
  //  delete pClient;
  //}
  //else
  //{
  //  TClientForMasterPrivate* pClient = GetClientByIDSession(id_session);
  //  if(pClient==NULL)
  //    return;
  //  pClient->SetState(TClientForMasterPrivate::eReady);
  //  mIDSessionKey.erase(id_session);
  //}
}
//-------------------------------------------------------------------------
TGroupClientPrivate* TMaster::GetGroupByKey(unsigned int key)
{
  TMapUintGroupClientPtrIt fit = mMapIDGroupClient.find(key);
  if(mMapIDGroupClient.end()==fit)
  {
    GetLogger()->Get(STR_NAME_MELISSA)->
      WriteF_time("TMaster::GetGroupByKey() ������ key=%u �� �������.\n",key);
    return NULL;
  }
  return fit->second;
}
//-------------------------------------------------------------------------
TSlavePrivate* TMaster::GetSlaveByIDSession(unsigned int id)
{
  TMapUintSlavePtrIt fit = mMapID_SessionSlave.find(id);
  if(mMapID_SessionSlave.end()==fit)
  {
    GetLogger()->Get(STR_NAME_MELISSA)->
      WriteF_time("TMaster::GetSlaveByIDSession() Slave id=%u �� ������.\n",id);
    return NULL;
  }
  return fit->second;
}
//-------------------------------------------------------------------------
void TMaster::Done()
{
  
}
//-------------------------------------------------------------------------
void TMaster::DisconnectSuperServer()
{
  TEventDisconnectUp event;
  event.id_session = mID_SessionUp;
  AddEventCopy(&event, sizeof(event));

  mID_SessionUp = INVALID_HANDLE_SESSION;
  flgConnectUp  = false;
}
//-------------------------------------------------------------------------
void TMaster::DisconnectClientWait(TClientForMasterPrivate* pClient)
{

}
//-------------------------------------------------------------------------
void TMaster::DisconnectClient(TClientForMasterPrivate* pClient)
{

}
//-------------------------------------------------------------------------
void TMaster::DisconnectSlave(TSlavePrivate* pSlave)
{
  // ��� ��� ����� �� Slave ��� ����� ���� �����������������
  TEventDisconnectDown event;
  event.id_session = pSlave->GetID_Session();
  AddEventCopy(&event, sizeof(event));

  mMapID_SessionSlave.erase(pSlave->GetID_Session());
  delete pSlave;
}
//-------------------------------------------------------------------------
bool TMaster::EvalCreateGroupNow(list<unsigned int>& l_id_client, unsigned int& id_group)
{
  TListSlavePrior listSlavePrior;  // ��������� ������ �� ����������
  BOOST_FOREACH(TMapUintSlavePtr::value_type& bit,mMapID_SessionSlave)
  {
    TSlavePrivate* pSlave = bit.second;
    if(pSlave->GetState()==TSlavePrivate::eConnect)
      continue;
    int loadFree  = pSlave->GetLoadProcentFreeClient();
    int loadGroup = pSlave->GetLoadProcentGroupClient();
    TDescChoiseSlave desc;
    desc.pSlave = pSlave;
    if(pSlave->GetLoadProcent()<eLimitMoreEmpty)
      desc.type = TDescChoiseSlave::eEmpty;
    else if(pSlave->GetLoadProcent()<eLimitMoreHalf)
    {
      if( loadFree > loadGroup )
        desc.type = TDescChoiseSlave::eFree;
      else
        desc.type = TDescChoiseSlave::eGroup;
    }
    else
    {
      float delFG = float(loadFree)/loadGroup;// ����������� ��������� � ���������
      if( delFG < 0.5f )// ��������� ������� ������
        desc.type = TDescChoiseSlave::eMFree; 
      else if( delFG > 2.0f)// ��������� ������� ������
        desc.type = TDescChoiseSlave::eMGroup;
      else // �������� �������
        desc.type = TDescChoiseSlave::eGroupFree;
    }
  }
  TSlavePrivate* pFindSlave = AnalizListSlaveForGroup(listSlavePrior);
  if(pFindSlave==NULL)
    return false;
  id_group = CreateGroup(pFindSlave,l_id_client);
  return true;
}
//-------------------------------------------------------------------------
TSlavePrivate* TMaster::AnalizListSlaveForGroup(TListSlavePrior& l_slave_prior)
{
  BOOST_FOREACH(TDescChoiseSlave desc,l_slave_prior)
  {
    switch(desc.type)
    {   
      case TDescChoiseSlave::eEmpty:
        break;
      case TDescChoiseSlave::eFree:
        break;
      case TDescChoiseSlave::eMFree:
        break;
      case TDescChoiseSlave::eGroup:
        break;
      case TDescChoiseSlave::eMGroup:
        break;
      case TDescChoiseSlave::eGroupFree:
        break;
    }
  }
  return NULL;
}
//-------------------------------------------------------------------------
unsigned int TMaster::CreateGroup(TSlavePrivate* pSlave,
                                  list<unsigned int>& l_id_client)
{
  mCurCounterGroup++;
  set<TClientForMasterPrivate*> setClient;
  BOOST_FOREACH(unsigned int id, l_id_client)
  {
    TClientForMasterPrivate* pClient = GetClientByKey(id);
    if(pClient)
    {
      pClient->SetGroupID(mCurCounterGroup);
      mControlSc->mRcm->SetContext(&pClient->GetC()->mRcm);
      mControlSc->mRcm->Start(pSlave->GetID_Session(), pClient);
      setClient.insert(set<TClientForMasterPrivate*>::value_type(pClient));
    }
  }
  TGroupClientPrivate* pGroup = new TGroupClientPrivate;
  pGroup->SetID(mCurCounterGroup);
  pGroup->SetClient(setClient);
  mMapIDGroupClient.insert(TMapUintGroupClientPtr::value_type(mCurCounterGroup,pGroup));
  return mCurCounterGroup;
}
//-------------------------------------------------------------------------
void TMaster::EndRcm(IScenario* pSc)
{
  
}
//-------------------------------------------------------------------------
void TMaster::NeedContextLoginClient(unsigned int id_session)
{
  if(GetClientByIDSessionWait(id_session))
  {
    // ���������� ������
    GetLogger()->Get(STR_NAME_MELISSA)->
      WriteF_time("TMaster::LoginClient() ��������� ������� �����������.\n");
    return;
  }
  TClientForMasterPrivate* pClient = new TClientForMasterPrivate;
  SetupScForContext(pClient->GetC());
  pClient->GetC()->SetID_Session(id_session);
  pClient->GetC()->SetUserPtr(pClient);

  mMapID_SessionClientWait.insert(TMapUintClientPtr::value_type(id_session,pClient));
  // ������ �������� �����������
  //mControlSc->mLoginClient->Start();
}
//-------------------------------------------------------------------------
void TMaster::NeedContextLoginSlave(unsigned int id_session)
{
  if(GetSlaveByIDSession(id_session))
  {
    // ���������� ������
    GetLogger()->Get(STR_NAME_MELISSA)->
      WriteF_time("TMaster::LoginSlave() ��������� ������� �����������.\n");
    return;
  }
  TSlavePrivate* pSlave = new TSlavePrivate;
  SetupScForContext(pSlave->GetC());
  pSlave->GetC()->SetID_Session(id_session);
  pSlave->GetC()->SetUserPtr(pSlave);
  mMapID_SessionSlave.insert(TMapUintSlavePtr::value_type(id_session,pSlave));

  mControlSc->mLoginSlave->SetContext(&pSlave->GetC()->mLoginSlave);
  // ������� ������
  TEventConnectDown event;
  event.id_session = id_session;
  AddEventCopy(&event, sizeof(event));
}
//-------------------------------------------------------------------------
void TMaster::NeedContextRcm(unsigned int id_session)
{

}
//-------------------------------------------------------------------------
void TMaster::NeedContextSynchroSlave(unsigned int id_session)
{
  TSlavePrivate* pSlave = GetSlaveByIDSession(id_session);
  if(pSlave==NULL)
    return;
  mControlSc->mSynchroSlave->SetContext(&pSlave->GetC()->mSynchroSlave);
}
//-------------------------------------------------------------------------
void TMaster::EndSynchroSlave(IScenario* pSc)
{
  TContextScSynchroSlave* pContextSynchroSlave = (TContextScSynchroSlave*)pSc->GetContext();
  TSlavePrivate* pSlave = (TSlavePrivate*)pContextSynchroSlave->GetUserPtr();
  BL_ASSERT(pSlave);
  pSlave->SetLoadProcent(pContextSynchroSlave->GetLoadProcent());
}
//-------------------------------------------------------------------------
void TMaster::SendByClientKey(list<unsigned int>& l, TBreakPacket bp)
{

}
//-------------------------------------------------------------------------