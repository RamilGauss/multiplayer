/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_SCENARIO_SYNCHRO_SLAVE_H
#define MELISSA_SCENARIO_SYNCHRO_SLAVE_H

#include "IScenario.h"
#include "ContextScSynchroSlave.h"
#include "MakerScenario.h"

namespace nsMelissa
{
  class TScenarioSynchroSlave : public IScenario
  {
    enum{eFromSlave,};
    struct THeaderSynchroSlave : public IScenario::TBaseHeader
    {
      unsigned char loadProcent;
      THeaderSynchroSlave(){type=TMakerScenario::eSynchroSlave;subType=eFromSlave;}
    };
    //-------------------------------------------------
  public:
    TScenarioSynchroSlave();
    virtual ~TScenarioSynchroSlave();
    virtual void Recv(TDescRecvSession* pDesc);
    
    void SendSynchro(int loadProcent);
  protected:
    void RecvFromSlave(THeaderSynchroSlave* pPacket);

    TContextScSynchroSlave* Context(){return (TContextScSynchroSlave*)mCurContext;}
  };
}
#endif