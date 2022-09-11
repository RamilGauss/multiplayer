/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_SLAVE_H
#define MELISSA_SLAVE_H

#include "ActiveServer.h"

#include <map>

namespace nsMelissa
{
  class IScenario;
  class TClientPrivate;
  class MELISSA_EI TSlave : public TActiveServer
  {

    enum{eDeltaSynchro = 3000,// мс
    };

    unsigned int mTimeNeedSendSynchro;

    typedef std::map<unsigned int, TClientPrivate*> TMapUintPtr;
    typedef TMapUintPtr::iterator TMapUintPtrIt;
    TMapUintPtr mMapKeyClient;
    TMapUintPtr mMapID_SessionClient;

  public:
    TSlave();
    virtual ~TSlave();
    
    virtual void SaveContext(unsigned int id_session, void* data, int size);
    virtual unsigned int GetClientKeyBySession(unsigned int id_session);
    virtual unsigned int GetSessionByClientKey(unsigned int key);

		// BaseServer
    virtual void SendByClientKey(std::list<unsigned int>& l, TBreakPacket bp);

		struct TDescDownSlave // для GetDescDown
		{
			unsigned int id_session;
		};
		virtual int  GetCountDown();
		virtual bool GetDescDown(int index, void* pDesc, int& sizeDesc);// pDesc имеет тип TDescDownSlave
    virtual void SendDown(unsigned int id_session, TBreakPacket bp, bool check = true);
    // ActiveServer      
    virtual void ConnectUp(unsigned int ip, unsigned short port);
  protected:
    // Base
		virtual void WorkInherit();
    virtual void DisconnectInherit(unsigned int id_session);
    
	protected:
    virtual void NeedContextLoginClient(unsigned int id_session);
    virtual void NeedContextRcm(unsigned int id_session);
    virtual void NeedContextSendToClient(unsigned int id_session);

    virtual void EndLoginClient(IScenario*);
    virtual void EndLoginSlave(IScenario*);
    virtual void EndRcm(IScenario*);
  private:
    TClientPrivate* GetClientByUint(TMapUintPtr& m, unsigned int k);
    TClientPrivate* GetClientByKey(unsigned int key);
    TClientPrivate* GetClientByID_Session(unsigned int id_session);
    TClientPrivate* AddClientPrivate( unsigned int key, 
                                      unsigned int id_session = INVALID_HANDLE_SESSION);
    void RemoveClientPrivate(unsigned int id_session);
    void ClearClientPrivate();
  };
}

#endif
