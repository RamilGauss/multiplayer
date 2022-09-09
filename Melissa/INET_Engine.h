/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012
===========================================================================
                        Common Information
"Tanks" GPL Source Code

This file is part of the "Tanks" GPL Source Code.

"Tanks" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"Tanks" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "Tanks" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "Tanks" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "Tanks" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef INET_EngineH
#define INET_EngineH

#include "CallBackRegistrator.h"
#include "UdpDevice.h"
#include "glib/gthread.h"
#include "TransportProtocolPacket.h"
#include "hArray.h"
#include "GCS.h"
#include "BL_Debug.h"

// Melissa - транспорт
class INET_Engine
{
public:

	struct InfoData
	{
		unsigned int   ip_dst;     // dst ip
		unsigned short port_dst;   // dst port
		unsigned int   ip_src;     // dst ip
		unsigned short port_src;   // dst port
		void*          packet; // выделено malloc
		int            size;
    InfoData(){packet = NULL;}
    ~InfoData(){}
	};

	enum{ePacket=0,
       eStream=1,
       eWaitSynchro=5,// сек
  };


	INET_Engine(char* pPathLog=NULL);
	virtual ~INET_Engine();
	virtual void InitLog(char* pPathLog) = 0;

  virtual bool Open(unsigned short port,int numNetWork=0) = 0;

	virtual void write(InfoData* data, bool check = true) = 0;
	// чтение - зарегистрируйся
  virtual void Register(TCallBackRegistrator::TCallBackFunc pFunc, int type) = 0;
  virtual void Unregister(TCallBackRegistrator::TCallBackFunc pFunc, int type) = 0;

	virtual void start() = 0;
	virtual void stop()  = 0;

  // синхронная функция
  virtual bool synchro(unsigned int ip, unsigned short port) = 0; // вызов только для клиента

};


#endif