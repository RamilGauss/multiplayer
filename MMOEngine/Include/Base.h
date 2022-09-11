/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef BASE_H
#define BASE_H

#include "TypeDef.h"

#include <map>
#include <set>

#include "SrcEvent.h"
#include "IMakerTransport.h"
#include "BreakPacket.h"
#include "ListMultiThread.h"

#include <boost/smart_ptr/scoped_ptr.hpp>

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
  struct TDescRecvSession;
  class MMO_ENGINE_EI TBase : public TSrcEvent
  {
    boost::scoped_ptr<TManagerManagerContextSc> mMgrMgrContextSc;
  protected:
    boost::scoped_ptr<TControlScenario>    mControlSc;
    boost::scoped_ptr<TContainerContextSc> mContainerUp;

		// уведомления о разрыве связи
    typedef TListMultiThread<unsigned int> TListUint;
    TListUint mIDSessionDisconnect;
		// полученные пакеты от mManagerSession
    typedef TListMultiThread<TDescRecvSession> TListRecvPacket;
    TListRecvPacket mRecvPacket;

		// транспорт
    boost::scoped_ptr<TManagerSession> mManagerSession;
		// загрузка CPU
    int mLoadProcent;// затраченное время/выделенное, %

    unsigned int mID_SessionUp;
    bool         flgConnectUp;
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
      eCreateGroup,
      eLeaveGroup, 
      eDestroyGroup,
      eEnterInQueue,
      eLeaveQueue,
    }tTypeEvent;

    TBase();
    virtual ~TBase();

    virtual void Init(IMakerTransport* pMakerTransport);
    virtual bool Open(unsigned short port, unsigned char subNet = 0);
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
    // события сценариев
    virtual void NeedContextDisconnectClient(unsigned int id_session){}
    virtual void NeedContextDisconnectSlave(unsigned int id_session){}
    //----------------------------------------------------
    virtual void NeedContextIDclientIDmaster(unsigned int id_client,unsigned int id_session){}//SS
    virtual void NeedContextLoginClient(unsigned int id_session){}// S,M
    virtual void NeedContextLoginClientByClientKey(unsigned int id_key_client){}//S,M,SS
    virtual void NeedIsExistClientID(unsigned int id_client){}// S,SS
    virtual void NeedLeaveFromQueue(unsigned int id_session){}// M
    virtual void NeedNumInQueue(unsigned int id_session){}// M
    virtual void SetIDClient(unsigned int id_client){};//C
    //----------------------------------------------------
    virtual void NeedContextLoginSlave(unsigned int id_session){}
    virtual void NeedContextLoginMaster(unsigned int id_session){}
    virtual void NeedContextRcm(unsigned int id_session){}
    virtual void NeedContextSendToClient(unsigned int id_session){}
    virtual void NeedContextSynchroSlave(unsigned int id_session){}

    virtual void EndDisconnectClient(IScenario*){}
    virtual void EndDisconnectSlave(IScenario*){}
    virtual void EndLoginClient(IScenario*){}
    virtual void EndLoginSlave(IScenario*){}
    virtual void EndLoginMaster(IScenario*){}
    virtual void EndRcm(IScenario*){}
    virtual void EndSynchroSlave(IScenario*){}
    
  protected:
    TManagerContextSc* AddManagerContextSc();
    void RemoveManagerContextSc(TManagerContextSc* pMSc);
    void SetupScForContext(TContainerContextSc* pCCSc);
	private:
    void HandleListDisconnect();
    void HandleListRecv();
  private:
    void RegisterNeedForLoginClient();
  };
}

#endif