/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
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
      eDeltaTimeNumInQueue = 1000,
    };

  public:
	  TScLoginClient_MasterImpl(IScenario* pSc);

    virtual void Work(unsigned int time_ms);

    void Reject(void* resForClient, int sizeResClient);
    void Accept(unsigned int key, void* resForClient, int sizeResClient, 
                unsigned int id_session_slave,unsigned int id_superserver);
    void Queue(int num, void* resForClient, int sizeResClient);

    void Disconnect();

  protected:
    virtual void RecvInherit(TDescRecvSession* pDesc);

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
    void CheckInfoSlaveC2M(TDescRecvSession* pDesc);

  private:
    void SendResultAccept2ClientAndSlave(unsigned int key, 
                                         void* resForClient, int sizeResClient);
  };
  //---------------------------------------------------------------------------------
  //---------------------------------------------------------------------------------
  //template <typename F, class C>
  //void TScLoginClient_MasterImpl::RegisterOnNeedNumInQueue(F f, C pObject)
  //{
  //  mCallBackNeedNumInQueue.Register(f,pObject);
  //}
  ////---------------------------------------------------------------------------------
  //template <typename F, class C>
  //void TScLoginClient_MasterImpl::RegisterOnNeedLeaveFromQueue(F f, C pObject)
  //{
  //  mCallBackNeedLeaveFromQueue.Register(f,pObject);
  //}
  ////---------------------------------------------------------------------------------
}
#endif
