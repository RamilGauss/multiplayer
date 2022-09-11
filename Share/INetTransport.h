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


#ifndef INetTransportH
#define INetTransportH

#define STR_NAME_NET_TRANSPORT "NT"

#include "CallBackRegistrator.h"
#include "TypeDef.h"
#include "ShareMisc.h"
#include "BreakPacket.h"

// транспорт
class SHARE_EI INetTransport
{
public:
  //типы callback вызовов
  typedef enum{
    eRecv       = 0, // TDescRecv		   
    eDisconnect = 1, // TIP_Port
  }eTypeCallback;
	typedef enum{
		ePacket  = 0,		   
		eStream  = 1,
	}eTypeRecv;
  // callback вернет данную структуру
  struct TDescRecv
  {
    TIP_Port ip_port;
    char* data;
    int   sizeData;
		eTypeRecv type;
  };

  INetTransport();
  virtual ~INetTransport();
  virtual bool Open(unsigned short port, unsigned char numNetWork = 0) = 0;

  // в качестве передаваемых данных выступает объект, который
  // содержит цепочку данных
	virtual void Send(unsigned int ip, unsigned short port,
                    TBreakPacket packet, bool check = true) = 0;

	// чтение - зарегистрируйся
  virtual void Register(TCallBackRegistrator::TCallBackFunc pFunc, eTypeCallback type) = 0;
  virtual void Unregister(TCallBackRegistrator::TCallBackFunc pFunc, eTypeCallback type) = 0;

	// старт и стоп движка
	virtual void Start() = 0;
	virtual void Stop()  = 0;
	virtual bool IsActive() = 0;

  // синхронная функция
	// вызывать до вызова Start()
  virtual bool Connect(unsigned int ip, unsigned short port) = 0; // вызов только для клиента

	virtual void Close(unsigned int ip, unsigned short port) = 0;
};


#endif