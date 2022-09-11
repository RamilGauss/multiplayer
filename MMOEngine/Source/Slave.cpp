/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
√удаков –амиль —ергеевич 
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
}
//-------------------------------------------------------------------------
TSlave::~TSlave()
{

}
//-------------------------------------------------------------------------
void TSlave::SaveContext(unsigned int id_session, void* data, int size)
{
  TContainerContextSc* pC = mMngContextClient->FindContextBySession(id_session);
  if(pC) // передать контекст в сценарий
  {
    mControlSc->mRcm->SetContext(&pC->mRcm);
    mControlSc->mRcm->SaveContext(data, size);// это уже вызов функции сценари€
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
  // либо обрыв св€зи с верхним соединением - тогда мы одни, нерабочее состо€ние
  if(id_session==mID_SessionUp)
  {
    flgConnectUp = false;
    DisconnectAllClient();// распустить всех клиентов
    mID_SessionUp = INVALID_HANDLE_SESSION;
    
    TEventDisconnectUp event;
    event.id_session = id_session;
    AddEventCopy(&event, sizeof(event));
    return;
  }
  // либо обрыв с одним из клиентов - уничтожить клиента
  // физический обрыв или сценарий сам оборвал
  unsigned int id;
  if(mMngContextClient->FindKeyBySession(id_session,id)==false)
    return;

  mMngContextClient->DeleteByKey(id);
  
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
void TSlave::ConnectUp(unsigned int ip, unsigned short port)
{
  // если сесси€ жива, то значит либо соединились, либо соедин€емс€
  if(mID_SessionUp!=INVALID_HANDLE_SESSION)
    return;

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
void TSlave::DisconnectAllClient()
{
  unsigned int id;
  // перечисл€ем всех клиентов и узнаем их ключи
  while(mMngContextClient->GetFirstKey(id))
  {
    // по ключу ищем сессию
    unsigned int id_session;
    if(mMngContextClient->FindSessionByKey(id, id_session))
    {
      // закрываем сессию
      mManagerSession->CloseSession(id_session);
      // удал€ем по ключу
      mMngContextClient->DeleteByKey(id);
      // генераци€ событи€ о дисконнекте
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
void TSlave::NeedContextRcm(unsigned int id_session)
{

}
//-------------------------------------------------------------------------
void TSlave::NeedContextSendToClient(unsigned int id_session_client)
{
  // запрос на отправку какому-то клиенту
  TContainerContextSc* pC = mMngContextClient->FindContextBySession(id_session_client);
  if(pC)
    mControlSc->mSendToClient->SetContext(&pC->mSendToClient);
}
//-------------------------------------------------------------------------
void TSlave::SendByClientKey(list<unsigned int>& lKey, TBreakPacket bp)
{

}
//-------------------------------------------------------------------------
void TSlave::NeedContextLoginClientByClientKey(unsigned int id_client)
{
  TContainerContextSc* pC = mMngContextClient->FindContextByKey(id_client);
  if(pC==NULL)
  {
    pC = mMngContextClient->AddContextByKey(id_client);
    SetupScForContext(pC);
  }

  mControlSc->mLoginClient->SetContext(&pC->mLoginClient);
}
//-------------------------------------------------------------------------
void TSlave::NeedIsExistClientID(unsigned int id_client)
{
  bool isExist = false;
  TContainerContextSc* pC = mMngContextClient->FindContextByKey(id_client);
  if(pC)
  {
    isExist = true;
    // заодно добавить описание сессии
    mMngContextClient->AddSessionByKey(id_client,pC->mLoginClient.GetID_SessionClientSlave());
  }
  mControlSc->mLoginClient->SetIsExistClientID_slave(isExist,
                                                     id_client);
}
//-------------------------------------------------------------------------