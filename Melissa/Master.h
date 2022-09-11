/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012, 2013
===========================================================================
                        Common Information
"TornadoEngine" GPL Source Code

This file is part of the "TornadoEngine" GPL Source Code.

"TornadoEngine" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"TornadoEngine" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "TornadoEngine" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "TornadoEngine" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "TornadoEngine" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 

#ifndef MELISSA_MASTER_H
#define MELISSA_MASTER_H

#include <list>
#include "ActiveServer.h"

namespace nsMelissa
{
  class MELISSA_EI TMaster : public TActiveServer
  {
  public:
    TMaster();
    virtual ~TMaster();
    
    virtual unsigned int TryCreateGroup(std::list<unsigned int>& l);
    virtual void DestroyGroup(unsigned int id_group);
    virtual void LeaveGroup(unsigned int id_client);
    virtual void GetListForGroup(unsigned int id_group, std::list<unsigned int>& l);
    virtual void SetResultLogin(bool res, unsigned int id_session, 
                                unsigned int id_client, 
                                void* resForClient, int sizeResClient);
    virtual unsigned int GetSlaveSessionByGroup(unsigned int id_group);
    virtual void SendToClient(unsigned int id_client, void* data, int size, bool check = true);
   
    // Base
    virtual void Work();
		// BaseServer
		struct TDescDownMaster
		{
			unsigned int id_session;
		};
		virtual int  GetCountDown();
		virtual bool GetDescDown(int index, void* pDesc, int& sizeDesc);

	protected:
    virtual void Disconnect(unsigned int id_session);

    virtual void RecvFromClient(TDescRecvSession* pDesc);
    virtual void RecvFromSlave(TDescRecvSession* pDesc);
    virtual void RecvFromSuperServer(TDescRecvSession* pDesc);
	private:

  };
}

#endif