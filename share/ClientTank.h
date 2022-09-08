#ifndef ClientTankH
#define ClientTankH

#include "CallBackRegistrator.h"
#include "TransportProtocolTank.h"

class ClientTank
{
	bool flgNeedStop;
	bool flgActive;
	enum{eWaitThread = 100, // мс
			 eWaitCheck  = 1500, //мс, реально будем ждать 3 секунды
	};

	volatile guint32 mLastTimeRcv;// время последнего получения пакета от сервера (стрим или обычный пакет)

	TransportProtocolTank mTransport;

	CallBackRegistrator mCallBackDisconnect;
	CallBackRegistrator mCallBackStream;
	CallBackRegistrator mCallBackPacket;

	GThread* thread;// поток определения дисконнекта от сервера

  unsigned int mIP_server;
public:
  
  ClientTank();
  ~ClientTank();
  void start();
  void stop();
  void Register(CallBackRegistrator::TCallBackFunc pFunc, int type);
  void Unregister(CallBackRegistrator::TCallBackFunc pFunc, int type);

//--------------------------------------------------------------------
public:
  bool IsConnect();
  
//--------------------------------------------------------------------
public:
  // команды и запросы на сервер
  // request
  bool Connect(unsigned int ip, unsigned int port, char* sNick);
  void SendRequestListTank();
  void SendRequestExitFromWait();
  void SendRequestExitFromFight();
  void SendRequestInFight();
  // cmd
  //void SendClientReadyForRoom();
  void SetCurrentTank(int i);
  void PushButton(int button);

  void SendRequestCorrectPacket();
//--------------------------------------------------------------------
protected:
	friend void* ClientThreadDefDisconnect(void*p);
	void startThreadDefDisconnect();
	void ThreadDefDisconnect();

	friend void ClientCallBackRcvPacket(void* data, int size);
	friend void ClientCallBackRcvStream(void* data, int size);
	friend void ClientCallBackRcvDisconnect(void* data, int size);
 	
	// для уведомления внешених зарегистрированных объектов (как правило GUI окон)
	void RcvStream(void* data, int size);	
	void RcvPacket(void* data, int size);
	void Disconnect(void* data, int size);	

	void SetLastTime(){mLastTimeRcv=ht_GetMSCount();};


  void WriteTransport(TBasePacket* packet);
};

extern ClientTank GlobalClientTank;

#endif