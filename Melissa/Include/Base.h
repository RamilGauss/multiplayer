/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_BASE_H
#define MELISSA_BASE_H

#include "TypeDef.h"

#include <map>
#include <set>

#include "SrcEvent.h"
#include "IMakerTransport.h"
#include "BreakPacket.h"
#include "ListMultiThread.h"

#include <boost/smart_ptr/scoped_ptr.hpp>

#define STR_NAME_MELISSA "Melissa"
#define INVALID_HANDLE_SESSION 0

namespace nsMelissa
{
  class TManagerSession;
  class TManagerContextSc;
  class TContainerContextSc;
  class TControlScenario;
  class IScenario;
  struct TDescRecvSession;
  class MELISSA_EI TBase : public TSrcEvent
  {
    typedef std::set<TManagerContextSc*> TSetPtr;
    TSetPtr mSetManagerContextSc;
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

	protected:
    void Recv( TDescRecvSession* pDescRecvSession );
    void Disconnect(unsigned int id);

    virtual void DisconnectInherit(unsigned int id) = 0;
		virtual void WorkInherit(){};
    // события сценариев
    virtual void NeedContextDisconnectClient(unsigned int id_session){}
    virtual void NeedContextDisconnectSlave(unsigned int id_session){}
    virtual void NeedContextLoginClient(unsigned int id_session){}
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
  };
}

#endif