/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef BASE_H
#define BASE_H

#include "TypeDef.h"
#include "Structs.h"

#include "SrcEvent.h"
#include "IMakerTransport.h"
#include "BreakPacket.h"
#include "ListMultiThread.h"

#include <boost/smart_ptr/scoped_ptr.hpp>
#include <list>

#define STR_NAME_MMO_ENGINE "MMOEngine"
#define INVALID_HANDLE_SESSION 0

namespace nsMMOEngine
{
  class TManagerSession;
  class TManagerContextSc;
  class TManagerManagerContextSc;
  class TContainerContextSc;
  class TControlScenario;
  class IScenario;
	class TDelegateManagerContextSc;
	struct TDescRecvSession;
  class MMO_ENGINE_EI TBase : public TSrcEvent
  {
    boost::scoped_ptr<TManagerManagerContextSc> mMngMngContextSc;
  protected:
    boost::scoped_ptr<TControlScenario>    mControlSc;
    boost::scoped_ptr<TContainerContextSc> mContainerUp;

		// ����������� � ������� �����
    typedef TListMultiThread<unsigned int> TListUint;
    TListUint mIDSessionDisconnect;
		// ���������� ������ �� mManagerSession
    typedef TListMultiThread<TDescRecvSession> TListRecvPacket;
    TListRecvPacket mRecvPacket;

		// ���������
    boost::scoped_ptr<TManagerSession> mManagerSession;
		// �������� CPU
    int mLoadProcent;// ����������� �����/����������, %

    unsigned int mID_SessionUp;
    bool         flgConnectUp;

    typedef std::list<TContainerContextSc*> TListPtr;

    TListPtr mListDelayDeleteContainerSc;
  public:
    typedef enum
    {
      eConnectDown,
      eDisconnectDown,
      eConnectUp,
      eDisconnectUp,
      eError,
      eRecvFromDown,
      eRecvFromUp,
      eSaveContext,
      eRestoreContext,
      eTryLogin, 
      eResultLogin,
      eDestroyGroup,
      eEnterInQueue,
    }tTypeEvent;

    TBase();
    virtual ~TBase();

    virtual void Init(IMakerTransport* pMakerTransport);
    virtual bool Open(TDescOpen* pDesc, int count = 1);
    virtual void DisconnectUp();
    virtual void SendUp(TBreakPacket bp, bool check = true);
    void Work();
    virtual bool IsConnectUp();
    virtual bool IsConnect(unsigned int id);
		virtual void SetLoad(int procent);

    virtual void SetTimeLiveSession(unsigned int time_ms);

    bool GetInfoSession(unsigned int id_session, TIP_Port& ip_port);
	protected:
    void Recv( TDescRecvSession* pDescRecvSession );
    void Disconnect(unsigned int id);

    virtual void DisconnectInherit(unsigned int id) = 0;
		virtual void WorkInherit(){};
    // ������� ���������
    virtual void NeedContextDisconnectClient(unsigned int id_client){}
    //----------------------------------------------------
    // LoginClient
    virtual void NeedContextByMasterSessionByClientKey(unsigned int id_session,
                                                       unsigned int id_client){}//SS
    virtual void NeedContextLoginClientBySession(unsigned int id_session){}// S,M
    virtual void NeedContextLoginClientByClientKey(unsigned int id_client){}//S,M,SS
    virtual void NeedNumInQueueLoginClient(unsigned int id_session){}// M
    virtual void EventSetClientKeyLoginClient(unsigned int id_client){};//C
    virtual void NeedContextLoginClientByClientSessionByKeyClient(unsigned int id_session_client,
                                                                  unsigned int id_client){}//S
    //----------------------------------------------------
    // RCM
    virtual void ActivateRcmClient(IScenario* pSc){}//M
    virtual void NeedSlaveSessionDonor(IScenario* pSc){}//M
    //----------------------------------------------------
    virtual void NeedContextLoginSlave(unsigned int id_session){}
    virtual void NeedContextLoginMaster(unsigned int id_session){}
    virtual void NeedContextRcm(unsigned int id_session){}
    virtual void NeedContextSendToClient(unsigned int id_client){}
    virtual void NeedContextSynchroSlave(unsigned int id_session){}

    virtual void EndDisconnectClient(IScenario*){}
    virtual void EndDisconnectSlave(IScenario*){}
    virtual void EndLoginClient(IScenario*){}
    virtual void EndLoginSlave(IScenario*){}
    virtual void EndLoginMaster(IScenario*){}
    virtual void EndRcm(IScenario*){}
    virtual void EndSynchroSlave(IScenario*){}
	private:    
		TManagerContextSc* AddManagerContextSc();
		void RemoveManagerContextSc(TManagerContextSc* pMSc);
	protected:
		friend class TDelegateManagerContextSc;
    void SetupScForContext(TContainerContextSc* pCCSc);
    void DelayDeleteContainerScenario(TContainerContextSc* pCCSc);
	private:
    void HandleListDisconnect();
    void HandleListRecv();

    void DeleteContainerScenario();
  private:
    void SetDefualtContextForScenario();
    void RegisterOnScenarioEvent();
    void RegisterNeedForLoginClient();
    void RegisterNeedForRcmClient();
  };
}

#endif