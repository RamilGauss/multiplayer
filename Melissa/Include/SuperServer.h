/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_SUPERSERVER_H
#define MELISSA_SUPERSERVER_H

#include "BaseServer.h"
#include <list>
#include <map>

namespace nsMelissa
{
  class TMasterPrivate;
  class MELISSA_EI TSuperServer : public TBaseServer
  {
    // DOWN
    // Master
    typedef std::map<unsigned int,TMasterPrivate*> TMapUintMasterPtr;
    typedef TMapUintMasterPtr::iterator TMapUintMasterPtrIt;
    TMapUintMasterPtr mMapID_SessionMaster;
    // для поиск мастера по клиенту
    typedef std::map<unsigned int,unsigned int> TMapUintUint;
    typedef TMapUintUint::iterator TMapUintUintIt;
    TMapUintUint mMapIDClientIDSessionMaster;

  public:
    TSuperServer();
    virtual ~TSuperServer();
    
		// BaseServer
    virtual void SendByClientKey(std::list<unsigned int>& l, TBreakPacket bp);

		struct TDescDownSuperServer
		{
			unsigned int id_session;
			int countClient;
		};
		virtual int  GetCountDown();
		virtual bool GetDescDown(int index, void* pDesc, int& sizeDesc);
    virtual void SendDown(unsigned int id_session, TBreakPacket bp, bool check = true);

	protected:
    // Base
    virtual void DisconnectInherit(unsigned int id_session);

    virtual void NeedContextLoginMaster(unsigned int id_session);
    virtual void NeedContextLoginClientByClientKey(unsigned int id_key_client);

	private:
    TMasterPrivate* GetMasterByIDSession(unsigned int id);
  };
}

#endif