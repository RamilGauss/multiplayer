/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef SCENARIO_LOGIN_CLIENT_H
#define SCENARIO_LOGIN_CLIENT_H

#include "IScenario.h"
#include "ContextScLoginClient.h"
#include "MakerScenario.h"
#include "ShareMisc.h"
#include "CallBackRegistrator.h"

#include "ScLoginClient_ClientImpl.h"
#include "ScLoginClient_MasterImpl.h"
#include "ScLoginClient_SlaveImpl.h"
#include "ScLoginClient_SuperServerImpl.h"

namespace nsMMOEngine
{
  class TScenarioLoginClient : public IScenario
  {
    TScLoginClient_ClientImpl      mClient;
    TScLoginClient_SlaveImpl       mSlave;
    TScLoginClient_MasterImpl      mMaster;
    TScLoginClient_SuperServerImpl mSS;

    TBaseScLoginClient* mCurBehavior;
  public:
    typedef enum
    {
      eClient,
      eSlave,
      eMaster,
      eSuperServer,
    }eBehavior;

    TScenarioLoginClient();
    virtual ~TScenarioLoginClient();
    
    void SetBehavior(eBehavior v);
    virtual void Recv(TDescRecvSession* pDesc);

    template <typename F, class C>
    void RegisterSetIDClient(F f, C pObject);

    template <typename F, class C>
    void RegisterOnNeedLeaveFromQueue(F f, C pObject);

    template <typename F, class C>
    void RegisterOnNeedIsExistClientID(F f, C pObject);

    template <typename F, class C>
    void RegisterOnNeedContextByKeyClient(F f, C pObject);

    template <typename F, class C>
    void RegisterOnNeedContextIDclientIDmaster(F f, C pObject);

    template <typename F, class C>
    void RegisterOnNeedNumInQueue(F f, C pObject);

  public:
    // от клиента, попытка авторизации
    void TryLogin(unsigned int ip, unsigned short port, 
                  void* data, int size);
    void LeaveQueue();
    // решение Мастера
    void Reject(void* resForClient, int sizeResClient);
    void Accept(unsigned int key, void* resForClient, int sizeResClient, 
                unsigned int id_session_slave, unsigned int id_superserver);
    void Queue(int num);
    
    // решение Slave
    void SetIsExistClientID_slave(bool isExist, unsigned int id_client);

    // решение SuperServer
    void SetIsExistClientID_ss(bool isExist);
  protected:
    virtual void Work();
  };
  //--------------------------------------------------------------------------------
  //--------------------------------------------------------------------------------
  template <typename F, class C>
  void TScenarioLoginClient::RegisterOnNeedLeaveFromQueue(F f, C pObject)
  {
    mMaster.RegisterOnNeedLeaveFromQueue(f,pObject);
  }
  //------------------------------------------------------------
  template <typename F, class C>
  void TScenarioLoginClient::RegisterOnNeedIsExistClientID(F f, C pObject)
  {
    mSlave.RegisterOnNeedIsExistClientID(f,pObject);
    mSS.   RegisterOnNeedIsExistClientID(f,pObject);
  }
  //------------------------------------------------------------
  template <typename F, class C>
  void TScenarioLoginClient::RegisterOnNeedContextByKeyClient(F f, C pObject)
  {
    mSlave. RegisterOnNeedContextByKeyClient(f,pObject);
    mMaster.RegisterOnNeedContextByKeyClient(f,pObject);
  }
  //------------------------------------------------------------
  template <typename F, class C>
  void TScenarioLoginClient::RegisterOnNeedContextIDclientIDmaster(F f, C pObject)
  {
    mSS.RegisterOnNeedContextIDclientIDmaster(f,pObject);
  }
  //------------------------------------------------------------
  template <typename F, class C>
  void TScenarioLoginClient::RegisterOnNeedNumInQueue(F f, C pObject)
  {
    mMaster.RegisterOnNeedNumInQueue(f,pObject);
  }
  //------------------------------------------------------------
  template <typename F, class C>
  void TScenarioLoginClient::RegisterSetIDClient(F f, C pObject)
  {
    mClient.RegisterSetIDClient(f,pObject);
  }
  //------------------------------------------------------------
}
#endif
