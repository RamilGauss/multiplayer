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


#ifndef ControlTrafficH
#define ControlTrafficH

#include <set>
#include <list>

#include "INetTransport.h"
#include "BreakPacket.h"
#include "GCS.h"
#include "hArray.h"
#include "DescSendPacket.h"
#include "DoserProtocolPacket.h"
#include "Queue2Thread.h"

class TControlTraffic
{
	enum{
		eSizeQueue= 100,
		eTimeWork = 10,// мс
	};

  GCS mConnectMutex; // Send/Work
	
	struct TEvent
	{
		TIP_Port      ip_port;
		unsigned char cntTry;
	};
	TQueue2Thread mQEvent;// для обмена событиями потока Work и потока LostPacket

  INetTransport* mTransport;
public:

	TControlTraffic();
	~TControlTraffic();

	void LostPacket(INetTransport::TLostPacket* p);//INetTransport::TDescRecv* p);
  void Send(TIP_Port& ip_port, std::list<nsNetDoser::TDescSendPacket*>* pLDSP);
  bool Work();
  void SetTransport(INetTransport* pNT);

private:
  // по каждому ip ведется статистика и учет. Регулируется скорость подачи данных
  // содержится список пакетов на отправку
  TArrayObject mArrConnect;

  // активные соединения
  // в пользовательском потоке только добавление
  // в рабочем только удаление
  // !!! обрамлять критической секцией удаление и добавление
  // считается активной, только если есть пакет на отправку (Single или BigPacket)
	typedef std::set<nsNetDoser::TDescConnect*> TSetPtr;
	typedef TSetPtr::iterator TSetPtrIt;
  TSetPtr mActiveConnect;

protected:
  static int SortConnect(const void* p1, const void* p2);
  static int SortConnectByIP_Port(const void* p1, const void* p2);

  // гарантированно вернет указатель на описание соединения
  nsNetDoser::TDescConnect* GetDescConnect(unsigned int ip, unsigned short port);

	void HandleEvent();


};

#endif
