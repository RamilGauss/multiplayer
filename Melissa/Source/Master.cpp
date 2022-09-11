/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
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
#include "ScenarioDisconnectSlave.h"


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
  // проверка на существование всех клиентов
  BOOST_FOREACH(unsigned int id, l_id_client)
  {
    TClientForMasterPrivate* pClient = GetClientByKey(id);
    if(pClient==NULL)
      return false;
    // если клиент существует, то спросить состоит ли он в группе
    if(pClient->GetGroupID()!=-1)
    {
      // генерация ошибки
      GetLogger(STR_NAME_MELISSA)->
        WriteF_time("TMaster::TryCreateGroup() Client(%u) is entered in group id=%u.\n",
                    id,pClient->GetGroupID());
      return false;
    }
    // нельзя объединять в группу если клиент авторизуется
    if(pClient->GetState()==TClientForMasterPrivate::eLogin)
    {
      // генерация ошибки
      GetLogger(STR_NAME_MELISSA)->
        WriteF_time("TMaster::TryCreateGroup() Client(%u) is been authorized.\n",id);
      return false;
    }
  }
  // оценка ситуации на Slave
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
    pClient->GetSlave()->AddDeltaFreeClient(1);
  }

  mMapIDGroupClient.erase(id_group);
  delete pGroup;
  // генерация события
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
    GetLogger(STR_NAME_MELISSA)->
      WriteF_time("TMaster::LeaveGroup() Client(%u) don't belong to group.\n",id_client);
    return;
  }
  pClient->GetSlave()->AddDeltaFreeClient(1);
  pClient->SetGroupID(-1);// не состоит ни в одной группе

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
    // генерация ошибки
    GetLogger(STR_NAME_MELISSA)->
      WriteF_time("TMaster::SetResultLogin() register Client by busy key=%u.\n",id_client);
    return;
  }
  mMapID_SessionClientWait.erase(id_session);
  mMapKeyClient.insert(TMapUintClientPtr::value_type(id_client,pClient));
  mIDSessionKey.insert(TMapUintUint::value_type(id_session,id_client));
  pClient->SetState(TClientForMasterPrivate::eLogin);

  // решить на какой Slave закинуть клиента
  unsigned int id_session_slave = 0;
  mControlSc->mLoginClient->Accept(id_client,resForClient,sizeResClient,id_session_slave);
}
//-------------------------------------------------------------------------
unsigned int TMaster::GetSlaveSessionByGroup(unsigned int id_group)
{
  TGroupClientPrivate* pGroup = GetGroupByKey(id_group);
  if(pGroup==NULL)
    return -1;
  if(pGroup->GetClient().size()==0)
  {
    GetLogger(STR_NAME_MELISSA)->
      WriteF_time("TMaster::GetSlaveSessionByGroup() empty group id=%u.\n",id_group);
    return -1;
  }
  TClientForMasterPrivate* pClient = *(pGroup->GetClient().begin());
  return pClient->GetSlave()->GetID_Session();
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
  GetLogger(STR_NAME_MELISSA)->
    WriteF_time("TMaster::DisconnectInherit() session not exist id=%u.\n",id_session);
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
    GetLogger(STR_NAME_MELISSA)->
      WriteF_time("TMaster::GetDescDown() size of buffer less then size of structure.\n");
    return false;
  }
  if(index>=GetCountDown())
  {
    GetLogger(STR_NAME_MELISSA)->
      WriteF_time("TMaster::GetDescDown() index is out of band.\n");
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
  // тут же опросить на созданную сессию
  mID_SessionUp = mControlSc->mLoginMaster->GetContext()->GetID_Session();
}
//-------------------------------------------------------------------------
void TMaster::EndLoginMaster(IScenario* pSc)
{
  // взять по этому контексту, задать всем контекстам
  mID_SessionUp = pSc->GetContext()->GetID_Session();
  mContainerUp->SetID_Session(mID_SessionUp);

  flgConnectUp = mContainerUp->mLoginMaster.IsConnect();
  if(flgConnectUp)
  {
    // вход в кластер закончен
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
    GetLogger(STR_NAME_MELISSA)->
      WriteF_time("TMaster::GetClientByIDSessionWait() Client id=%u is not founded.\n",id);
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
    GetLogger(STR_NAME_MELISSA)->
      WriteF_time("TMaster::GetClientByID() Client id=%u is not founded.\n",id);
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

  //if(((TContextScLoginClient*)pSc->GetContext())->IsReject())
  //{
  //  // удалить 
  //  TClientForMasterPrivate* pClient = GetClientByIDSessionWait(id_session);
  //  if(pClient==NULL)
  //    return;
  //  mMapID_SessionClientWait.erase(id_session);
  //  delete pClient;
  //}
}
//-------------------------------------------------------------------------
void TMaster::EndLoginClientBySlave(IScenario* pSc)
{
  //TScenarioLoginClientByMaster1* pLCM1 = (TScenarioLoginClientByMaster1*)pSc;
  //unsigned int id_session = pLCM1->GetID_Session();
  //if(pLCM1->IsReject())
  //{
  //  // удалить 
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
    GetLogger(STR_NAME_MELISSA)->
      WriteF_time("TMaster::GetGroupByKey() group key=%u is not founded.\n",key);
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
    GetLogger(STR_NAME_MELISSA)->
      WriteF_time("TMaster::GetSlaveByIDSession() Slave id=%u is not founded.\n",id);
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
  mID_SessionUp = INVALID_HANDLE_SESSION;
  flgConnectUp  = false;

  TEventDisconnectUp event;
  event.id_session = mID_SessionUp;
  AddEventCopy(&event, sizeof(event));
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
  // составить список клиентов, которые сидели на Slave
  std::vector<unsigned int> vID_client;
  pSlave->GetVectorID_client(vID_client);
  mControlSc->mDisSlave->SendFromMasterToSuperServer(vID_client);

  unsigned int id_slave = pSlave->GetC()->GetID_Session();

  mMapID_SessionSlave.erase(pSlave->GetID_Session());
  delete pSlave;
  // все кто сидят на Slave или будут туда перекоммутированы
  TEventDisconnectDown event;
  event.id_session = id_slave;
  AddEventCopy(&event, sizeof(event));
}
//-------------------------------------------------------------------------
bool TMaster::EvalCreateGroupNow(list<unsigned int>& l_id_client, unsigned int& id_group)
{
  TListSlavePrior listSlavePrior;  // составить список по приоритету
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
      float delFG = float(loadFree)/loadGroup;// соотношения групповых к свободным
      if( delFG < 0.5f )// групповых намного меньше
        desc.type = TDescChoiseSlave::eMFree; 
      else if( delFG > 2.0f)// групповых намного больше
        desc.type = TDescChoiseSlave::eMGroup;
      else // примерно поровну
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
void TMaster::NeedContextLoginClientByClientKey(unsigned int id_key_client)
{
  
}
//-------------------------------------------------------------------------
void TMaster::NeedContextLoginClient(unsigned int id_session)
{
  TClientForMasterPrivate* pClient = GetClientByIDSessionWait(id_session);
  if(pClient)
  {
    // внутренняя ошибка
    GetLogger(STR_NAME_MELISSA)->
      WriteF_time("TMaster::LoginClient() against try authorized.\n");
    return;
  }
  pClient = new TClientForMasterPrivate;
  SetupScForContext(pClient->GetC());
  pClient->GetC()->SetID_Session(id_session);
  pClient->GetC()->SetUserPtr(pClient);

  mMapID_SessionClientWait.insert(TMapUintClientPtr::value_type(id_session,pClient));
  // начать сценарий авторизации
  //mControlSc->mLoginClient->Start();
}
//-------------------------------------------------------------------------
void TMaster::NeedContextLoginSlave(unsigned int id_session)
{
  if(GetSlaveByIDSession(id_session))
  {
    // внутренняя ошибка
    GetLogger(STR_NAME_MELISSA)->
      WriteF_time("TMaster::LoginSlave() against try authorized.\n");
    return;
  }
  TSlavePrivate* pSlave = new TSlavePrivate;
  SetupScForContext(pSlave->GetC());
  pSlave->GetC()->SetID_Session(id_session);
  pSlave->GetC()->SetUserPtr(pSlave);
  mMapID_SessionSlave.insert(TMapUintSlavePtr::value_type(id_session,pSlave));

  mControlSc->mLoginSlave->SetContext(&pSlave->GetC()->mLoginSlave);
  // событие наружу
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