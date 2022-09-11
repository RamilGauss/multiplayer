/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef SCENARIO_RECOMMUTATION_CLIENT_H
#define SCENARIO_RECOMMUTATION_CLIENT_H

#include "IScenario.h"
#include "ContextScRecommutationClient.h"

#include "ScRecommutationClient_ClientImpl.h"
#include "ScRecommutationClient_MasterImpl.h"
#include "ScRecommutationClient_SlaveImpl.h"

namespace nsMMOEngine
{
  class TScenarioRecommutationClient : public IScenario
  {
    // �� ����� ���� ��������, � ������ ����� ��� ���������
    TCallBackRegistrator2<unsigned int,bool> mCBNeedContextByClientKeyForSlave;
    // �� ����� Slave ��������� ������? ��������� ������ ����� Context()->SetSessionDonor()
    TCallBackRegistrator1<unsigned int> mCBNeedSessionDonorByClientKey;
    // ��� ������ DelayBegin ��� ��� (Begin()==true)
    // �������� ������������� � ������� ����� ��������
    // ��� ����� ������ � ����������
    TCallBackRegistrator1<IScenario*>   mCBActivate;
    // ���� � ������� ���������� ����������, �� ������ ������� � ������� ����� � �������� �� ����������
    // � �� ������ ���������� ���������� � �������
    TCallBackRegistrator1<unsigned int> mCBDisconnectByClientKey;

    TScRecommutationClient_ClientImpl mClient;
		TScRecommutationClient_SlaveImpl  mSlave;// Donor/Recipient
    TScRecommutationClient_MasterImpl mMaster;

    TBaseScRecommutationClient*       mCurBehavior;
  public:
    typedef enum
    {
      eNeedContextByClientKeyForSlave = IScenario::eCountCallBack,
      eNeedSessionDonor,
      eEventActivate,
      eEventDisconnectClient,
    }eTypeCallBack;
  public:
    typedef enum
    {
      eClient,
      eSlave,
      eMaster,
    }eBehavior;

    TScenarioRecommutationClient();
    virtual ~TScenarioRecommutationClient();

    void SetBehavior(eBehavior v);
    virtual void Recv(TDescRecvSession* pDesc);
  public:
    void Start(unsigned int id_session_recipient,
               unsigned int id_client);
    
    void SaveContext(void* data, int size);

    void DisconnectClient();

  protected:
    virtual void Work();
    virtual void DelayBegin();
  };
}
#endif