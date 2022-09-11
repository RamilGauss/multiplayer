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


#ifndef NetDoserH
#define NetDoserH

#include <set>

#include "NetTransport.h"
#include "DoserProtocolPacket.h"
#include "hArray.h"
#include "GCS.h"
#include "DoserPacket.h"
#include "ControlTraffic.h"

// ���� ������:
// 1. �������� ������� (������� ������ �������� ����� �������� � �����������), �������� ������� ��������.
// 2. �������� ������� ������� 1.4�� - 1��.
// � ������ ����������� �����, ������� ��� �� ��������� � ������.
// ����� ������� ������ � ������ �������� � � ������ �������� ������ ������ ������������� �������

class TNetDoser : public INetTransport
{
  enum
  {
    eWaitThread     = 50,// ��
    eSleepThread    = 60,// ��
    eWaitActivation = 20,
  };

  volatile bool flgActive;
  volatile bool flgNeedStop;
	
  GThread* thread;

	TCallBackRegistrator mCallBackRecv;      // TDescRecv		
  TCallBackRegistrator mCallBackDisconnect;// TIP_Port

public:

  enum{    
    eLimitSizePacket = 1355, // ������������ ������ ������, ������� �� ������� ���������, ����
  };

	TNetDoser(char* pPathLog=NULL);
	virtual ~TNetDoser();
	virtual void InitLog(char* pPathLog);

  virtual bool Open(unsigned short port, unsigned char numNetWork = 0);

	virtual void Send(unsigned int ip, unsigned short port, 
                    TBreakPacket& packet,//void* packet, int size, 
                    bool check = true);

	// ������ - ���������������
  virtual void Register(TCallBackRegistrator::TCallBackFunc pFunc, eTypeCallback type);
  virtual void Unregister(TCallBackRegistrator::TCallBackFunc pFunc, eTypeCallback type);

	virtual void Start();
	virtual void Stop();
	virtual bool IsActive();

  // ���������� �������
  virtual bool Connect(unsigned int ip, unsigned short port); // ����� ������ ��� �������

private:
	TNetTransport       mTransport;
  TDoserPacket        mDoserPacket;
  TControlTraffic   mControlTrafficTo;

protected:
  friend void* ThreadDoser(void*p);
  void Engine();

  void NotifyRecvSinglePacket(TDescRecv* p);

public:
  void Analiz(TDescRecv* p, int s);// ������ ����� �������
  void Disconnect(TIP_Port* p, int s);
  void RecvBigPacket(TDescRecv* p, int s);

};

#endif
