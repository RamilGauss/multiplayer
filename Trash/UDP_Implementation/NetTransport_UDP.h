/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef NetTransportUDPH
#define NetTransportUDPH

#include <set>

#include "CallBackRegistrator.h"
#include "UdpDevice.h"
#include "GCS.h"
#include "BL_Debug.h"
#include "INetTransport.h"
#include "BasePacketNetTransport.h"
#include "TransportProtocolPacket.h"

// �������� ����� ��� �������� ��������� ������� �� ����
// ����������� �� ������������� ������� ������ - 1400 ����.
// �� ����� �������������� ������
// ����� ������ � ��� ��� �� �������� ������ �� UDP

// Thread safe - Send ������������.

class TNetTransport_UDP : public INetTransport
{
protected:
  // ��� �������, � ������� ����� �������� ������������
  // ��� �������� ���������� �������
  // � ��� ��������� �� ������������ ������� (��. GetInfoConnect(...))
	GCS gcsSendRcv;
	void lockSendRcv()  {Lock(&gcsSendRcv);};
	void unlockSendRcv(){Unlock(&gcsSendRcv);};

	UdpDevice mUDP;	

	volatile bool flgActive;
	volatile bool flgNeedStop;

  typedef std::map<TIP_Port,nsNetTransportStruct::TInfoConnect*> TMapIP_Ptr;
  typedef TMapIP_Ptr::iterator TMapIP_Ptr_It;
	// ���������� �� ������, ����� ������� ����� ������ ���� �� ����� � �� ��������
  TMapIP_Ptr mMapInfoConnect;

  typedef std::multimap<TIP_Port,TBasePacketNetTransport*> TMultiMapIP_Ptr;
  typedef TMultiMapIP_Ptr::iterator TMultiMapIP_Ptr_It;
	// ������, ��������� ���������
  TMultiMapIP_Ptr mMultiMapWaitCheck;

	enum
  {
    eSizeBuffer        = 65535,
    eCntTry            = 10,
    eTimeLivePacket    = 120, //���� � ������� ������ ������� �� ����� ���������, ��������� ����� �� ���������, ��
    eWaitThread        = eTimeLivePacket*eCntTry,
    eTimeRefreshEngine = 60,  // ������� ���������� ������, ��
    
    // ������ ��������� �������
    eSizeBufferForRecv = 30000000, // ����
    eSizeBufferForSend = 30000000, // ����
    eFeedBack          = 20,// ����� ���� (��)�������������� ���������, ��
	};
	typedef enum
	{
		eTcp  = 'P',
		eUdp  = 'S',
		eSynchro = 'C',
		eCheck   = 'K',
	}eTypeHeadPacket;
  enum
  {
    eWaitSynchro=5,// ���
  };

	char mBuffer[eSizeBuffer];

  TCallBackRegistrator1<TDescRecv*> mCallBackEventRecieve;
  TCallBackRegistrator1<TIP_Port*>  mCallBackEventDisconnect;

public:

	TNetTransport_UDP();
	virtual ~TNetTransport_UDP();

  virtual bool Open(unsigned short port, unsigned char numNetWork = 0);
	virtual void Send(unsigned int ip, unsigned short port, 
                    TBreakPacket packet,
                    bool check = true);

	virtual void Start();
	virtual void Stop();
	virtual bool IsActive();

  // ���������������� �������
  virtual bool Connect(unsigned int ip, unsigned short port); // ����� ������ ��� �������
	
	virtual void Close(unsigned int ip, unsigned short port){};

  virtual TCallBackRegistrator1<TDescRecv*>* GetCallbackRecv()      {return &mCallBackEventRecieve;}
  virtual TCallBackRegistrator1<TIP_Port* >* GetCallbackDisconnect(){return &mCallBackEventDisconnect;}

protected:
  bool SendSynchro(unsigned int ip, unsigned short port, int cntTry);

  // �������� ������ ����������
  void Lock(void* pLocker);
  void Unlock(void* pLocker);

protected:
	void Engine();

protected:
	void AnalizPacket(unsigned int ip,unsigned short port,int size);
  void FindAndCheck(nsNetTransportStruct::THeader* prefix,
                    unsigned int ip,unsigned short port);
  void NotifyRecv(eTypeRecv type, int size);
  void SendCheck(nsNetTransportStruct::THeader* prefix,
                 unsigned int ip,unsigned short port);

  // ������� ����� � ������
	int GetTimeout();// ��
	void SendUnchecked();

  bool Send(TBasePacketNetTransport* pDefPacket);

protected:
  bool Write(void *p, int size, unsigned int ip, unsigned short port);

  void WriteLog(void *p, int size, unsigned int ip, unsigned short port);
	void ReadLog(int size, unsigned int ip, unsigned short port);
	void LogTransportInfo(nsNetTransportStruct::THeader* p,int size);

protected:

  bool IsPacketFresh();
  bool IsStreamFresh();

  bool A_more_B(unsigned short A, unsigned short B);
  
  // ���� ������ ������ - ������ ��������� �� ����
  // ����� ������� �����
	nsNetTransportStruct::TInfoConnect* GetInfoConnect(TIP_Port& v);

  // � ������� pArr ����� � ������� ������, ������� �������� �� pDefPacket
  void SearchAndDeleteInMapWaitCheck(std::set<TIP_Port>& setIP_port);
  void SetupBufferForSocket();

  void NotifyReceive(TDescRecv* p);
  void NotifyDisconnect(TIP_Port* p);
};
//-------------------------------------------------------------------

#endif