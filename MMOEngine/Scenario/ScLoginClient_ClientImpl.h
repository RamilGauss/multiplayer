/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef BASE_SC_LOGIN_CLIENT_CLIENT_IMPL_H
#define BASE_SC_LOGIN_CLIENT_CLIENT_IMPL_H

#include "BaseScLoginClient.h"

namespace nsMMOEngine
{
  class TScLoginClient_ClientImpl : public TBaseScLoginClient
  {
    enum{
      eTimeWait=20000,
    };
    
    // уведомить о назначении клиенту ключа Мастером
    TCallBackRegistrator1<unsigned int> mCallBackSetIDClient;
  public:
	  TScLoginClient_ClientImpl(IScenario* pSc);

    virtual void Work(unsigned int time_ms);

    template <typename F, class C>
    void RegisterSetIDClient(F f, C pObject);

    void TryLogin(unsigned int ip, unsigned short port, void* data, int size);
    void LeaveQueue();
  protected:
    virtual void RecvInherit(TDescRecvSession* pDesc);

  private:
    void RecvFromSlave(TDescRecvSession* pDesc);
    void RecvFromMaster(TDescRecvSession* pDesc);

    void CheckConnectToSlaveS2C(TDescRecvSession* pDesc);
    void ResultLoginM2C(TDescRecvSession* pDesc);
    void CheckLeaveQueueM2C(TDescRecvSession* pDesc);
    void InfoSlaveM2C(TDescRecvSession* pDesc);
  private:
    void CloseSessionMaster();
  private:
    void SetIDClient(unsigned int id_client);
  };
  //------------------------------------------------------------
  template <typename F, class C>
  void TScLoginClient_ClientImpl::RegisterSetIDClient(F f, C pObject)
  {
    mCallBackSetIDClient.Register(f, pObject);
  }
  //------------------------------------------------------------
}
#endif
