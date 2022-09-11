/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_BASE_SC_LOGIN_CLIENT_MASTER_IMPL_H
#define MELISSA_BASE_SC_LOGIN_CLIENT_MASTER_IMPL_H

#include "BaseScLoginClient.h"

namespace nsMelissa
{
  class TScLoginClient_MasterImpl : public TBaseScLoginClient
  {
  protected:
    // запрос к мастеру о номере клиента в очереди
    TCallBackRegistrator0               mCallBackNeedNumInQueue;
  public:
	  TScLoginClient_MasterImpl(IScenario* pSc);

    template <typename F, class C>
    void RegisterOnNeedNumInQueue(F f, C pObject)
    {mCallBackNeedNumInQueue.Register(f,pObject);}

    virtual void Work(unsigned int time_ms);

    void Reject(void* resForClient, int sizeResClient);
    void Accept(unsigned int key, void* resForClient, int sizeResClient, 
      unsigned int id_session_slave);
    void Queue(int num);

  protected:
    virtual void RecvInherit(TDescRecvSession* pDesc);

    void NeedNumInQueue();
  };
}
#endif
