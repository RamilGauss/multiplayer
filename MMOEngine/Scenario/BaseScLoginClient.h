/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef BASE_SC_LOGIN_CLIENT_H
#define BASE_SC_LOGIN_CLIENT_H

#include "IScenario.h"
#include "DescRecvSession.h"
#include "BaseScLoginClient_Struct.h"
#include "DescCallBack.h"
#include "MapCallBack.h"

namespace nsMMOEngine
{
  class TContextScLoginClient;
  class TBaseScLoginClient
  {
  protected:
	  IScenario* mScenario;
  public:
	  TBaseScLoginClient(IScenario* pSc);
    virtual ~TBaseScLoginClient();

    void Recv(TDescRecvSession* pDesc);
    virtual void Work(unsigned int time_ms) = 0;

  protected:
    TContextScLoginClient* Context();

    unsigned int GetID_SessionClientMaster();
    void SetID_SessionClientMaster(unsigned int id);
    unsigned int GetID_SessionClientSlave();
    void SetID_SessionClientSlave(unsigned int id);
    unsigned int GetID_SessionMasterSlave();
    void SetID_SessionMasterSlave(unsigned int id);
    unsigned int GetID_SessionMasterSS();
    void SetID_SessionMasterSS(unsigned int id);
  protected:
    virtual void RecvInherit(TDescRecvSession* pDesc) = 0;
  protected:
    void SetTimeWaitForNow();
  protected:
    // �� Scenario
    bool Begin();
    void End();
  protected:
    // ������ �� ����� ������, ��� ����������������� �������� �������� � ������� SetContext()
    void NeedContextBySession(unsigned int id_session);
    void NeedContextByClientKey(unsigned int id_client);
    void NeedContextByMasterSessionByClientKey(unsigned int id_session_master,unsigned int id_client);
    void NeedNumInQueueByClientKey(unsigned int id_client);
    void EventSetClientKey(unsigned int id_client);
    void NeedContextByClientSessionByClientKey(unsigned int id_session_client,unsigned int id_client);
  };
}
#endif
