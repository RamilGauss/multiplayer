/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_SCENARIO_DISCONNECT_CLIENT_H
#define MELISSA_SCENARIO_DISCONNECT_CLIENT_H

#include "IScenario.h"
#include "ContextScDisconnectClient.h"
#include "MakerScenario.h"

namespace nsMelissa
{
  class TScenarioDisconnectClient : public IScenario
  {
    struct THeaderDisconnectClient : public IScenario::TBaseHeader
    {
      THeaderDisconnectClient(){type=TMakerScenario::eDisconnectClient;} 
    };
    //-------------------------------------------------

  public:
    TScenarioDisconnectClient();

    virtual void Recv(TDescRecvSession* pDesc);

  protected:
    virtual void Work();
  private:
    TContextScDisconnectClient* Context(){return (TContextScDisconnectClient*)mCurContext;}
  };
}

#endif