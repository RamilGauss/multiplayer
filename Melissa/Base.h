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

#ifndef MELISSA_BASE_H
#define MELISSA_BASE_H

#include "SrcEvent.h"
#include "TypeDef.h"
#include "IMakerTransport.h"
#include "BreakPacket.h"
#include "ListMultiThread.h"
#include <map>

#define STR_NAME_MELISSA "Melissa"
#define INVALID_HANDLE_SESSION 0

namespace nsMelissa
{
  class TManagerSession;
  class TManagerScenario;
  struct TDescRecvSession;
  class MELISSA_EI TBase : public TSrcEvent
  {
  protected:
    struct TStateConnect
    {
      //std::list<TPacket*> listWaitPacket;
      bool flg;
    };

    typedef std::map<unsigned int,TStateConnect*> TMapUintPtr;
    typedef TMapUintPtr::iterator TMapUintPtrIt;
    TMapUintPtr mMapStateConnect;

    typedef TListMultiThread<unsigned int> TListUint;
    TListUint mIDSessionNeedDisconnect;

    TManagerSession* mManagerSession;
    TManagerScenario* mManagerScenario;

    int mLoadProcent;
  public:
    typedef enum
    {
      eAccept,
      eReject,
      eQueue,
    }tResultLogin;
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

    virtual void WorkInherit() = 0;

    void Recv( TDescRecvSession* pDescRecvSession );
    
    virtual void RecvFromClient(TDescRecvSession* pDesc);
    virtual void RecvFromSlave(TDescRecvSession* pDesc);
    virtual void RecvFromMaster(TDescRecvSession* pDesc);
    virtual void RecvFromSuperServer(TDescRecvSession* pDesc);

    virtual void Disconnect(unsigned int id);
	private:

  };
}

#endif