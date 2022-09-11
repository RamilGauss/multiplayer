/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_SCENARIO_RECOMMUTATION_CLIENT_H
#define MELISSA_SCENARIO_RECOMMUTATION_CLIENT_H

#include "IScenario.h"
#include "ContextScRecommutationClient.h"

namespace nsMelissa
{
  class TClientForMasterPrivate;
  class TScenarioRecommutationClient : public IScenario
  {
    TClientForMasterPrivate* mClient;

    typedef std::list<unsigned int> TListUint;
    TListUint mListNextID_SessionSlave;

  public:
    TScenarioRecommutationClient();
    virtual ~TScenarioRecommutationClient();

    virtual void Recv(TDescRecvSession* pDesc);

    void Start(unsigned int new_id_session,TClientForMasterPrivate* pClient);
    
    void SaveContext(void* data, int size);

  protected:
    virtual void Work();
    virtual void DelayBegin();
  protected:
    void SendFirstPacket();

    TContextScRecommutationClient* Context(){return (TContextScRecommutationClient*)mCurContext;}
  };
}
#endif