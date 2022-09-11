/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
√удаков –амиль —ергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef BASE_SC_LOGIN_CLIENT_MASTER_IMPL_H
#define BASE_SC_LOGIN_CLIENT_MASTER_IMPL_H

#include "BaseScLoginClient.h"

namespace nsMMOEngine
{
  class TScLoginClient_MasterImpl : public TBaseScLoginClient
  {
  protected:
    enum{
      eTimeWait = 20000,
    };

    // запрос к мастеру о номере клиента в очереди
    TCallBackRegistrator1<unsigned int> mCallBackNeedNumInQueue;
    // уведомление о выходе из очереди клиента с ID сессии
    TCallBackRegistrator1<unsigned int> mCallBackNeedLeaveFromQueue;
  public:
	  TScLoginClient_MasterImpl(IScenario* pSc);

    template <typename F, class C>
    void RegisterOnNeedNumInQueue(F f, C pObject);
    template <typename F, class C>
    void RegisterOnNeedLeaveFromQueue(F f, C pObject);
    virtual void Work(unsigned int time_ms);

    void Reject(void* resForClient, int sizeResClient);
    void Accept(unsigned int key, void* resForClient, int sizeResClient, 
                unsigned int id_session_slave,unsigned int id_superserver);
    void Queue(int num);

  protected:
    virtual void RecvInherit(TDescRecvSession* pDesc);

    void NeedNumInQueue(unsigned int id_Session);
    void NeedLeaveFromQueue(unsigned int id_session);

  private:
    void RecvFromClient(TDescRecvSession* pDesc);
    void RecvFromSlave(TDescRecvSession* pDesc);
    void RecvFromSuperServer(TDescRecvSession* pDesc);
  
  private:
    void CheckRequestSS2M(TDescRecvSession* pDesc);
    void TryLoginC2M(TDescRecvSession* pDesc);
    void LeaveQueueC2M(TDescRecvSession* pDesc);
    void ClientConnectS2M(TDescRecvSession* pDesc);
    void CheckInfoClientS2M(TDescRecvSession* pDesc);

  private:
    void SendResultAccept2ClientAndSlave(unsigned int key, 
                                         void* resForClient, int sizeResClient);
  };
  //---------------------------------------------------------------------------------
  //---------------------------------------------------------------------------------
  template <typename F, class C>
  void TScLoginClient_MasterImpl::RegisterOnNeedNumInQueue(F f, C pObject)
  {
    mCallBackNeedNumInQueue.Register(f,pObject);
  }
  //---------------------------------------------------------------------------------
  template <typename F, class C>
  void TScLoginClient_MasterImpl::RegisterOnNeedLeaveFromQueue(F f, C pObject)
  {
    mCallBackNeedLeaveFromQueue.Register(f,pObject);
  }
  //---------------------------------------------------------------------------------
}
#endif
