/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "SuperServer.h"
#include "MasterPrivate.h"
#include "Logger.h"
#include "IScenario.h"
#include "ContainerContextSc.h"
#include "ControlScenario.h"
#include "Events.h"

#include "ScenarioLoginMaster.h"
#include "ScenarioFlow.h"
#include "ScenarioSendToClient.h"

using namespace nsMelissa;

TSuperServer::TSuperServer()
{

}
//-------------------------------------------------------------------------
TSuperServer::~TSuperServer()
{

}
//-------------------------------------------------------------------------
void TSuperServer::SendByClientKey(std::list<unsigned int>& l, TBreakPacket bp)
{
  BOOST_FOREACH(unsigned int id_client, l)
  {
    // поиск мастера, у которого есть клиенты, ассоциированные с этими ключами
    BOOST_FOREACH(TMapUintMasterPtr::value_type& bit, mMapID_SessionMaster)
    {
      TMasterPrivate* pMaster = bit.second;
      if(pMaster->SearchClient(id_client))
      {
        mControlSc->mSendToClient->SetContext(&pMaster->GetC()->mSendToClient);
        mControlSc->mSendToClient->SendFromSuperServerToMaster(id_client,bp);
        break;
      }
    }
  }
}
//-------------------------------------------------------------------------
void TSuperServer::DisconnectInherit(unsigned int id_session)
{
  TMasterPrivate* pMaster = GetMasterByIDSession(id_session);
  if(pMaster==NULL)
    return;

  mMapID_SessionMaster.erase(id_session);
  delete pMaster;

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
    GetLogger(STR_NAME_MELISSA)->
      WriteF_time("TSuperServer::GetDescDown() size of buffer less then size of structure.\n");
    return false;
  }
  if(index>=GetCountDown())
  {
    GetLogger(STR_NAME_MELISSA)->
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
  TMasterPrivate* pMaster = GetMasterByIDSession(id_session);
  if(pMaster==NULL)
    return;
  mControlSc->mFlow->SetContext(&pMaster->GetC()->mFlow);
  mControlSc->mFlow->SendDown(bp,check);
}
//-------------------------------------------------------------------------
void TSuperServer::NeedContextLoginMaster(unsigned int id_session)
{
  if(GetMasterByIDSession(id_session))
  {
    // внутренняя ошибка
    GetLogger(STR_NAME_MELISSA)->
      WriteF_time("TSuperServer::LoginMaster() against try authorized.\n");
    return;
  }
  TMasterPrivate* pMaster = new TMasterPrivate;
  SetupScForContext(pMaster->GetC());
  pMaster->GetC()->SetID_Session(id_session);
  pMaster->GetC()->SetUserPtr(pMaster);
  mMapID_SessionMaster.insert(TMapUintMasterPtr::value_type(id_session,pMaster));
  // назначить контекст для сценария
  mControlSc->mLoginMaster->SetContext(&pMaster->GetC()->mLoginMaster);
  // событие наружу
  TEventConnectDown event;
  event.id_session = id_session;
  AddEventCopy(&event, sizeof(event));
}
//-------------------------------------------------------------------------
TMasterPrivate* TSuperServer::GetMasterByIDSession(unsigned int id)
{
  TMapUintMasterPtrIt fit = mMapID_SessionMaster.find(id);
  if(mMapID_SessionMaster.end()==fit)
  {
    GetLogger(STR_NAME_MELISSA)->
      WriteF_time("TSuperServer::GetMasterByIDSession() Master id=%u is not founded.\n",id);
    return NULL;
  }
  return fit->second;
}
//-------------------------------------------------------------------------
void TSuperServer::NeedContextLoginClientByClientKey(unsigned int id_key_client)
{

}
//-------------------------------------------------------------------------
