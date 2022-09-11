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


#ifndef INetControl_H
#define INetControl_H

#include <list>

#include "GCS.h"

#include "BreakPacket.h"
#include "CallBackRegistrator.h"
#include "INetTransport.h"
#include "INetMakerEventThread.h"
#include "TypeEvent.h"

static TCallBackRegistrator1<INetTransport::TDescRecv*> mCallBackRecv;
static TCallBackRegistrator1<TIP_Port*>                 mCallBackDisconnect;

class INetMakerEvent;

class INetControl
{
protected:
  unsigned short mLocalPort; 
  unsigned char  mNumNetWork;

public:
  enum{
    eSystemSizeForRecvBuffer_Socket = 3000000, // байт
    eSystemSizeForSendBuffer_Socket = 3000000, // байт
  };

  INetControl();
  virtual ~INetControl();
  // for INetMakerEvent
  virtual void Work(INetMakerEventThread* pThreadContext, int sock, 
    std::list<nsNetTypeEvent::eTypeEvent>& event) = 0;
  // TNetTransport_XXX
  virtual bool Open( unsigned short port, unsigned char numNetWork = 0) = 0;
  virtual bool Connect(unsigned int ip, unsigned short port) = 0;              // blocking
  virtual void Send(unsigned int ip, unsigned short port, TBreakPacket bp) = 0;

	virtual void Close(unsigned int ip, unsigned short port) = 0;
	virtual void Close(int sock) = 0;

  static void SetMakerEvent(INetMakerEvent* pME);

  template<typename F, class A1>
  static void RegisterReceive(F pFunc, A1 pObject)
  {mCallBackRecv.Register(pFunc,pObject);}
  template<class A1>
  static void UnregisterReceive(A1 pObject)
  {mCallBackRecv.Unregister(pObject);}
  template<typename F, class A1>
  static void RegisterDisconnect(F pFunc, A1 pObject)
  {mCallBackDisconnect.Register(pFunc,pObject);}
  template<class A1>
  static void UnregisterDisconnect(A1 pObject)
  {mCallBackDisconnect.Unregister(pObject);}

protected:
  INetMakerEvent* GetMakerEvent();

  void NotifyRecv(INetTransport::TDescRecv* p);
	void NotifyDisconnect(TIP_Port* p);

  GCS mMutexNotify;
  void lock()  {mMutexNotify.lock();}
  void unlock(){mMutexNotify.unlock();}

};

#endif