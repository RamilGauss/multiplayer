/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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
the "Tanks" Source Code.  If not, please request a copy in writing from id Software at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef TransportProtocolH
#define TransportProtocolH

#include "CallBackRegistrator.h"
#include "UdpDevice.h"
#include "List.h"
#include "glib/gthread.h"
#include "TransportProtocolPacket.h"
#include "hArray.h"
#include "GCS.h"

// Melissa - ���������
class TransportProtocol
{
  // ��� �������, � ������� ����� �������� ������������
  // ��� �������� ���������� �������
  // � ��� ��������� �� ������������ ������� (��. GetInfoConnect(...))
  GCS gcsArrFresh;
  void lockArrFresh(){gcsArrFresh.lock();};
  void unlockArrFresh(){gcsArrFresh.unlock();};

	GCS gcsSendRcv;
	void lockSendRcv(){gcsSendRcv.lock();};
	void unlockSendRcv(){gcsSendRcv.unlock();};


  CallBackRegistrator mCallBackRecvPacket;// ��������� �� ip, port, ����� � ������ ������ �� ���������, ������ - {4�|2�|size�-6�}
  CallBackRegistrator mCallBackRecvStream;// ��������� �� ip, port, ����� � ������ ������ �� ���������, ������ - {4�|2�|size�-6�}
  CallBackRegistrator mCallBackDisconnect;// ��������� �� ip, port � ��� ��������� ������ �����

	UdpDevice mUDP;	

	bool flgActive;
	bool flgNeedStop;

	enum{eSizeBuffer     = 8096,
       eCntTry         = 15,
       eTimeLivePacket = 60,// ��
			 eTimeout        = 60,// ��
       eWaitThread     = eCntTry*eTimeLivePacket,// ��
	};
	char mBuffer[eSizeBuffer];
	
	GThread* thread;

public:

	struct InfoData
	{
		unsigned int   ip_dst;     // dst ip
		unsigned short port_dst;   // dst port
		unsigned int   ip_src;     // dst ip
		unsigned short port_src;   // dst port
		void*          packet; // �������� malloc
		int            size;
    InfoData(){packet = NULL;}
    ~InfoData(){}
	};

	enum{ePacket=0,eStream=1,};

	TransportProtocol(char* pPathLog=NULL);
	~TransportProtocol();
	void InitLog(char* pPathLog);

  bool Open(unsigned short port,int numNetWork=0);

	void write(InfoData* data, bool check = true);
	// ������ - ���������������

	void start();
	void stop();

  enum{
		eWaitSynchro=5,
	};// ���
  // ���������� �������
  bool synchro(unsigned int ip, unsigned short port); // ����� ������ ��� �������
protected:
  bool SendSynchro(unsigned int ip, unsigned short port, int cntTry);

public:
  void Register(CallBackRegistrator::TCallBackFunc pFunc, int type);
  void Unregister(CallBackRegistrator::TCallBackFunc pFunc, int type);

protected:
  void notifyRcvPacket(void * data,int size){mCallBackRecvPacket.Notify(data,size);};
  void notifyRcvStream(void * data,int size){mCallBackRecvStream.Notify(data,size);};
  void notifyDisconnect(TIP_Port* data){mCallBackDisconnect.Notify(data,sizeof(TIP_Port));};

	friend void* ThreadTransport(void*p);
	void Engine();

protected:
	
  class TDefPacket  : public TObject
  {
  protected:
    InfoData*        infoData;// new
    //----------------------
    void* mReadyPacket;
    int   mSizePacket;
    //----------------------
  public:  
    void SetData(InfoData* pInfoD )
    {
      infoData = pInfoD;
      free(mReadyPacket);
      mSizePacket = infoData->size+sizeof(TPrefixTransport);
      mReadyPacket = malloc(mSizePacket);
      memcpy((char*)mReadyPacket+sizeof(TPrefixTransport),pInfoD->packet,infoData->size);
      ((TPrefixTransport*)mReadyPacket)->ip_port_dst.ip   = infoData->ip_dst;
      ((TPrefixTransport*)mReadyPacket)->ip_port_dst.port = infoData->port_dst;
			((TPrefixTransport*)mReadyPacket)->ip_port_src.ip   = infoData->ip_src;
			((TPrefixTransport*)mReadyPacket)->ip_port_src.port = infoData->port_src;
      ((TPrefixTransport*)mReadyPacket)->cntTry           = -1;
    }
    void SetType(unsigned char type)
    {
      if(mReadyPacket==NULL) {BL_FIX_BUG();return;}
      TPrefixTransport* prefix = (TPrefixTransport*)mReadyPacket;
      prefix->type = type;
    }
    void SetCnIn(unsigned short cn)
    {
      if(mReadyPacket==NULL) {BL_FIX_BUG();return;}
      TPrefixTransport* prefix = (TPrefixTransport*)mReadyPacket;
      prefix->cn_in = cn;
    }
    void SetCnOut(unsigned short cn)
    {
      if(mReadyPacket==NULL) {BL_FIX_BUG();return;}
      TPrefixTransport* prefix = (TPrefixTransport*)mReadyPacket;
      prefix->cn_out = cn;
    }
    void SetTime(guint32 time_ms)
    {
      if(mReadyPacket==NULL) {BL_FIX_BUG();return;}
      TPrefixTransport* prefix = (TPrefixTransport*)mReadyPacket;
      prefix->time_ms = time_ms;
    }
    void SetCntTry(unsigned char  cntTry)
    {
      if(mReadyPacket==NULL) {BL_FIX_BUG();return;}
      TPrefixTransport* prefix = (TPrefixTransport*)mReadyPacket;
      prefix->cntTry = cntTry;
    }

    unsigned short GetCnIn()
    {
      if(mReadyPacket==NULL) {BL_FIX_BUG();return 0;}
      TPrefixTransport* prefix = (TPrefixTransport*)mReadyPacket;
      return prefix->cn_in;
    }
    unsigned short GetCnOut()
    {
      if(mReadyPacket==NULL) {BL_FIX_BUG();return 0;}
      TPrefixTransport* prefix = (TPrefixTransport*)mReadyPacket;
      return prefix->cn_out;
    }
    guint32 GetTime()
    {
      if(mReadyPacket==NULL) {BL_FIX_BUG();return 0;}
      TPrefixTransport* prefix = (TPrefixTransport*)mReadyPacket;
      return prefix->time_ms;
    }
    unsigned char GetCntTry()
    {
      if(mReadyPacket==NULL) {BL_FIX_BUG();return 0;}
      TPrefixTransport* prefix = (TPrefixTransport*)mReadyPacket;
      return prefix->cntTry;
    }

    void* GetData(int & size, unsigned int& ip_dst, unsigned short& port_dst)
    {
      if(mReadyPacket==NULL) BL_FIX_BUG();
      size = mSizePacket;
      ip_dst   = infoData->ip_dst;
      port_dst = infoData->port_dst;
      return mReadyPacket;
    }

    unsigned int GetIP_dst() const
    {
      if(mReadyPacket==NULL) BL_FIX_BUG();
      return infoData->ip_dst;
    }

    unsigned short GetPort_dst() const
    {
      if(mReadyPacket==NULL) BL_FIX_BUG();
      return infoData->port_dst;
    }

    TDefPacket(){infoData=NULL;mReadyPacket=NULL;mSizePacket=0;}
    ~TDefPacket(){delete infoData;free(mReadyPacket);}
  };

  // ������, ��������� ���������
	/*TList<TDefPacket>*/ TArrayObject mArrWaitCheck;
  // ������, ��������� ��������
  /*TList<TDefPacket>*/ TArrayObject mArrWaitSend;
	
	void* FindInList();

protected:
	void AnalizPacket(unsigned int ip,unsigned short port,int size);
  void FindAndCheck(TPrefixTransport* prefix,unsigned int ip,unsigned short port);
  void NotifyRcvPacket(int size);
  void NotifyRcvStream(int size);
  void SendCheck(TPrefixTransport* prefix,unsigned int ip,unsigned short port);

	int GetTimeout();
	void SendUnchecked();

  bool Send(TDefPacket* pDefPacket);
  void Disconnect(TDefPacket* pDefPacket);

protected:
	bool Write(void *p, int size, unsigned int ip, unsigned short port);

	TSaveOnHDD mLogRcvSend;

	TSaveOnHDD mLogEvent;

	void WriteLog(void *p, int size, unsigned int ip, unsigned short port);
	void ReadLog(int size, unsigned int ip, unsigned short port);
	void LogTransportInfo(TPrefixTransport* p,int size);

protected:
  class  InfoConnect : public TObject
  {
    public:
      InfoConnect(){cn_in_s=0;cn_out_s=0;cn_in_p=0;cn_out_p=0;};
      unsigned int   ip;
      unsigned short port;
			// ��� Stream
			unsigned short cn_in_s;     // ����������� ����� ��� ����������� �������� ��� Rcv
			unsigned short cn_out_s;    // ����������� ����� ��� Send
			// ��� Packet
			unsigned short cn_in_p;     // ����������� ����� ��� ����������� �������� ��� Rcv
			unsigned short cn_out_p;    // ����������� ����� ��� Send
  };

  TArrayObject mArrFresh;

  bool IsPacketFresh();
  bool IsStreamFresh();

  bool A_more_B(unsigned short A, unsigned short B);
  
	static int SortFreshInfoConnect(const void* p1, const void* p2);
  static int SortFreshDefPacket(const void* p1, const void* p2);

	InfoConnect* GetInfoConnect(unsigned int ip,unsigned short port);

	bool FindInArrWaitCheck(TDefPacket* pDefPacket);
};


#endif