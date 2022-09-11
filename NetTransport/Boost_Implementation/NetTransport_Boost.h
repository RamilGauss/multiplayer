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


#ifndef NetTransport_BoostH
#define NetTransport_BoostH

#include "INetTransport.h"
#include "GCS.h"
#include "NetWorkThread.h"

#include "NetControlTCP.h"
#include "NetControlAcceptor.h"
#include "NetControlUDP.h"

// Thread safe - Send поддерживает.

class INetControl;

class TNetTransport_Boost : public INetTransport
{
  GCS mMutexSend;
  GCS mMutexMapIP_TCP;

  TNetWorkThread mNetWorkThread;

  TNetControlUDP      mUDP;
  TNetControlAcceptor mAcceptor;
  TNetControlTCP*     pTCP_Up;

  typedef std::map<TIP_Port,TNetControlTCP*> TMapIP_Ptr;
  typedef TMapIP_Ptr::iterator TMapIP_PtrIt;
  
  TMapIP_Ptr mMapIP_TCP;

public:
	TNetTransport_Boost();
	virtual ~TNetTransport_Boost();

  virtual bool Open(unsigned short port, unsigned char numNetWork = 0);

	virtual void Send(unsigned int ip, unsigned short port, 
                    TBreakPacket packet, bool check = true);

  virtual TCallBackRegistrator1<TDescRecv*>* GetCallbackRecv(){return INetControl::GetCallbackRecv();}
  virtual TCallBackRegistrator1<TIP_Port* >* GetCallbackDisconnect(){return INetControl::GetCallbackDisconnect();}

	virtual void Start();
	virtual void Stop();
	virtual bool IsActive();

  // синхронная функция
  virtual bool Connect(unsigned int ip, unsigned short port); // вызов только для клиента

	virtual void Close(unsigned int ip, unsigned short port);

public:

  void AddInMapTCP(TIP_Port& ip_port, TNetControlTCP* pNetControl);
	void RemoveFromMapTCP(TIP_Port* ip_port, TNetControlTCP* pControl);

protected:
  void CloseAll();
  void DeleteMapControlTCP();

  TNetControlTCP* GetTCP_ByIP(TIP_Port &ip_port);
  void Done();
};


#endif
