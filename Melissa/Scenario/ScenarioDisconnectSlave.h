/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_SCENARIO_DISCONNECT_SLAVE_H
#define MELISSA_SCENARIO_DISCONNECT_SLAVE_H

#include "IScenario.h"
#include "ContextScDisconnectSlave.h"
#include "MakerScenario.h"

namespace nsMelissa
{
  class TScenarioDisconnectSlave : public IScenario
  {
    struct THeaderDisconnectSlave : public IScenario::TBaseHeader
    {
      THeaderDisconnectSlave(){type=TMakerScenario::eDisconnectSlave;} 
    };
    //-------------------------------------------------

  public:
    TScenarioDisconnectSlave();

    virtual void Recv(TDescRecvSession* pDesc);

  protected:
    virtual void Work();
  private:
    TContextScDisconnectSlave* Context(){return (TContextScDisconnectSlave*)mCurContext;}
  };
}

#endif