/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_BASE_SC_LOGIN_CLIENT_SUPER_SERVER_IMPL_H
#define MELISSA_BASE_SC_LOGIN_CLIENT_SUPER_SERVER_IMPL_H

#include "BaseScLoginClient.h"

namespace nsMelissa
{
  class TScLoginClient_SuperServerImpl : public TBaseScLoginClient
  {
  public:
	  TScLoginClient_SuperServerImpl(IScenario* pSc);

    virtual void Work(unsigned int time_ms);

    void SetResultLoginClient(bool isExist);
  protected:
    virtual void RecvInherit(TDescRecvSession* pDesc);
  private:
    void RecvFromMaster(TDescRecvSession* pDesc);
  };
}
#endif
