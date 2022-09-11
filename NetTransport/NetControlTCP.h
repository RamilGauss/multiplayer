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

  int mWorkSocket;// действительно только внутри методов XXXEvent()
    
  enum{
       eHeader    		 = 0xCC5C,
       eSizeBuffer		 = 64000,  
  };

#ifdef WIN32
#pragma pack(push, 1)
#endif
  struct THeader
  {
    short header;
    int   size;
    THeader(){header = short(eHeader);}
  };
#ifdef WIN32
#pragma pack(pop)
#endif

  int mReadSize;
  char mBuffer[eSizeBuffer];
  //----------------------------------------------
  typedef enum{
    eSearchBegin,
    eSearchSize,
    eSearchEnd,
  }eStatePacket;
  struct TDescHistoryRead
  {
		TIP_Port ip_port;
    int sizePacket;// предполагаемый размер пакета
    TContainerRise c;    
    eStatePacket   state;
    TDescHistoryRead()
    {
      state = eSearchBegin;
      sizePacket = 0;
    }
    void Clear()
    {
      state      = eSearchBegin;
      sizePacket = 0;
    }
  };

  typedef std::map<int,TDescHistoryRead> TMapIntDH;
  typedef TMapIntDH::iterator TMapIntDHIt;

	typedef TMapDual<TIP_Port,int> TMapD_IPInt;

  TMapIntDH mMapHistory;
	TMapD_IPInt mMapDIPSock;

  GCS gcsSendAccept;
  void lockSA(){gcsSendAccept.lock();}
  void unlockSA(){gcsSendAccept.unlock();}
public:

  TNetControlTCP();
  virtual ~TNetControlTCP();
  // for INetMakerEvent
	virtual void Work(int sock, std::list<eTypeEvent>& event);
  // TNetTransport_XXX
  virtual bool Open( unsigned short port, unsigned char numNetWork = 0);
  virtual bool Connect(unsigned int ip, unsigned short port);
  virtual void Send(unsigned int ip, unsigned short port, TBreakPacket bp);

	virtual void Close(unsigned int ip, unsigned short port);

protected:
	void ReadEvent();
	void WriteEvent();
	void ConnectEvent();
	void AcceptEvent();				
	void CloseEvent();

  void Analiz();
  int SearchBegin(TDescHistoryRead* pH, int beginPos);
  int SearchSize(TDescHistoryRead* pH, int beginPos);
  int SearchEnd(TDescHistoryRead* pH, int beginPos);

	void AddToMakerEvent(int sock);

  TDescHistoryRead* GetHistoryBuffer(int sock);
	int GetSocketByIpPort( TIP_Port& ip_port);
	void GetIP_PortBySocket( TIP_Port& ip_port, int& sock );

	void Notify(int sock, char* buffer, int size);
};


#endif