/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012
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
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef INET_TransportH
#define INET_TransportH

#include "CallBackRegistrator.h"
#include "UdpDevice.h"
#include "glib/gthread.h"
#include "TransportProtocolPacket.h"
#include "hArray.h"
#include "GCS.h"
#include "BL_Debug.h"

// Melissa - транспорт
class INET_Transport
{
public:
  //типы callback вызовов
  enum{
    eRcvPacket  = 0,		   
    eRcvStream  = 1,
    eDisconnect = 2,
  };

	struct InfoData
	{
		unsigned int   ip_dst;     
		unsigned short port_dst;   
		unsigned int   ip_src;     
		unsigned short port_src;   
		void*          packet;
		int            size;
    InfoData(){size=0;packet = NULL;}
    ~InfoData(){}
	};

	enum{eWaitSynchro=5,// сек
  };


  INET_Transport(char* pPathLog=NULL){};
  virtual ~INET_Transport(){};
	virtual void InitLog(char* pPathLog) = 0;

  virtual bool Open(unsigned short port,int numNetWork=0) = 0;

	virtual void Write(InfoData* data, bool check = true, bool add_in_queque = true) = 0;

	// чтение - зарегистрируйся
  virtual void Register(TCallBackRegistrator::TCallBackFunc pFunc, int type) = 0;
  virtual void Unregister(TCallBackRegistrator::TCallBackFunc pFunc, int type) = 0;

	virtual void Start() = 0;// для активной работы
	virtual void Stop()  = 0;

  // синхронная функция
  virtual bool Synchro(unsigned int ip, unsigned short port) = 0; // вызов только для клиента
};


#endif