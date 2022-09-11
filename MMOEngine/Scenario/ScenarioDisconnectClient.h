/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef SCENARIO_DISCONNECT_CLIENT_H
#define SCENARIO_DISCONNECT_CLIENT_H

#include "IScenario.h"
#include "ContextScDisconnectClient.h"
#include "MakerScenario.h"
#include "CallBackRegistrator.h"

#include <vector>

namespace nsMMOEngine
{
  class TScenarioDisconnectClient : public IScenario
  {
    enum{
      eFromMaster,
      eFromSlave,
    };

    struct THeaderDisconnectClient : public IScenario::TBaseHeader
    {
      THeaderDisconnectClient(){type=TMakerScenario::eDisconnectClient;} 
    };
    //-------------------------------------------------
    struct THeaderFromMaster : public THeaderDisconnectClient
    {
      THeaderFromMaster(){subType=eFromMaster;}
      int countID;// ����� ������ unsigned int
    };
    //-------------------------------------------------
    struct THeaderFromSlave : public THeaderDisconnectClient
    {
      THeaderFromSlave(){subType=eFromSlave;}
      unsigned int id_client;
    };
    //-------------------------------------------------
  public:
    TScenarioDisconnectClient();

    virtual void Recv(TDescRecvSession* pDesc);

    void DisconnectFromSlave(unsigned int id_client);
    void DisconnectFromMaster(std::vector<unsigned int>& vecID_client);

  protected:
    virtual void Work();
  private:
    void RecvFromMaster(TDescRecvSession* pDesc);
    void RecvFromSlave(TDescRecvSession* pDesc);

  private:
    TContextScDisconnectClient* Context(){return (TContextScDisconnectClient*)mCurContext;}
  };
}

#endif