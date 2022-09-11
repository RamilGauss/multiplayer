/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
√удаков –амиль —ергеевич 
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


#ifndef INetControlH
#define INetControlH

#include <list>

#include "BreakPacket.h"
#include "CallBackRegistrator.h"
#include "INetTransport.h"

class INetMakerEvent;

class INetControl
{
public:
  typedef enum{
    eAccept = 0,// попытка к нам соединитьс€
    eConnect,   // мы к кому-то соединились
    eRead,      // есть что прочитать
    eWrite,     // теперь можно писать
    eClose,     // закрытие соединени€
  } eTypeEvent;
  enum{
    eSystemSizeForRecvBuffer_Socket = 3000000, // байт
    eSystemSizeForSendBuffer_Socket = 3000000, // байт
  };

  INetControl(){};
  virtual ~INetControl(){};
  // for INetMakerEvent
  virtual void Work(int sock, std::list<eTypeEvent>& event) = 0;
  // TNetTransport_XXX
  virtual bool Open( unsigned short port, unsigned char numNetWork = 0) = 0;
  virtual bool Connect(unsigned int ip, unsigned short port) = 0;
  virtual void Send(unsigned int ip, unsigned short port, TBreakPacket& bp) = 0;

	virtual void Close() = 0;

  static void SetMakerEvent(INetMakerEvent* pME);

  static void Register(TCallBackRegistrator::TCallBackFunc pFunc,   INetTransport::eTypeCallback type);
  static void Unregister(TCallBackRegistrator::TCallBackFunc pFunc, INetTransport::eTypeCallback type);

protected:
  INetMakerEvent* GetMakerEvent();

	void NotifyRecv(char* p, int size);
	void NotifyDisconnect(char* p, int size);
};


#endif