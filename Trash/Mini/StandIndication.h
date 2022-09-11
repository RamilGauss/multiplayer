/*
			04.10.2012
				Gauss
	 ласс дл€ управлени€ и взаимодействи€ со стендом индикации.
	“ранспортный уровень - гаранти€ доставки, иначе оповещение о разрыве св€зи
	явл€етс€ частью ѕќ "ћагний"
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
		// физические параметры ком-порта
		eBaudRate  = 19200, // скорость, бит в секунду
		eByteSize  = 8,     // кол-во информационных бит в посылке( 8 или 7 )
		eParity    = 0,     // 0 - Ќет проверки четности, 1 - ѕроверка четности, 2 - ѕроверка нечетности
		eStopBits  = 2,     // кол-во стоповых бит: 0 - 1 бит, 2 - 2 бита
		eIsCTR_RTS = 0,     // выкл. аппаратное управление потоком

		// кол-во ком-портов в системе
		eCntComPort = 8,
	
		eTryCount 	           = 3,
		eCntTryReadSignatura   = 3,
	
		eTimeSleepEngineThread = 30,// мс
		eTimeWaitStopThread    = 100,// мс
		eTimeRequest			     = 60,// мс
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
  
  // по отношению к устройству
	TVectorChar mSignaturaIn;// в устройство
	TVectorChar mSignaturaOut;// из устройства
	
public:
	// передать сигнатуру дл€ соединени€ с устройством (когда вызываетс€ Open())
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

