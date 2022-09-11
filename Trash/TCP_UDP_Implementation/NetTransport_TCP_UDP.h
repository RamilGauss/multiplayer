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


#ifndef NetTransport_TCP_UDPH
#define NetTransport_TCP_UDPH

#include "INetTransport.h"
#include "GCS.h"

// Thread safe - Send поддерживает.

class INetMakerEvent;
class INetControl;

class TNetTransport_TCP_UDP : public INetTransport
{
  INetMakerEvent* mNetMakerEvent;

  INetControl* mTCP;
  INetControl* mUDP;

  GCS mMutex;
  void lock()  {mMutex.lock();}
  void unlock(){mMutex.unlock();}

public:
	TNetTransport_TCP_UDP();
	virtual ~TNetTransport_TCP_UDP();

  virtual bool Open(unsigned short port, unsigned char numNetWork = 0);

	virtual void Send(unsigned int ip, unsigned short port, 
                    TBreakPacket packet,
                    bool check = true);

	// чтение - зарегистрируйся
	virtual void Start();
	virtual void Stop();
	virtual bool IsActive();

  // синхронная функция
  virtual bool Connect(unsigned int ip, unsigned short port); // вызов только для клиента

	virtual void Close(unsigned int ip, unsigned short port);
protected:
  void Done();
};


#endif
