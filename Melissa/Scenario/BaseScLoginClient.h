/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_BASE_SC_LOGIN_CLIENT_H
#define MELISSA_BASE_SC_LOGIN_CLIENT_H

#include "IScenario.h"
#include "DescRecvSession.h"
#include "BaseScLoginClient_Struct.h"

namespace nsMelissa
{
  class TContextScLoginClient;
  class TBaseScLoginClient
  {
  protected:
	  IScenario* mScenario;
    // запрос на контекст по ключу клиента
    TCallBackRegistrator1<unsigned int> mCallBackNeedContextForKeyClient;

  public:

	  TBaseScLoginClient(IScenario* pSc);
    virtual ~TBaseScLoginClient();

    template <typename F, class C>
    void RegisterOnNeedContextByKeyClient(F f, C pObject)
    {mCallBackNeedContextForKeyClient.Register(f,pObject);}

    void Recv(TDescRecvSession* pDesc);
    
    virtual void Work(unsigned int time_ms) = 0;

  protected:
    TContextScLoginClient* Context();

    unsigned int GetID_SessionClientMaster();
    void SetID_SessionClientMaster(unsigned int id);
    unsigned int GetID_SessionClientSlave();
    void SetID_SessionClientSlave(unsigned int id);
    unsigned int GetID_SessionMasterSlave();
    void SetID_SessionMasterSlave(unsigned int id);
    unsigned int GetID_SessionMasterSS();
    void SetID_SessionMasterSS(unsigned int id);

    void SetupContext(nsLoginClientStruct::THeader* pPacket, unsigned int id_session);

    void NeedContextByKeyClient(unsigned int key);

    virtual void RecvInherit(TDescRecvSession* pDesc) = 0;

    // от Scenario
    bool Begin();
    void End();
    // запрос на новую сессию, кто зарегистрировался выставит контекст с помощью SetContext()
    void NeedContext(unsigned int id_session);

  };

}
#endif
