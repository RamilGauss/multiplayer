/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef BASE_SC_RECOMMUTATION_CLIENT_H
#define BASE_SC_RECOMMUTATION_CLIENT_H

#include "IScenario.h"
#include "DescRecvSession.h"
#include "BaseScRecommutationClient_Struct.h"
#include "DescCallBack.h"
#include "MapCallBack.h"

namespace nsMMOEngine
{
  class TContextScRecommutationClient;
  class TBaseScRecommutationClient
  {
  protected:
	  IScenario* mScenario;
  public:
	  TBaseScRecommutationClient(IScenario* pSc);
    virtual ~TBaseScRecommutationClient();

    void Recv(TDescRecvSession* pDesc);
    virtual void Work(unsigned int time_ms) = 0;

  protected:
    TContextScRecommutationClient* Context();

    unsigned int GetID_SessionClientSlave();
    void SetID_SessionClientSlave(unsigned int id);
    unsigned int GetID_SessionMasterSlave();
    void SetID_SessionMasterSlave(unsigned int id);
  protected:
    virtual void RecvInherit(TDescRecvSession* pDesc) = 0;
  protected:
    void SetTimeWaitForNow();
  protected:
    // от Scenario
    bool Begin();
    void End();
  protected:
    // запрос на новую сессию, кто зарегистрировался выставит контекст с помощью SetContext()
    //void NeedContext(unsigned int id_session);
    //void NeedContextByClientKey(unsigned int id_client);
    //void NeedContextByMasterSessionByClientKey(unsigned int id_session_master,unsigned int id_client);
    //void NeedNumInQueueByClientKey(unsigned int id_client);
    //void EventSetClientKey(unsigned int id_client);
    //void NeedContextByClientSessionByClientKey(unsigned int id_session_client,unsigned int id_client);
  };
}
#endif
