/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef BASE_SC_LOGIN_CLIENT_SUPER_SERVER_IMPL_H
#define BASE_SC_LOGIN_CLIENT_SUPER_SERVER_IMPL_H

#include "BaseScLoginClient.h"
#include "CallBackRegistrator.h"

namespace nsMMOEngine
{
  class TScLoginClient_SuperServerImpl : public TBaseScLoginClient
  {
    // запрос SuperServer-у для того что бы он проверил существует ли клиент
    // в системе, а потом вызвал SetIsExistClientID()
    TCallBackRegistrator1<unsigned int> mCallBackNeedIsExistClient;

    enum{
      eTimeWait=20000,
    };

  public:
	  TScLoginClient_SuperServerImpl(IScenario* pSc);

    template <typename F, class C>
    void RegisterOnNeedIsExistClient(F f, C pObject);
    virtual void Work(unsigned int time_ms);

    void SetIsExistClientID(bool isExist);
  protected:
    virtual void RecvInherit(TDescRecvSession* pDesc);
  private:
    void RecvFromMaster(TDescRecvSession* pDesc);

    void RequestM2SS(TDescRecvSession* pDesc);

    void NeedIsExistClient(unsigned int id_client);
  };
  //---------------------------------------------------------------------------
  //---------------------------------------------------------------------------
  template <typename F, class C>
  void TScLoginClient_SuperServerImpl::RegisterOnNeedIsExistClient(F f, C pObject)
  {
    mCallBackNeedIsExistClient.Register(f,pObject);
  }
  //---------------------------------------------------------------------------
}
#endif
