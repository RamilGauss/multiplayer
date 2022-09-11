/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef SCENARIO_SEND_TO_CLIENT_H
#define SCENARIO_SEND_TO_CLIENT_H

#include "IScenario.h"
#include "ContextScSendToClient.h"
#include "MakerScenario.h"

namespace nsMMOEngine
{
  class TScenarioSendToClient : public IScenario
  {
    enum{eFromSuperServerToMaster,eFromMasterToSlave};
    struct THeaderSendToClient : public IScenario::TBaseHeader
    {
      THeaderSendToClient(){type = TMakerScenario::eSendToClient;}
    };
    //-------------------------------------------------
    struct THeaderFromSuperServerToMaster : public THeaderSendToClient
    {
      THeaderFromSuperServerToMaster(){subType = eFromSuperServerToMaster;}
      unsigned int id_client;
    };
    //-------------------------------------------------
    struct THeaderFromMasterToSlave : public THeaderSendToClient
    {
      THeaderFromMasterToSlave(){subType = eFromMasterToSlave;}
      unsigned int id_client;
    };
    //-------------------------------------------------
  public:
    TScenarioSendToClient();
    virtual ~TScenarioSendToClient();
    
    virtual void Recv(TDescRecvSession* pDesc);

    void SendFromSuperServerToMaster(unsigned int id_client, TBreakPacket bp);
  protected:

    void RecvFromSuperServerToMaster(TDescRecvSession* pDesc);

  protected:
    TContextScSendToClient* Context(){return (TContextScSendToClient*)mCurContext;}
  };
}
#endif