/*
			04.10.2012
				Gauss
	����� ��� ���������� � �������������� �� ������� ���������.
	������������ ������� - �������� ��������, ����� ���������� � ������� �����
	�������� ������ �� "������"
*/

#ifndef STANDINDICATION_H
#define STANDINDICATION_H


#include "CallBackRegistrator.h"
#include "StandIndication_struct.h"
#include "SaveOnHDD.h"
#include "PacketTransportLevel_SI.h"

#ifdef WIN32
  #include "glib/gthread.h"
  #include "ComPort.h"
#else
  #include "rs232.h"
  #include <pthread.h>
#endif

#include <string.h>
#include <stdio.h>
#include <vector>


class TStandIndication
{
public:
	enum
	{
		// ���������� ��������� ���-�����
		eBaudRate  = 19200, // ��������, ��� � �������
		eByteSize  = 8,     // ���-�� �������������� ��� � �������( 8 ��� 7 )
		eParity    = 0,     // 0 - ��� �������� ��������, 1 - �������� ��������, 2 - �������� ����������
		eStopBits  = 2,     // ���-�� �������� ���: 0 - 1 ���, 2 - 2 ����
		eIsCTR_RTS = 0,     // ����. ���������� ���������� �������

		// ���-�� ���-������ � �������
		eCntComPort = 8,
	
		eTryCount 	           = 3,
		eCntTryReadSignatura   = 3,
	
		eTimeSleepEngineThread = 30,// ��
		eTimeWaitStopThread    = 100,// ��
		eTimeRequest			     = 60,// ��
		eTimeWaitSignatura		 = eTimeRequest*eTryCount,
		eSizeBufferRead        = 2048,
	};

protected:

	volatile bool flgActive;
	volatile bool flgNeedStop;

#ifdef WIN32	
  GThread*
#else
  pthread_t
#endif
  thread_id;

#ifdef WIN32	
  TComPort
#else
	RS232 
#endif
  mDevice;
	
	TCallBackRegistrator mCallBackRegistratorAnswer;

	TSaveOnHDD mLog;

	char mBufferRead[eSizeBufferRead];
	long unsigned int mSizeRead;
	long unsigned int mSizeRest;
	
  typedef std::list<TPacketTransportLevel_SI*> TListPtr;
  
  TListPtr mListReadPacket;
  
  typedef std::vector<char> TVectorChar;
  
  // �� ��������� � ����������
	TVectorChar mSignaturaIn;// � ����������
	TVectorChar mSignaturaOut;// �� ����������
	
public:
	// �������� ��������� ��� ���������� � ����������� (����� ���������� Open())
	TStandIndication();
	virtual ~TStandIndication();

	void SetSignatura(TVectorChar* sSignaturaIn, TVectorChar* sSignaturaOut);
	
	bool Start(int port = -1);// sync
	void Stop();// sync
	bool Send(void* pData, int size);// async
	
	bool IsActive();
	
	void Register(  /*int type, */TCallBackRegistrator::TCallBackFunc callBack);
	void Unregister(/*int type, */TCallBackRegistrator::TCallBackFunc callBack);
	//------------------------------------------------------------
protected:
	//void NotifyDisconnect();
	void NotifyAnswer(TPacketTransportLevel_SI& packet);

	bool Open(int port = -1);	
	bool TryOpenAndReadSignatura(int port);
	
	friend void* ThreadSI(void*);
	void StartThread();
	void StopThread();
	void Engine();

	bool Read();
  void SearchPacket();

	void Init();
	
	bool WriteSignatura();
	bool ReadSignatura();
  
  void ClearListPacket();

  bool CompareSignatura(TPacketTransportLevel_SI* pPacket);

};
  
#endif

