/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
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
    // ������ � ������� � ������ ������� � �������
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
