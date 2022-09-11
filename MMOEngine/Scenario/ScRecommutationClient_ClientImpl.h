/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef SC_RECOMMUTATION_CLIENT_CLIENT_IMPL_H
#define SC_RECOMMUTATION_CLIENT_CLIENT_IMPL_H

#include "BaseScRecommutationClient.h"

namespace nsMMOEngine
{
  class TScRecommutationClient_ClientImpl : public TBaseScRecommutationClient
  {
  protected:
    //enum{
    //  eTimeWait = 20000,
    //  eDeltaTimeNumInQueue = 1000,
    //};

  public:
	  TScRecommutationClient_ClientImpl(IScenario* pSc);

    virtual void Work(unsigned int time_ms);

  protected:
    virtual void RecvInherit(TDescRecvSession* pDesc);

  private:
    void RecvFromSlaveDonor(TDescRecvSession* pDesc);
    void RecvFromSlaveRecipient(TDescRecvSession* pDesc);
  private:
  };
}
#endif
