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
    enum{eFromMasterToSuperServer,};
    struct THeaderDisconnectSlave : public IScenario::TBaseHeader
    {
      THeaderDisconnectSlave(){type=TMakerScenario::eDisconnectSlave;} 
    };
    //-------------------------------------------------
    struct THeaderFromMasterToSuperServer : public IScenario::TBaseHeader
    {
      THeaderFromMasterToSuperServer(){subType=eFromMasterToSuperServer;countIDClient=0;} 
      int countIDClient;// type id is unsigned int 
    };
    //-------------------------------------------------

  public:
    TScenarioDisconnectSlave();

    virtual void Recv(TDescRecvSession* pDesc);

    void SendFromMasterToSuperServer(std::vector<unsigned int>& vID_client);
  protected:

    void RecvFromMaster();
  private:
    TContextScDisconnectSlave* Context(){return (TContextScDisconnectSlave*)mCurContext;}
  };
}

#endif