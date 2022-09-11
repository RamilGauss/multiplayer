/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
√удаков –амиль —ергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "Slave.h"
#include "Logger.h"
#include "ClientPrivate.h"
#include <boost/foreach.hpp>
#include "ManagerSession.h"
#include "ControlScenario.h"

#include "ScenarioFlow.h"
#include "ScenarioLoginClient.h"
#include "ScenarioLoginSlave.h"
#include "ScenarioRecommutationClient.h"
#include "ScenarioSendToClient.h"
#include "ScenarioSynchroSlave.h"
#include "Events.h"


using namespace nsMelissa;
using namespace std;

TSlave::TSlave()
{
  mTimeNeedSendSynchro = 0;
}
//-------------------------------------------------------------------------
TSlave::~TSlave()
{
  ClearClientPrivate();
}
//-------------------------------------------------------------------------
void TSlave::SaveContext(unsigned int id_session, void* data, int size)
{
  TClientPrivate* pClient = GetClientByID_Session(id_session);
  if(pClient)  // передать контекст в сценарий
  {
    mControlSc->mRcm->SetContext(&pClient->GetC()->mRcm);
    mControlSc->mRcm->SaveContext(data, size);// это уже вызов функции сценари€
  }
  else
    GetLogger()->Get(STR_NAME_MELISSA)->
      WriteF_time("TSlave::SaveContext(%u) запись не найдена.\n", id_session);
}
//-------------------------------------------------------------------------
unsigned int TSlave::GetClientKeyBySession(unsigned int id_session)
{
  TClientPrivate* pClient = GetClientByID_Session(id_session);
  if(pClient)
    return pClient->GetKey();
  return (unsigned int)(-1);
}
//-------------------------------------------------------------------------
unsigned int TSlave::GetSessionByClientKey(unsigned int key)
{
  TClientPrivate* pClient = GetClientByKey(key);
  if(pClient)
    return pClient->GetID_SessionDown();
  return INVALID_HANDLE_SESSION;
}
//-------------------------------------------------------------------------
void TSlave::DisconnectInherit(unsigned int id_session)
{
  // либо обрыв св€зи с верхним соединением - тогда мы одни, нерабочее состо€ние
  if(id_session==mID_SessionUp)
  {
    flgConnectUp = false;
    ClearClientPrivate();// распустить всех клиентов
    mID_SessionUp = INVALID_HANDLE_SESSION;
    
    TEventDisconnectUp event;
    event.id_session = id_session;
    AddEventCopy(&event, sizeof(event));
    return;
  }
  // либо обрыв с одним из клиентов - уничтожить клиента
  // физический обрыв или сценарий сам оборвал
  RemoveClientPrivate(id_session);
  
  TEventDisconnectDown event;
  event.id_session = id_session;
  AddEventCopy(&event, sizeof(event));
}
//-------------------------------------------------------------------------
int TSlave::GetCountDown()
{
	return mMapID_SessionClient.size();
}
//-------------------------------------------------------------------------
bool TSlave::GetDescDown(int index, void* pDesc, int& sizeDesc)
{
  if(sizeDesc<sizeof(TDescDownSlave))
  {
    GetLogger()->Get(STR_NAME_MELISSA)->
      WriteF_time("TSlave::GetDescDown() размер буфера меньше размера структуры.\n");
    return false;
  }
  if(index>=GetCountDown())
  {
    GetLogger()->Get(STR_NAME_MELISSA)->
      WriteF_time("TSlave::GetDescDown() индекс вне массива.\n");
    return false;
  }
  TDescDownSlave* pDescDownSlave = (TDescDownSlave*)pDesc;
  TMapUintPtrIt it = mMapID_SessionClient.begin();
  for( int i = 0 ; i < index ; i++)// + index
    it++;
  pDescDownSlave->id_session = it->second->GetID_SessionDown();
  sizeDesc = sizeof(TDescDownSlave);
	return true;
}
//-------------------------------------------------------------------------
void TSlave::ConnectUp(unsigned int ip, unsigned short port)
{
  mControlSc->mLoginSlave->ConnectToMaster(ip, port);
  // тут же опросить на созданную сессию
  mID_SessionUp = mContainerUp->mLoginSlave.GetID_Session();
}
//-------------------------------------------------------------------------
void TSlave::WorkInherit()
{
  // пока нет св€зи с ћастером - синхронизацию не проводить
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
void TSlave::ClearClientPrivate()
{
  BOOST_FOREACH(TMapUintPtr::value_type& bit, mMapID_SessionClient)
  {
    mManagerSession->CloseSession(bit.second->GetID_SessionDown());
    delete bit.second;

    TEventDisconnectDown event;
    event.id_session = bit.first;
    AddEventCopy(&event, sizeof(event));
  }

  mMapID_SessionClient.clear();
  mMapKeyClient.clear();
}
//-------------------------------------------------------------------------
TClientPrivate* TSlave::AddClientPrivate( unsigned int key, unsigned int id_session )
{
  TClientPrivate* pClient = new TClientPrivate;
  pClient->SetID_SessionDown(id_session);
  pClient->SetKey(key);

  mMapKeyClient.insert(TMapUintPtr::value_type(key,pClient));
  mMapID_SessionClient.insert(TMapUintPtr::value_type(id_session,pClient));
  return pClient;
}
//-------------------------------------------------------------------------
void TSlave::RemoveClientPrivate(unsigned int id_session)
{
  TClientPrivate* pClient = GetClientByID_Session(id_session);
  if(pClient==NULL)
  {
    GetLogger()->Get(STR_NAME_MELISSA)->
      WriteF_time("TSlave::RemoveClientPrivate(%u) запись не найдена.\n", id_session);
    return;
  }
  mMapID_SessionClient.erase(pClient->GetID_SessionDown());
  mMapKeyClient.erase(pClient->GetKey());

  delete pClient;
}
//-------------------------------------------------------------------------
TClientPrivate* TSlave::GetClientByUint(TMapUintPtr& m, unsigned int k)
{
  TMapUintPtrIt fit = m.find(k);
  if(m.end()==fit)
  {
    GetLogger()->Get(STR_NAME_MELISSA)->
      WriteF_time("TSlave::GetClientByUint(%u) запись не найдена.\n", k);
    return NULL;
  }
  return fit->second;
}
//-------------------------------------------------------------------------
TClientPrivate* TSlave::GetClientByKey(unsigned int key)
{
  return GetClientByUint(mMapKeyClient, key);
}
//-------------------------------------------------------------------------
TClientPrivate* TSlave::GetClientByID_Session(unsigned int id_session)
{
  return GetClientByUint(mMapID_SessionClient, id_session);
}
//-------------------------------------------------------------------------
void TSlave::SendDown(unsigned int id_session, TBreakPacket bp, bool check)
{
  TClientPrivate* pClient = GetClientByID_Session(id_session);
  if(pClient)
  {
    mControlSc->mFlow->SetContext(&pClient->GetC()->mFlow);
    mControlSc->mFlow->SendDown(bp, check);
  }
  else
    GetLogger()->Get(STR_NAME_MELISSA)->
      WriteF_time("TSlave::SendDown(%u) запись не найдена.\n", id_session);
}
//-------------------------------------------------------------------------
void TSlave::EndLoginClient(IScenario*)
{

}
//-------------------------------------------------------------------------
void TSlave::EndLoginSlave(IScenario* pSc)
{
  // вз€ть по этому контексту, задать всем контекстам
  mID_SessionUp = pSc->GetContext()->GetID_Session();
  mContainerUp->SetID_Session(mID_SessionUp);

  flgConnectUp = mContainerUp->mLoginSlave.IsConnect();
  if(flgConnectUp)
  {
    // вход в кластер закончен
    TEventConnectUp event;
    event.id_session = mID_SessionUp;
    AddEventCopy(&event, sizeof(event));
  }
}
//-------------------------------------------------------------------------
void TSlave::EndRcm(IScenario*)
{

}
//-------------------------------------------------------------------------
void TSlave::NeedContextLoginClient(unsigned int id_session)
{
  if(id_session==mID_SessionUp)
  {
    // создание нового клиента

  }
  else
  {
    //TClientPrivate* pClient = GetClientByID_Session(id_session);
    //if(pClient)
      //mControlSc->mLoginClient->SetContext(&pClient->GetC()->mSendToClient);
  }
}
//-------------------------------------------------------------------------
void TSlave::NeedContextRcm(unsigned int id_session)
{

}
//-------------------------------------------------------------------------
void TSlave::NeedContextSendToClient(unsigned int id_session_client)
{
  // запрос на отправку какому-то клиенту
  TClientPrivate* pClient = GetClientByID_Session(id_session_client);
  if(pClient)
    mControlSc->mSendToClient->SetContext(&pClient->GetC()->mSendToClient);
}
//-------------------------------------------------------------------------
void TSlave::SendByClientKey(list<unsigned int>& l, TBreakPacket bp)
{

}
//-------------------------------------------------------------------------