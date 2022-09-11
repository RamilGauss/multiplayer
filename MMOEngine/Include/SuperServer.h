/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef SUPERSERVER_H
#define SUPERSERVER_H

#include "BaseServer.h"
#include <list>
#include <map>

namespace nsMMOEngine
{
  class TMaster_ss;
  class TClient_ss;
  class MMO_ENGINE_EI TSuperServer : public TBaseServer
  {
    // DOWN
    // Master
    typedef std::map<unsigned int,TMaster_ss*> TMapUintMasterPtr;
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
    virtual void SendByClientKey(std::list<unsigned int>& lKey, TBreakPacket bp);

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

    virtual void NeedContextIDclientIDmaster(unsigned int id_client,unsigned int id_session);//SS
    virtual void NeedIsExistClientID(unsigned int id_client);// S,SS

	private:
    TClient_ss* FindClient(unsigned int id_client);
    TClient_ss* AddClient(unsigned int id_client, unsigned int id_session_master);
    void DeleteClient(unsigned int id_client);
    
    TMaster_ss* FindMasterByClient(unsigned int id_client);
    TMaster_ss* FindMaster(unsigned int id_session_master);
    TMaster_ss* AddMaster(unsigned int id_session_master);
    void DeleteMaster(unsigned int id_session_master);

  };
}

#endif