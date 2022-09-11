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
  class MELISSA_EI TSuperServer : public TBaseServer
  {
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
    virtual void SendDown(unsigned int id_session, TBreakPacket bp, bool check);

	protected:
    // Base
    virtual void DisconnectInherit(unsigned int id_session);

	private:

  };
}

#endif