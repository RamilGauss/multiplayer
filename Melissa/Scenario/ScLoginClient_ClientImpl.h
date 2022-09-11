/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_BASE_SC_LOGIN_CLIENT_CLIENT_IMPL_H
#define MELISSA_BASE_SC_LOGIN_CLIENT_CLIENT_IMPL_H

#include "BaseScLoginClient.h"

namespace nsMelissa
{
  class TScLoginClient_ClientImpl : public TBaseScLoginClient
  {
    enum{
      eTimeWait=20000,
    };
  public:
	  TScLoginClient_ClientImpl(IScenario* pSc);

    virtual void Work(unsigned int time_ms);

    void TryLogin(unsigned int ip, unsigned short port, void* data, int size);
  protected:
    virtual void RecvInherit(TDescRecvSession* pDesc);

  private:
    void SetTimeWaitForNow();

    void RecvFromSlave(TDescRecvSession* pDesc);
    void RecvFromMaster(TDescRecvSession* pDesc);

    void CheckConnectToSlaveS2C(TDescRecvSession* pDesc);
    void ResultLoginM2C(TDescRecvSession* pDesc);
    void CheckLeaveQueueM2C(TDescRecvSession* pDesc);
    void InfoSlaveM2C(TDescRecvSession* pDesc);
  };
}
#endif
