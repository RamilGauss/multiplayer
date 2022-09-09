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


#ifndef NetTransportH
#define NetTransportH

#include <set>

#include "glib/gthread.h"

#include "CallBackRegistrator.h"
#include "UdpDevice.h"
#include "hArray.h"
#include "GCS.h"
#include "BL_Debug.h"
#include "INetTransport.h"
#include "BasePacketNetTransport.h"

// �������� ����� ��� �������� ��������� ������� �� ����
// ����������� �� ������������� ������� ������ - 1400 ����.

class TNetTransport : public INetTransport
{
protected:
  // ��� �������, � ������� ����� �������� ������������
  // ��� �������� ���������� �������
  // � ��� ��������� �� ������������ ������� (��. GetInfoConnect(...))
  GCS gcsArrFresh;
  void lockArrConnect(){Lock(&gcsArrFresh);};
  void unlockArrConnect(){Unlock(&gcsArrFresh);};

	GCS gcsSendRcv;
	void lockSendRcv(){Lock(&gcsSendRcv);};
	void unlockSendRcv(){Unlock(&gcsSendRcv);};

  TCallBackRegistrator mCallBackRecvPacket;// ��������� �� ip, port, ����� � ������ ������ �� ���������, ������ - {4�|2�|size�-6�}
  TCallBackRegistrator mCallBackRecvStream;// ��������� �� ip, port, ����� � ������ ������ �� ���������, ������ - {4�|2�|size�-6�}
  TCallBackRegistrator mCallBackDisconnect;// ��������� �� ip, port � ��� ��������� ������ �����

	UdpDevice mUDP;	

	volatile bool flgActive;
	volatile bool flgNeedStop;

	enum
  {
    eSizeBuffer     = 65535,
    eCntTry         = 10, //15,
    eTimeLivePacket = 120,//60,//���� � ������� ������ ������� �� ����� ���������, ��������� ����� �� ���������, ��
    eTimeout        = 60, // ������� ���������� ������, ��
    eWaitThread     = eCntTry*eTimeLivePacket,// ��
    // ������ ��������� �������
    eSizeBufferForRecv = 30000000, // ����
    eSizeBufferForSend = 30000000, // ����
    eWaitActivation    = 20,// ����� ���� �������������� ���������, ��
	};
	typedef enum
	{
		ePacket  = 'P',
		eStream  = 'S',
		eSynchro = 'C',
		eCheck   = 'K',
	}eTypePacket;
  enum
  {
    eWaitSynchro=5,// ���
  };

	char mBuffer[eSizeBuffer];
	
	GThread* thread;

public:

	TNetTransport(char* pPathLog=NULL);
	virtual ~TNetTransport();
	virtual void InitLog(char* pPathLog);

  virtual bool Open(unsigned short port, unsigned char numNetWork = 0);

	virtual void Send(unsigned int ip, unsigned short port, 
                    TBreakPacket& packet,//void* packet, int size, 
                    bool check = true);

	// ������ - ���������������
  virtual void Register(TCallBackRegistrator::TCallBackFunc pFunc, int type);
  virtual void Unregister(TCallBackRegistrator::TCallBackFunc pFunc, int type);

	virtual void Start();
	virtual void Stop();
	virtual bool IsActive();

  // ���������� �������
  virtual bool Synchro(unsigned int ip, unsigned short port); // ����� ������ ��� �������
protected:
  bool SendSynchro(unsigned int ip, unsigned short port, int cntTry);

  // �������� ������ 
  void Lock(void* pLocker);
  void Unlock(void* pLocker);

protected:
  void notifyDisconnect(TIP_Port* data){mCallBackDisconnect.Notify(data,sizeof(TIP_Port));};

	friend void* ThreadTransport(void*p);
	void Engine();

protected:

  // ������, ��������� ���������
	TArrayObject mArrWaitCheck;
	
	void* FindInList();

protected:
	void AnalizPacket(unsigned int ip,unsigned short port,int size);
  void FindAndCheck(nsNetTransportStruct::THeader* prefix,
                    unsigned int ip,unsigned short port);
  void NotifyRecv(TCallBackRegistrator& callBack, int size);
  void SendCheck(nsNetTransportStruct::THeader* prefix,
                 unsigned int ip,unsigned short port);

  // ������� ����� � ������
	int GetTimeout();// ��
	void SendUnchecked();

  bool Send(TBasePacketNetTransport* pDefPacket);
  void Disconnect(TIP_Port* ip_port);

protected:
  bool Write(void *p, int size, unsigned int ip, unsigned short port);

  TSaveOnHDD mLogRcvSend;
  TSaveOnHDD mLogEvent;

  void WriteLog(void *p, int size, unsigned int ip, unsigned short port);
	void ReadLog(int size, unsigned int ip, unsigned short port);
	void LogTransportInfo(nsNetTransportStruct::THeader* p,int size);

protected:

  // ���������� �� ������. ����� ������� ����� ������ ���� �� ����� � �� ��������
  TArrayObject mArrConnect;

  bool IsPacketFresh();
  bool IsStreamFresh();

  bool A_more_B(unsigned short A, unsigned short B);
  
	static int SortFreshInfoConnect(const void* p1, const void* p2);
  static int SortFreshDefPacket(const void* p1, const void* p2);

  // ���� ������ ������ - ������ ��������� �� ����
  // ����� ������� �����
	nsNetTransportStruct::TInfoConnect* GetInfoConnect(TIP_Port& v);

  // � ������� pArr ����� � ������� ������, ������� �������� �� pDefPacket
  void SearchAndDelete(TArrayObject* pArr, TIP_Port* ip_port);

  void SearchAndDeleteConnect(TIP_Port* ip_port);

  void SetupBufferForSocket();
};


#endif