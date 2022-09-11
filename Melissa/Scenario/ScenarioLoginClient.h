/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_SCENARIO_LOGIN_CLIENT_H
#define MELISSA_SCENARIO_LOGIN_CLIENT_H

#include "IScenario.h"
#include "ContextScLoginClient.h"
#include "MakerScenario.h"
#include "ShareMisc.h"
#include "CallBackRegistrator.h"

#include "ScLoginClient_ClientImpl.h"
#include "ScLoginClient_MasterImpl.h"
#include "ScLoginClient_SlaveImpl.h"
#include "ScLoginClient_SuperServerImpl.h"

namespace nsMelissa
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
    void RegisterOnNeedContextByKeyClient(F f, C pObject)
    {
      mClient.RegisterOnNeedContextByKeyClient(f,pObject);
      mSlave. RegisterOnNeedContextByKeyClient(f,pObject);
      mMaster.RegisterOnNeedContextByKeyClient(f,pObject);
      mSS.    RegisterOnNeedContextByKeyClient(f,pObject);
    }

    template <typename F, class C>
    void RegisterOnNeedNumInQueue(F f, C pObject)
    {mMaster.RegisterOnNeedNumInQueue(f,pObject);}
  public:
    // от клиента, попытка авторизации
    void TryLogin(unsigned int ip, unsigned short port, 
                  void* data, int size);
    // решение Мастера
    void Reject(void* resForClient, int sizeResClient);
    void Accept(unsigned int key, void* resForClient, int sizeResClient, 
                unsigned int id_session_slave);
    void Queue(int num);

    // решение SuperServer
    void SetResultLoginClient(bool isExist);
  protected:
    virtual void Work();
  };
}
#endif