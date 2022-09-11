/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef SCENARIO_RECOMMUTATION_CLIENT_H
#define SCENARIO_RECOMMUTATION_CLIENT_H

#include "IScenario.h"
#include "ContextScRecommutationClient.h"

#include "ScRecommutationClient_ClientImpl.h"
#include "ScRecommutationClient_MasterImpl.h"
#include "ScRecommutationClient_SlaveImpl.h"

namespace nsMMOEngine
{
  class TScenarioRecommutationClient : public IScenario
  {
    // по ключу дать контекст, с учетом Донор или Реципиент
    TCallBackRegistrator2<unsigned int,bool> mCBNeedContextByClientKeyForSlave;
    // на каком Slave находится Клиент? назначить сессию через Context()->SetSessionDonor()
    TCallBackRegistrator1<unsigned int> mCBNeedSessionDonorByClientKey;
    // при вызове DelayBegin или при (Begin()==true)
    // сценарий активировался и начался обмен пакетами
    // это нужно учесть в статистике
    TCallBackRegistrator1<IScenario*>   mCBActivate;
    // если с Донором разорвется соединение, то Мастер сообщит о разрыве связи с Клиентом на Реципиенте
    // и он должен уничтожить информацию о Клиенте
    TCallBackRegistrator1<unsigned int> mCBDisconnectByClientKey;

    TScRecommutationClient_ClientImpl mClient;
		TScRecommutationClient_SlaveImpl  mSlave;// Donor/Recipient
    TScRecommutationClient_MasterImpl mMaster;

    TBaseScRecommutationClient*       mCurBehavior;
  public:
    typedef enum
    {
      eNeedContextByClientKeyForSlave = IScenario::eCountCallBack,
      eNeedSessionDonor,
      eEventActivate,
      eEventDisconnectClient,
    }eTypeCallBack;
  public:
    typedef enum
    {
      eClient,
      eSlave,
      eMaster,
    }eBehavior;

    TScenarioRecommutationClient();
    virtual ~TScenarioRecommutationClient();

    void SetBehavior(eBehavior v);
    virtual void Recv(TDescRecvSession* pDesc);
  public:
    void Start(unsigned int id_session_recipient,
               unsigned int id_client);
    
    void SaveContext(void* data, int size);

    void DisconnectClient();

  protected:
    virtual void Work();
    virtual void DelayBegin();
  };
}
#endif