/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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


#ifndef NetControlUDPH
#define NetControlUDPH

#include "INetControl.h"
#include "NetDeviceUDP.h"
#include "hArray.h"
#include "GCS.h"

class TNetControlUDP : public INetControl
{
	enum{
		eSizeBuffer=64000,
	};
	int mReadSize;
	char mBuffer[eSizeBuffer];


  TNetDeviceUDP mDevice;

	int mSocketLocal;

	TArrayObject mArrConnect;

#if defined( WIN32 )
#pragma pack(push, 1)
#endif

	struct TInfoConnect : public TObject
	{
	public:
		TInfoConnect(){Init();}
		unsigned int   ip;
		unsigned short port;
		// ��� Stream
		unsigned short cnt_in; // ���������� �������� ������ �� ������� ������
		unsigned short cnt_out;// �������� ������
		void Init()
		{
			cnt_in  = 0;
			cnt_out = 0;
		}
	};
	//-----------------------------------------------------------------------------
#if defined( WIN32 )
#pragma pack(pop)
#endif

	GCS gcsSendRcv;
	void lockSendRcv()  {gcsSendRcv.lock();};
	void unlockSendRcv(){gcsSendRcv.unlock();};

public:

  TNetControlUDP();
  virtual ~TNetControlUDP();
  // for INetMakerEvent
	virtual void Work(int sock, std::list<eTypeEvent>& event);
  // TNetTransport_XXX
  virtual bool Open( unsigned short port, unsigned char numNetWork = 0);
  virtual bool Connect(unsigned int ip, unsigned short port);
  virtual void Send(unsigned int ip, unsigned short port, TBreakPacket& bp);
	virtual void Close();

protected:

	static int SortFreshInfoConnect(const void* p1, const void* p2);

	bool IsStreamFresh(TIP_Port& ip_port);
	bool A_more_B(unsigned short A, unsigned short B);


	void ReadEvent();
	void WriteEvent();
	void ConnectEvent();
	void AcceptEvent();				
	void CloseEvent();

	TInfoConnect* GetInfoConnect(TIP_Port& v);
};


#endif