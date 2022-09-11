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

#include "TypeDef.h"

#include <map>
#include <set>

#include "SrcEvent.h"
#include "IMakerTransport.h"
#include "BreakPacket.h"
#include "ListMultiThread.h"

#define STR_NAME_MELISSA "Melissa"
#define INVALID_HANDLE_SESSION 0

namespace nsMelissa
{
  class TManagerSession;
  class TManagerScenario;
  class TScenarioSendUp;
  struct TDescRecvSession;
  class MELISSA_EI TBase : public TSrcEvent
  {
    typedef std::set<TManagerScenario*> TSetPtr;
    TSetPtr mSetManagerScenario;

  protected:
    typedef TListMultiThread<unsigned int> TListUint;
    TListUint mIDSessionDisconnect;

    typedef TListMultiThread<TDescRecvSession> TListRecvPacket;
    TListRecvPacket mRecvPacket;

    TManagerSession* mManagerSession;

    //TScenarioSendUp* mScenarioSendUp;
    //std::string sNameSendUp;

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

    void Recv( TDescRecvSession* pDescRecvSession );
    void Disconnect(unsigned int id);

    virtual void RecvFromClient(TDescRecvSession* pDesc);
    virtual void RecvFromSlave(TDescRecvSession* pDesc);
    virtual void RecvFromMaster(TDescRecvSession* pDesc);
    virtual void RecvFromSuperServer(TDescRecvSession* pDesc);

    virtual void DisconnectInherit(unsigned int id) = 0;
		virtual void WorkInherit(){};

    TManagerScenario* AddManagerScenario();
    void RemoveManagerScenario(TManagerScenario* pMSc);
	private:
    void HandleListDisconnect();
    void HandleListRecv();
  };
}

#endif