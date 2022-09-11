/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
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
#include "SetOrderElement.h"

#include "Events.h"

#include "ScenarioLoginClient.h"
#include "ScenarioLoginSlave.h"
#include "ScenarioLoginMaster.h"
#include "ScenarioFlow.h"
#include "ScenarioRecommutationClient.h"
#include "ScenarioSynchroSlave.h"
#include "ScenarioDisconnectClient.h"

#include "ContainerContextSc.h"

using namespace std;
using namespace nsMMOEngine;

TMaster::TMaster():
mMngContextClient(new TManagerContextMoreDownClientConnection),
mMngContextSlave (new TManagerContextDownConnection_Slave),
mMngContextClientLogining(new TManagerContextClientLogining),
mMngGroup(new TManagerGroupClient),
mSetClientKeyInQueue(new TSetOrderElement)
{
  mControlSc->mLoginClient->SetBehavior(TScenarioLoginClient::eMaster);
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
  BOOST_FOREACH(unsigned int id_client, l_id_client)
  {
    if(mMngContextClient->FindContextByClientKey(id_client)==NULL)
      return false;
    // если клиент существует, то спросить состоит ли он в группе
    if(mMngGroup->FindIDByClientKey(id_client,id_group))
    {
      // генерация ошибки
      GetLogger(STR_NAME_MMO_ENGINE)->
        WriteF_time("TMaster::TryCreateGroup() Client(%u) was entered in group id=%u.\n",
                    id_client,id_group);
      return false;
    }
  }
  // оценка ситуации на Slave
  return EvalCreateGroupNow(l_id_client,id_group);
}
//-------------------------------------------------------------------------
void TMaster::DestroyGroup(unsigned int id_group)
{
  mMngGroup->DeleteByID(id_group);
  // генерация события
  TEventDestroyGroup event;
  event.id_group = id_group;
  AddEventCopy(&event,sizeof(event)); 
}
//-------------------------------------------------------------------------
void TMaster::LeaveGroup(unsigned int id_client)
{
  unsigned int id_group;
  if(mMngGroup->FindIDByClientKey(id_client,id_group)==false)
    return;

  mMngGroup->DeleteClientKey(id_client);

  TEventLeaveGroup event;
  event.id_client = id_client;
  event.id_group  = id_group;
  AddEventCopy(&event,sizeof(event)); 
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
void TMaster::SetResultLogin(bool res, unsigned int id_session, 
                             unsigned int id_client, 
                             void* resForClient, int sizeResClient)
{
  TContainerContextSc* pC = mMngContextClientLogining->FindContextBySession(id_session);
  if(pC==NULL)
    return;

  mControlSc->mLoginClient->SetContext(&pC->mLoginClient);
  if(res==false)
  {
		// отказ
    mControlSc->mLoginClient->Reject(resForClient, sizeResClient);
    return;
  }
  // проверка на существование ключа в кластере
  unsigned int id_session_temp;
  if((mMngContextClient->FindContextByClientKey(id_client))||
     (mMngContextClientLogining->FindSessionByClientKey(id_client,id_session_temp)))
  {
    // генерация ошибки
    GetLogger(STR_NAME_MMO_ENGINE)->
      WriteF_time("TMaster::SetResultLogin() register Client by busy key=%u.\n",id_client);
    return;
  }
  mMngContextClientLogining->AddKeyBySession(id_session, id_client);
  // решить на какой Slave закинуть клиента
  unsigned int id_session_slave;
	unsigned char load_procent;
	if(mMngContextSlave->FindMinimumLoad(id_session_slave,load_procent)==false)
	{
		// генерация ошибки
		GetLogger(STR_NAME_MMO_ENGINE)->
			WriteF_time("TMaster::SetResultLogin() count slave = 0.\n");
		BL_FIX_BUG();
		return;
	}
	if(load_procent<eLimitLoadProcentOnSlaveForAdd)
	{
		// начали процесс авторизации
		mMngContextSlave->AddClientKey(id_session_slave, id_client);
		mControlSc->mLoginClient->Accept(id_client,resForClient,sizeResClient,
			id_session_slave,mID_SessionUp);
	}
	else
	{
		// в очередь на ожидание свободного места
		mSetClientKeyInQueue->AddKeyAtEnd(id_client);
		int index;
		if(mSetClientKeyInQueue->FindIndexByClientKey(id_client,index))
			mControlSc->mLoginClient->Queue(index,resForClient,sizeResClient);
	}
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
    //WriteF_time("TMaster::DisconnectInherit() session not exist id=%u.\n",id_session);
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
  // получить контекст
  TContextScLoginClient* pContextLoginClient = (TContextScLoginClient*)pSc->GetContext();
  // переместить информацию из mMngContextClientLogining в mMngContextClient
  // либо по сессии 
  unsigned int id_session_client = pContextLoginClient->GetID_SessionClientMaster();
  // удалить из списка авторизующихся
  mMngContextClientLogining->DeleteBySession(id_session_client);
  if(pContextLoginClient->IsReject())
    return;
  // тут если авторизация закончилась удачно
  if(pContextLoginClient->IsAccept())
  {
    // ID Клиента
    unsigned int id_client = pContextLoginClient->GetIDClient();
    // найти сессию Slave
    unsigned int id_session_slave = pContextLoginClient->GetID_SessionMasterSlave();
    TContainerContextSc* pC = mMngContextClient->AddContext(id_client,id_session_slave);
  }
}
//-------------------------------------------------------------------------
//void TMaster::EndLoginClientBySlave(IScenario* pSc)
//{
  //TScenarioLoginClientByMaster1* pLCM1 = (TScenarioLoginClientByMaster1*)pSc;
  //unsigned int id_session = pLCM1->GetID_Session();
  //if(pLCM1->IsReject())
  //{
  //  // удалить 
  //  TClientForMasterPrivate* pClient = FindClientByIDSessionWait(id_session);
  //  if(pClient==NULL)
  //    return;
  //  mMapID_SessionClientWait.erase(id_session);
  //  delete pClient;
  //}
  //else
  //{
  //  TClientForMasterPrivate* pClient = FindClientByIDSession(id_session);
  //  if(pClient==NULL)
  //    return;
  //  pClient->SetState(TClientForMasterPrivate::eReady);
  //  mIDSessionKey.erase(id_session);
  //}
//}
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
		// возможно клиент в очереди, удалим на всякий случай
		mSetClientKeyInQueue->DeleteKey(id_client);
    // закончить с Клиентом на Slave
    mControlSc->mLoginClient->SetContext(&pC->mLoginClient);
    mControlSc->mLoginClient->DisconnectFromMaster();
    // отослать SuperServer информацию о дисконнекте Клиента
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
  TContainerContextSc* pC = mMngContextSlave->FindContextBySession(id_session);
  if(pC==NULL)
    return false;
  // составить список клиентов, которые сидели на Slave
  int countClient = mMngContextSlave->GetCountClientKey(id_session);
  vector<unsigned int> vID_client;  
  for(int i = 0 ; i < countClient ; i++)
  {
    unsigned int id_client;
    mMngContextSlave->GetClientKeyByIndex(id_session, i, id_client);
    vID_client.push_back(id_client);
  }
  BOOST_FOREACH(unsigned int id_client, vID_client)
  {
    mMngContextClient->DeleteByKey(id_client);
    mMngGroup->DeleteClientKey(id_client);
  }

  // отослать SuperServer список клиентов
  mControlSc->mDisClient->DisconnectFromMaster(vID_client);

  // удалить Slave
  mMngContextSlave->DeleteContextBySession(id_session);
  // уведомить о разрыве
  TEventDisconnectDown event;
  event.id_session = id_session;
  AddEventCopy(&event, sizeof(event));
  return true;
}
//-------------------------------------------------------------------------
bool TMaster::EvalCreateGroupNow(list<unsigned int>& l_id_client, unsigned int& id_group)
{
  //TListSlavePrior listSlavePrior;  // составить список по приоритету
  //BOOST_FOREACH(TMapUintSlavePtr::value_type& bit,mMapID_SessionSlave)
  //{
  //  TSlave_master* pSlave = bit.second;
  //  if(pSlave->GetState()==TSlave_master::eConnect)
  //    continue;
  //  int loadFree  = pSlave->GetLoadProcentFreeClient();
  //  int loadGroup = pSlave->GetLoadProcentGroupClient();
  //  TDescChoiseSlave desc;
  //  desc.pSlave = pSlave;
  //  if(pSlave->GetLoadProcent()<eLimitMoreEmpty)
  //    desc.type = TDescChoiseSlave::eEmpty;
  //  else if(pSlave->GetLoadProcent()<eLimitMoreHalf)
  //  {
  //    if( loadFree > loadGroup )
  //      desc.type = TDescChoiseSlave::eFree;
  //    else
  //      desc.type = TDescChoiseSlave::eGroup;
  //  }
  //  else
  //  {
  //    float delFG = float(loadFree)/loadGroup;// соотношения групповых к свободным
  //    if( delFG < 0.5f )// групповых намного меньше
  //      desc.type = TDescChoiseSlave::eMFree; 
  //    else if( delFG > 2.0f)// групповых намного больше
  //      desc.type = TDescChoiseSlave::eMGroup;
  //    else // примерно поровну
  //      desc.type = TDescChoiseSlave::eGroupFree;
  //  }
  //}
  //TSlave_master* pFindSlave = AnalizListSlaveForGroup(listSlavePrior);
  //if(pFindSlave==NULL)
  //  return false;
  //id_group = CreateGroup(pFindSlave,l_id_client);
  return true;
}
//-------------------------------------------------------------------------
//TSlave_master* TMaster::AnalizListSlaveForGroup(TListSlavePrior& l_slave_prior)
//{
//  BOOST_FOREACH(TDescChoiseSlave desc,l_slave_prior)
//  {
//    switch(desc.type)
//    {   
//      case TDescChoiseSlave::eEmpty:
//        break;
//      case TDescChoiseSlave::eFree:
//        break;
//      case TDescChoiseSlave::eMFree:
//        break;
//      case TDescChoiseSlave::eGroup:
//        break;
//      case TDescChoiseSlave::eMGroup:
//        break;
//      case TDescChoiseSlave::eGroupFree:
//        break;
//    }
//  }
//  return NULL;
//}
//-------------------------------------------------------------------------
//unsigned int TMaster::CreateGroup(TSlave_master* pSlave,
//                                  list<unsigned int>& l_id_client)
//{
//  mCurCounterGroup++;
//  set<TClient_master*> setClient;
//  BOOST_FOREACH(unsigned int id, l_id_client)
//  {
//    TClient_master* pClient = FindClientByKey(id);
//    if(pClient)
//    {
//      pClient->SetGroupID(mCurCounterGroup);
//      mControlSc->mRcm->SetContext(&pClient->GetC()->mRcm);
//      mControlSc->mRcm->Start(pSlave->GetID_Session(), pClient);
//      setClient.insert(set<TClient_master*>::value_type(pClient));
//    }
//  }
//  TGroupClient_master* pGroup = new TGroupClient_master;
//  pGroup->SetID(mCurCounterGroup);
//  pGroup->SetClient(setClient);
//  mMapIDGroupClient.insert(TMapUintGroupClientPtr::value_type(mCurCounterGroup,pGroup));
//  return mCurCounterGroup;
//}
//-------------------------------------------------------------------------
void TMaster::EndRcm(IScenario* pSc)
{
  
}
//-------------------------------------------------------------------------
void TMaster::NeedContextLoginSlave(unsigned int id_session)
{
  TContainerContextSc* pC = mMngContextSlave->FindContextBySession(id_session);
  if(pC)
  {
    // внутренняя ошибка
    GetLogger(STR_NAME_MMO_ENGINE)->
      WriteF_time("TMaster::NeedContextLoginSlave() against try authorized.\n");
    return;
  }
  pC = mMngContextSlave->AddContext(id_session);
  mControlSc->mLoginSlave->SetContext(&pC->mLoginSlave);
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

}
//-------------------------------------------------------------------------
void TMaster::NeedContextLoginClientBySession(unsigned int id_session)
{
  // если это повторная авторизация, то Begin по данному контексту клиента
  // вернет false и в сценарии обработка пакета не продолжится
	TContainerContextSc* pC = mMngContextClientLogining->FindContextBySession(id_session);
  if(pC)
  {
    // внутренняя ошибка
    GetLogger(STR_NAME_MMO_ENGINE)->
      WriteF_time("TMaster::NeedContextLoginClientBySession() against try authorized.\n");
  }
  else
  {
		pC = mMngContextClientLogining->AddContext(id_session);
  }
  // назначить контекст
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
      BL_FIX_BUG();
  }
  else
    BL_FIX_BUG();
}
//-------------------------------------------------------------------------
void TMaster::NeedNumInQueueLoginClient(unsigned int id_session)
{

}
//-------------------------------------------------------------------------
void TMaster::NeedContextDisconnectClient(unsigned int id_client)
{
  bool client_is_logining = false;
  unsigned int id_session_client;
  // проверить есть ли вообще такой клиент
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
  // переслать дальше SuperServer
  if(mID_SessionUp!=INVALID_HANDLE_SESSION)
  {
    vector<unsigned int> vecID;
    vecID.push_back(id_client);
    mControlSc->mDisClient->SetContext(&pC->mDisClient);
    pC->mDisClient.SetID_Session(mID_SessionUp);
    mControlSc->mDisClient->DisconnectFromMaster(vecID);
  }
  // удалить из кластера
  if(client_is_logining)
    mMngContextClientLogining->DeleteBySession(id_session_client);
  else
    mMngContextClient->DeleteByKey(id_client);

	TryAddClientFromQueue();
}
//-------------------------------------------------------------------------
void TMaster::EndDisconnectClient(IScenario* pSc)
{

}
//-------------------------------------------------------------------------
void TMaster::TryAddClientFromQueue()
{
	// решить на какой Slave закинуть клиента
	unsigned int id_session_slave;
	unsigned char load_procent;
	if(mMngContextSlave->FindMinimumLoad(id_session_slave,load_procent)==false)
	{
		// генерация ошибки
		GetLogger(STR_NAME_MMO_ENGINE)->
			WriteF_time("TMaster::SetResultLogin() count slave = 0.\n");
		BL_FIX_BUG();
		return;
	}
	if(load_procent<eLimitLoadProcentOnSlaveForAdd)
	{
		unsigned int id_client;
		unsigned int id_session_client;
		TContainerContextSc* pC;
		while(1)
		{
			// ищем до тех пора не закончатся клиента в очереди
			if(mSetClientKeyInQueue->DeleteFirst(id_client)==false)
				return;// пусто
	  	if(mMngContextClientLogining->FindSessionByClientKey(id_client, id_session_client))
			{
				pC = mMngContextClientLogining->FindContextBySession(id_session_client);
				if(pC)
					break;// нашли
			}
		}
		
		void* resForClient = pC->mLoginClient.GetSaveQueueDataPtr();
  	int sizeResClient  = pC->mLoginClient.GetSaveQueueDataSize();
		// начали процесс авторизации
		mMngContextSlave->AddClientKey(id_session_slave, id_client);
		mControlSc->mLoginClient->Accept(id_client,resForClient,sizeResClient,
			id_session_slave,mID_SessionUp);
	}
}
//-------------------------------------------------------------------------
