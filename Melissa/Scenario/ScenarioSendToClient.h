/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_SCENARIO_SEND_TO_CLIENT_H
#define MELISSA_SCENARIO_SEND_TO_CLIENT_H

#include "IScenario.h"
#include "ContextScSendToClient.h"

namespace nsMelissa
{
  class TScenarioSendToClient : public IScenario
  {
  public:
    TScenarioSendToClient();
    virtual ~TScenarioSendToClient();
    
    virtual void Recv(TDescRecvSession* pDesc);
  protected:

    TContextScSendToClient* Context(){return (TContextScSendToClient*)mCurContext;}
  };
}
#endif