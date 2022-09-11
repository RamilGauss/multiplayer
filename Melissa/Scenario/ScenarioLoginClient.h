/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_SCENARIO_LOGIN_CLIENT_H
#define MELISSA_SCENARIO_LOGIN_CLIENT_H

#include "IScenario.h"
#include "ContextScLoginClient.h"
#include "MakerScenario.h"
#include "ShareMisc.h"
#include "CallBackRegistrator.h"

/*
  Hot Entity Master  - HEM - �������� ������� ������ TMaster.
  Cold Entity Master - CEM - TMaster
  ������� �������� ������ ��������.
  
  ����� ���������� ��� �������� �������� ������� ��� HEM.
    ���� Accept, ���� Reject. � ���� ������� CEM ����� ��� ������ ��� ���� �������.
    
    * ��� Reject �� ������� Reject ��� ��������. ������� ������ ������� � ���������� ��������.
  � ������� ��������� ������� eResultLogin � ������� � ����������� ������.
    
    * ��� Accept CEM ����� ������ �� ����� �� Slave �������� �������, � ������ ���� ��� �����
  �� �������� ������� ����� � �������� ����������� (������� eEnterInQueue). 
  ��������������, �������� ���������� ��� ����������. � � Work �������� ���� �������� 
  �� ��������� ���������. ����� �������� ���������� � ������ ������� � �������.
  ������ ����� ������� ������� NeedQueueNum ��� CEM. 
  
  1. 

*/

namespace nsMelissa
{
  class TScenarioLoginClient : public IScenario
  {
    // ���������� ClientToMaster - C2M
    enum{
      // ������, ��������� ���������
      eTryLoginC2M,
      eRejectM2C,
      eAcceptM2S,
      eQueueM2C,
      eLeaveQueueC2M,
      eCheckLeaveQueueM2C,
      //-------------------------------
      // ������������� ����� ��������
      eCheckAddContentS2M,
      eInfoSlaveM2C,
      eLoginContentC2S,
      eClientLoginS2M,
      eClientInClusterM2SS,
      eCheckClientInClusterSS2M,
      eCheckClientLoginM2S,
      eClientInClusterS2C,
    };
    struct THeader : public IScenario::TBaseHeader
    {
      THeader(){type=TMakerScenario::eLoginClient;key_client=0;}
      unsigned int key_client;
    };
    //-------------------------------------------------
    // ������ ��� �������
    struct THeaderTryLoginC2M : public THeader
    {
      THeaderTryLoginC2M(){subType=eTryLoginC2M;sizeData=0;}
      int sizeData;
    };
    //-------------------------------------------------
    struct THeaderRejectM2C : public THeader
    {
      THeaderRejectM2C(){subType=eRejectM2C;sizeResClient=0;}
      int sizeResClient;
    };
    //-------------------------------------------------
    struct THeaderAcceptM2S : public THeader
    {
      THeaderAcceptM2S(){subType=eAcceptM2S;sizeResClient=0;}
      int sizeResClient;
    };
    //-------------------------------------------------
    struct THeaderQueueM2C : public THeader
    {
      THeaderQueueM2C(){subType=eQueueM2C;numInQueue=0;}
      int numInQueue;
    };
    //-------------------------------------------------
    struct THeaderLeaveC2M : public THeader
    {
      THeaderLeaveC2M(){subType=eLeaveQueueC2M;}
    };
    //-------------------------------------------------
    struct THeaderCheckLeaveC2M : public THeader
    {
      THeaderCheckLeaveC2M(){subType=eCheckLeaveQueueM2C;}
    };
    //-------------------------------------------------
    //-------------------------------------------------
    struct THeaderCheckAddContentS2M : public THeader
    {
      THeaderCheckAddContentS2M(){subType=eCheckAddContentS2M;}
    };
    //-------------------------------------------------
    struct THeaderInfoSlaveM2C : public THeader
    {
      THeaderInfoSlaveM2C(){subType=eInfoSlaveM2C;}
    };
    //-------------------------------------------------
    struct THeaderLoginContentC2S : public THeader
    {
      THeaderLoginContentC2S(){subType=eLoginContentC2S;}
    };
    //-------------------------------------------------
    struct THeaderClientLoginS2M : public THeader
    {
      THeaderClientLoginS2M(){subType=eClientLoginS2M;}
    };
    //-------------------------------------------------
    struct THeaderClientInClusterM2SS : public THeader
    {
      THeaderClientInClusterM2SS(){subType=eClientInClusterM2SS;}
    };
    //-------------------------------------------------
    struct THeaderCheckClientInClusterSS2M : public THeader
    {
      THeaderCheckClientInClusterSS2M(){subType=eCheckClientInClusterSS2M;}
    };
    //-------------------------------------------------
    struct THeaderCheckClientLoginM2S : public THeader
    {
      THeaderCheckClientLoginM2S(){subType=eCheckClientLoginM2S;}
    };
    //-------------------------------------------------
    struct THeaderClientInClusterS2C : public THeader
    {
      THeaderClientInClusterS2C(){subType=eClientInClusterS2C;}
    };
    //-------------------------------------------------
    enum{
      eTimeWait = 20000,// ��
    };
    // ������ �� �������� �� ����� �������
		TCallBackRegistrator1<unsigned int> mCallBackNeedContextForKeyClient;
    // ������ � ������� � ������ ������� � �������
    TCallBackRegistrator0               mCallBackNeedNumInQueue;
  public:
    TScenarioLoginClient();
    virtual ~TScenarioLoginClient();
    
    virtual void Recv(TDescRecvSession* pDesc);

    template <typename F, class C>
    void RegisterOnNeedContextByKeyClient(F f, C pObject)
    {mCallBackNeedContextForKeyClient.Register(f,pObject);}

    template <typename F, class C>
    void RegisterOnNeedNumInQueue(F f, C pObject)
    {mCallBackNeedNumInQueue.Register(f,pObject);}
  public:
    // �� �������, ������� �����������
    void TryLogin(unsigned int ip, unsigned short port, 
                  void* data, int size);
    // ������� �������
    void Reject(void* resForClient, int sizeResClient);
    void Accept(unsigned int key, void* resForClient, int sizeResClient, 
                unsigned int id_session_slave);
    void Queue(int num);
  protected:
    virtual void Work();
    TContextScLoginClient* Context(){return (TContextScLoginClient*)mCurContext;}
  protected:
    // ������� �� ��������� ������, ����������� �� Recv()
    void TryLoginC2M(TDescRecvSession* pDesc);
    void RejectM2C(TDescRecvSession* pDesc);
    void AcceptM2S(TDescRecvSession* pDesc);
    void QueueM2C(TDescRecvSession* pDesc);
    void LeaveQueueC2M(TDescRecvSession* pDesc);
    void CheckLeaveQueueM2C(TDescRecvSession* pDesc);
    void CheckAddContentS2M(TDescRecvSession* pDesc);
    void InfoSlaveM2C(TDescRecvSession* pDesc);
    void LoginContentC2S(TDescRecvSession* pDesc);
    void ClientLoginS2M(TDescRecvSession* pDesc);
    void ClientInClusterM2SS(TDescRecvSession* pDesc);
    void CheckClientInClusterSS2M(TDescRecvSession* pDesc);
    void CheckClientLoginM2S(TDescRecvSession* pDesc);
    void ClientInClusterS2C(TDescRecvSession* pDesc);
  private:
    void SetTimeWaitForNow();

    unsigned int GetID_SessionClientMaster();
    void SetID_SessionClientMaster(unsigned int id);
    unsigned int GetID_SessionClientSlave();
    void SetID_SessionClientSlave(unsigned int id);
    unsigned int GetID_SessionMasterSlave();
    void SetID_SessionMasterSlave(unsigned int id);
    unsigned int GetID_SessionMasterSS();
    void SetID_SessionMasterSS(unsigned int id);
  protected:
    void SetupContext(THeader* pPacket, unsigned int id_session);
    void NeedContextByKeyClient(unsigned int key);
    void NeedNumInQueue();
  };
}
#endif