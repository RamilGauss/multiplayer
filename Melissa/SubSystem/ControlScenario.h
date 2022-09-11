/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_CONTROL_SCENARIO_H
#define MELISSA_CONTROL_SCENARIO_H

#include <map>
#include "MakerScenario.h"

namespace nsMelissa
{
	class IScenario;
  class TScenarioDisconnectClient;
  class TScenarioDisconnectSlave;
  class TScenarioFlow;
  class TScenarioLoginClient;
  class TScenarioLoginSlave;
  class TScenarioLoginMaster;
  class TScenarioRecommutationClient;
  class TScenarioSendToClient;
  class TScenarioSynchroSlave;
  struct TDescRecvSession;
  class TControlScenario
  {
    typedef std::map<int,IScenario*> TMapIntPtr;
    typedef TMapIntPtr::iterator TMapIntPtrIt;

    TMapIntPtr mMapTypeSc;

    TMakerScenario mMakerScenario;

  public:
    TControlScenario();
    ~TControlScenario();
    // ��� ��������� ���������� �������
    void Work(TDescRecvSession* pDesc);

    TScenarioDisconnectClient*    mDisClient;
    TScenarioDisconnectSlave*     mDisSlave;
    TScenarioFlow*                mFlow;
    TScenarioLoginClient*         mLoginClient;
    TScenarioLoginSlave*          mLoginSlave;
    TScenarioLoginMaster*         mLoginMaster;
    TScenarioRecommutationClient* mRcm;
    TScenarioSendToClient*        mSendToClient;
    TScenarioSynchroSlave*        mSynchroSlave;
  protected:
    void Add(IScenario* pSc);
  };
}

#endif