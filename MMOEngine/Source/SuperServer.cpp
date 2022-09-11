/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
√удаков –амиль —ергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "SuperServer.h"
#include "Master_ss.h"
#include "Client_ss.h"

#include "Logger.h"
#include "IScenario.h"
#include "ContainerContextSc.h"
#include "ControlScenario.h"
#include "Events.h"

#include "ScenarioLoginMaster.h"
#include "ScenarioFlow.h"
#include "ScenarioSendToClient.h"
#include "ScenarioLoginClient.h"

using namespace nsMMOEngine;

TSuperServer::TSuperServer()
{
  mControlSc->mLoginClient->SetBehavior(TScenarioLoginClient::eSuperServer);
}
//-------------------------------------------------------------------------
TSuperServer::~TSuperServer()
{
  BOOST_FOREACH(TMapUintMasterPtr::value_type& bit, mMapID_SessionMaster)
    delete bit.second;
  mMapID_SessionMaster.clear();
}
//-------------------------------------------------------------------------
void TSuperServer::SendByClientKey(std::list<unsigned int>& lKey, TBreakPacket bp)
{
  BOOST_FOREACH(unsigned int id_client, lKey)
  {
    // поиск мастера, у которого есть клиенты, ассоциированные с этими ключами
    TMaster_ss* pMaster = FindMasterByClient(id_client);
    mControlSc->mSendToClient->SetContext(&pMaster->GetC()->mSendToClient);
    mControlSc->mSendToClient->SendFromSuperServerToMaster(id_client,bp);
  }
}
//-------------------------------------------------------------------------
void TSuperServer::DisconnectInherit(unsigned int id_session)
{
  TMaster_ss* pMaster = FindMaster(id_session);
  if(pMaster==NULL)
    return;

  DeleteMaster(id_session);

  TEventDisconnectDown event;
  event.id_session = id_session;
  AddEventCopy(&event, sizeof(event));
}
//-------------------------------------------------------------------------
int TSuperServer::GetCountDown()
{
	return mMapID_SessionMaster.size();
}
//-------------------------------------------------------------------------
bool TSuperServer::GetDescDown(int index, void* pDesc, int& sizeDesc)
{
  if(sizeDesc<sizeof(TDescDownSuperServer))
  {
    GetLogger(STR_NAME_MMO_ENGINE)->
      WriteF_time("TSuperServer::GetDescDown() size of buffer less then size of structure.\n");
    return false;
  }
  if(index>=GetCountDown())
  {
    GetLogger(STR_NAME_MMO_ENGINE)->
      WriteF_time("TSuperServer::GetDescDown() index is out of band.\n");
    return false;
  }
  TMapUintMasterPtrIt it = mMapID_SessionMaster.begin();
  for(int i = 0 ; i < index ; i++)
    it++;
  TDescDownSuperServer* pD = (TDescDownSuperServer*)pDesc;
  pD->id_session  = it->second->GetC()->GetID_Session();
  pD->countClient = it->second->GetCountClient();
  sizeDesc = sizeof(TDescDownSuperServer);
  return true;
}
//-------------------------------------------------------------------------
void TSuperServer::SendDown(unsigned int id_session, TBreakPacket bp, bool check)
{
  TMaster_ss* pMaster = FindMaster(id_session);
  if(pMaster==NULL)
    return;
  mControlSc->mFlow->SetContext(&pMaster->GetC()->mFlow);
  mControlSc->mFlow->SendDown(bp,check);
}
//-------------------------------------------------------------------------
void TSuperServer::NeedContextLoginMaster(unsigned int id_session)
{
  if(FindMaster(id_session))
  {
    // внутренн€€ ошибка
    GetLogger(STR_NAME_MMO_ENGINE)->
      WriteF_time("TSuperServer::LoginMaster() against try authorized.\n");
    return;
  }
  TMaster_ss* pMaster = AddMaster(id_session);
  // назначить контекст дл€ сценари€
  mControlSc->mLoginMaster->SetContext(&pMaster->GetC()->mLoginMaster);
  // событие наружу
  TEventConnectDown event;
  event.id_session = id_session;
  AddEventCopy(&event, sizeof(event));
}
//-------------------------------------------------------------------------
void TSuperServer::NeedContextIDclientIDmaster(unsigned int id_client,
                                               unsigned int id_session)//SS
{
  TMaster_ss* pMaster = FindMaster(id_session);
  if(pMaster==NULL)
  {
    BL_FIX_BUG();
    return;
  }

  TClient_ss* pClient = pMaster->FindClient(id_client);
  if(pClient==NULL)
    pClient = AddClient(id_client,id_session);

  mControlSc->mSendToClient->SetContext(&pClient->GetC()->mLoginClient);
}
//-------------------------------------------------------------------------
void TSuperServer::NeedIsExistClientID(unsigned int id_client)
{
  bool isExist = false;
  if(FindClient(id_client))
    isExist = true;

  mControlSc->mLoginClient->SetIsExistClientID_ss(isExist);
}
//-------------------------------------------------------------------------
TClient_ss* TSuperServer::FindClient(unsigned int id_client)
{
  TMaster_ss* pMaster = FindMasterByClient(id_client);
  if(pMaster==NULL)
  {
    // внутренн€€ ошибка
    GetLogger(STR_NAME_MMO_ENGINE)->
      WriteF_time("TSuperServer::FindClient() client not found.\n");
    return NULL;
  }
  return pMaster->FindClient(id_client);
}
//-------------------------------------------------------------------------
TClient_ss* TSuperServer::AddClient(unsigned int id_client, unsigned int id_session_master)
{
  TClient_ss* pClient = FindClient(id_client);
  // проверить есть ли клиент
  if(pClient)
    return pClient;
  // есть ли мастер
  TMaster_ss* pMaster = FindMaster(id_session_master);
  if(pMaster==NULL)
    return NULL;
  // а теперь добавить
  pClient = new TClient_ss;
  pClient->SetID_SessionMaster(id_session_master);
  SetupScForContext(pClient->GetC());
  pClient->GetC()->SetID_Session(id_session_master);
  pClient->GetC()->SetUserPtr(pClient);

  pMaster->AddClient(pClient);

  mMapIDClientIDSessionMaster.insert(TMapUintUint::value_type(id_client,id_session_master));
  return pClient;
}
//-------------------------------------------------------------------------
TMaster_ss* TSuperServer::FindMasterByClient(unsigned int id_client)
{
  TMapUintUintIt fit = mMapIDClientIDSessionMaster.find(id_client);
  if(fit==mMapIDClientIDSessionMaster.end())
  {
    // внутренн€€ ошибка
    GetLogger(STR_NAME_MMO_ENGINE)->
      WriteF_time("TSuperServer::FindMasterByClient() master not found.\n");
    return NULL;
  }

  return FindMaster(fit->second);
}
//-------------------------------------------------------------------------
TMaster_ss* TSuperServer::FindMaster(unsigned int id_session_master)
{
  TMapUintMasterPtrIt fit = mMapID_SessionMaster.find(id_session_master);
  if(fit==mMapID_SessionMaster.end())
  {
    // внутренн€€ ошибка
    GetLogger(STR_NAME_MMO_ENGINE)->
      WriteF_time("TSuperServer::FindMaster() master not found.\n");
    return NULL;
  }
  return fit->second;
}
//-------------------------------------------------------------------------
TMaster_ss* TSuperServer::AddMaster(unsigned int id_session_master)
{
  TMaster_ss* pMaster = FindMaster(id_session_master);
  if(pMaster)
    return pMaster;
  pMaster = new TMaster_ss;

  SetupScForContext(pMaster->GetC());
  pMaster->SetID_Session(id_session_master);
  pMaster->GetC()->SetID_Session(id_session_master);
  pMaster->GetC()->SetUserPtr(pMaster);
  mMapID_SessionMaster.insert(TMapUintMasterPtr::value_type(id_session_master,pMaster));

  return pMaster;
}
//-------------------------------------------------------------------------
void TSuperServer::DeleteMaster(unsigned int id_session_master)
{
  TMaster_ss* pMaster = FindMaster(id_session_master);
  mMapID_SessionMaster.erase(id_session_master);
  delete pMaster;
}
//-------------------------------------------------------------------------
void TSuperServer::DeleteClient(unsigned int id_client)
{
  TMaster_ss* pMaster = FindMasterByClient(id_client);
  if(pMaster==NULL)
    return;
	pMaster->DeleteClient(id_client);
}
//-------------------------------------------------------------------------
