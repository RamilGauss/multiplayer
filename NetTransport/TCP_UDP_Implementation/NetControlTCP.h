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


#ifndef NetControlTCPH
#define NetControlTCPH

#include <map>

#include "INetControl.h"
#include "NetDeviceTCP.h"
#include "ContainerRise.h"
#include "GCS.h"
#include "MapDual.h"

class TNetControlTCP : public INetControl
{
  TNetDeviceTCP mDevice;

  int mSocketUp;   // для соединения с сервером
  int mSocketDown; // слушающий сокет, ждет подключения от клиентов

  enum{
       //eHeader    		 = 0xCC5C,
       eSizeBuffer		 = 64000,  
  };

//#ifdef WIN32
//#pragma pack(push, 1)
//#endif
//  struct THeaderTCP
//  {
//    short header;
//    int   size;
//    THeaderTCP(){header = short(eHeader);}
//  };
//#ifdef WIN32
//#pragma pack(pop)
//#endif

  GCS gcsSendAccept;
  void lockSA(){gcsSendAccept.lock();}
  void unlockSA(){gcsSendAccept.unlock();}
public:

  TNetControlTCP();
  virtual ~TNetControlTCP();
  // for INetMakerEvent
	virtual void Work(INetMakerEventThread* pThreadContext, int sock, 
    std::list<nsNetTypeEvent::eTypeEvent>& event);
  // TNetTransport_XXX
  virtual bool Open( unsigned short port, unsigned char numNetWork = 0);
  virtual bool Connect(unsigned int ip, unsigned short port);
  virtual void Send(unsigned int ip, unsigned short port, TBreakPacket bp);

	virtual void Close(unsigned int ip, unsigned short port);
	virtual void Close(int sock);
protected:
	void ReadEvent(INetMakerEventThread* pThreadContext, int sock);
	void WriteEvent(INetMakerEventThread* pThreadContext, int sock);
	void ConnectEvent(INetMakerEventThread* pThreadContext, int sock);
	void AcceptEvent(INetMakerEventThread* pThreadContext, int sock);				
	void CloseEvent(INetMakerEventThread* pThreadContext, int sock);

	void AddToMakerEvent(int sock, bool without_delay);

  void Notify(int sock, char* buffer, int size);

  void Done();
};


#endif