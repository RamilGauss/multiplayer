/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef SCENARIO_LOGIN_CLIENT_H
#define SCENARIO_LOGIN_CLIENT_H

#include "IScenario.h"
#include "ContextScLoginClient.h"
#include "MakerScenario.h"
#include "ShareMisc.h"
#include "CallBackRegistrator.h"

#include "ScLoginClient_ClientImpl.h"
#include "ScLoginClient_MasterImpl.h"
#include "ScLoginClient_SlaveImpl.h"
#include "ScLoginClient_SuperServerImpl.h"

namespace nsMMOEngine
{
  class TScenarioLoginClient : public IScenario
  {
    // ������ �������� �� ������ ������� � ����� �������
    TCallBackRegistrator2<unsigned int,unsigned int> mCBContextByMasterSessionByClientKey;
    // ������ � ������� � ������ ������� � �������
    TCallBackRegistrator1<unsigned int>              mCBNumInQueueByClientKey;
    // ��������� � ���������� ������� ����� ��������
    TCallBackRegistrator1<unsigned int>              mCBSetClientKey;
    // ������ �� �������� �� ������ � ����� �������
    TCallBackRegistrator2<unsigned int,unsigned int> mCBContextByClientSessionByClientKey;

    TScLoginClient_ClientImpl      mClient;
    TScLoginClient_SlaveImpl       mSlave;
    TScLoginClient_MasterImpl      mMaster;
    TScLoginClient_SuperServerImpl mSS;

    TBaseScLoginClient* mCurBehavior;

  public:
    typedef enum
    {
      eContextByMasterSessionByClientKey = IScenario::eCountCallBack,
      eNumInQueueByClientKey,
      eSetClientKey,
      eContextByClientSessionByClientKey,
      eCountCallBack,
    }eTypeCallBack;
  public:
    typedef enum
    {
      eClient,
      eSlave,
      eMaster,
      eSuperServer,
    }eBehavior;

    TScenarioLoginClient();
    virtual ~TScenarioLoginClient();
    
    void SetBehavior(eBehavior v);
    virtual void Recv(TDescRecvSession* pDesc);
  public:
    // �� �������, ������� �����������
    void TryLogin(unsigned int ip, unsigned short port, 
                  void* data, int size);
    void LeaveQueue();
    // ������� �������
    void Reject(void* resForClient, int sizeResClient);
    void Accept(unsigned int key, void* resForClient, int sizeResClient, 
                unsigned int id_session_slave, unsigned int id_superserver);
    void Queue(int num,void* resForClient, int sizeResClient);
    
    void DisconnectFromMaster();
    void DisconnectFromClient();

    void SetFakeClient(bool val);
  protected:
    virtual void Work();
  };
}
#endif
