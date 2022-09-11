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

namespace nsMMOEngine
{
  class TContextScLoginClient;
  class TBaseScLoginClient
  {
  protected:
	  IScenario* mScenario;
    // ������ �� �������� �� ����� �������
    TCallBackRegistrator1<unsigned int> mCallBackNeedContextForKeyClient;
    // ������ �� �������� �� ����� �������
    TCallBackRegistrator1<unsigned int> mCallBackNeedIsExistClientID;
    
    TCallBackRegistrator2<unsigned int,unsigned int> mCallBackNeedContextIDclientIDmaster;
  public:

	  TBaseScLoginClient(IScenario* pSc);
    virtual ~TBaseScLoginClient();

    template <typename F, class C>
    void RegisterOnNeedContextByKeyClient(F f, C pObject);

    template <typename F, class C>
    void RegisterOnNeedIsExistClientID(F f, C pObject);

    template <typename F, class C>
    void RegisterOnNeedContextIDclientIDmaster(F f, C pObject);

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
    void SetupContext(nsLoginClientStruct::THeader* pPacket, unsigned int id_session);

    virtual void RecvInherit(TDescRecvSession* pDesc) = 0;
  protected:
    void SetTimeWaitForNow();
  protected:
    // �� Scenario
    bool Begin();
    void End();
  protected:
    // ������ �� ����� ������, ��� ����������������� �������� �������� � ������� SetContext()
    void NeedContext(unsigned int id_session);
    void NeedContextByKeyClient(unsigned int key);
    void NeedIsExistClientID( unsigned int id_client);
    void NeedContextIDclientIDmaster(unsigned int id_client,unsigned int id_session);
  };
  //----------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------
  template <typename F, class C>
  void TBaseScLoginClient::RegisterOnNeedContextByKeyClient(F f, C pObject)
  {
    mCallBackNeedContextForKeyClient.Register(f,pObject);
  }
  //----------------------------------------------------------------------------------
  template <typename F, class C>
  void TBaseScLoginClient::RegisterOnNeedIsExistClientID(F f, C pObject)
  {
    mCallBackNeedIsExistClientID.Register(f,pObject);
  }
  //---------------------------------------------------------------------------------
  template <typename F, class C>
  void TBaseScLoginClient::RegisterOnNeedContextIDclientIDmaster(F f, C pObject)
  {
    mCallBackNeedContextIDclientIDmaster.Register(f,pObject);
  }
  //---------------------------------------------------------------------------

}
#endif
