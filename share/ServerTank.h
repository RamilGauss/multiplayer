#ifndef ServerTankH
#define ServerTankH


#include "TransportProtocolTank.h"
#include <glib/gthread.h>
#include "List.h"
#include "ServerStruct.h"
#include "CallBackRegistrator.h"
#include "ApplicationProtocolPacketRequest.h"
#include "StatisticValue.h"
#include "ApplicationProtocolPacketAnswer.h"

class ServerTank
{

  typedef TStatType_double TStatLoad;
  TStatLoad statistica;


	TSaveOnHDD mLogLoad;
	
	CallBackRegistrator mCallBackRequestListClient; // 

	TransportProtocolTank mTransport;

	GThread* threadStreamAnswerCommand;

	friend void* ThreadProcServer( void* p);

  bool flgActive;
  bool flgNeedStop;

  enum{eWaitThread    = 100, // мс
       eTimeSleep     = 100, // мс
       eMaxLoadServer = 75,  // проценты
  };


  guint32 mLastTime;

	// список комнат
	TList<TRoom>  mListRoom;
	// список клиентов
  TArrayObject mArrClients;

  // вместо Delete использовать Unlink!
  TArrayObject mArrClientsFight;//указатель на тех кто в бою
  TArrayObject mArrClientsWait;

  // запросы и команды на изменение параметров
  TList<nsServerStruct::TPacketServer> mListPacket;
  // стрим от клиента (ориентация прицела)
  TList<nsServerStruct::TPacketServer> mListStream;
	// подготовленный транспортом список на дисконнект, асинхронное разъединение
	TList<TIP_Port> mListDisconnectClient;

public:

  // клиенты
	struct TDefClient
	{
		char name[nsServerStruct::TClient::eMaxLenNick];
		unsigned int ip;
		unsigned int port;
		int state;
    bool flgDisconnect;
	};
  // комнаты
  struct TDefRoom
  {
    guint32 time_rest;
    int cntActiveClient;
  };

  struct TAnswerRequest
  {
    int cntClient;
    TDefClient* mArrClient;
    int cntRoom;
    TDefRoom*   mArrRoom;
    TAnswerRequest()
    {
      cntClient = 0;
      mArrClient = NULL;
      cntRoom = 0;
      mArrRoom = NULL;
    }
    ~TAnswerRequest()
    {
      cntClient = 0;
      delete mArrClient;
      mArrClient = NULL;
      cntRoom = 0;
      delete mArrRoom;
      mArrRoom = NULL;
    }
  };

	ServerTank(int numNetWork = 0);
	~ServerTank();

	void start();
	void stop();

  void requestListClient();

  void registerRequest(CallBackRegistrator::TCallBackFunc func){mCallBackRequestListClient.Register(func);};
  void unregisterRequest(CallBackRegistrator::TCallBackFunc func){mCallBackRequestListClient.Unregister(func);};
  
protected:

  void SendPacket_A_InFight(nsServerStruct::TClient* pClient);

  void Notify(void* data,unsigned int size){mCallBackRequestListClient.Notify(data,size);};

	void ThreadStreamAnswer();
	void StartThread();
  void StopThread();


  void WaitRestTime();

	friend void CallBackFuncServerRcvPacket(void* data, int size);
  friend void CallBackFuncServerRcvStream(void* data, int size);
  void AddPacket(TIP_Port* ip_port, TBasePacket* packet);
  void AddStream(TIP_Port* ip_port, TBasePacket* packet);

	friend void CallBackFuncServerDisconnect(void* data, int size);
	void DisconnectClient(TIP_Port* ip_port);

	void WorkListDisconnect();
	void WorkCmdRequest();
	void WorkStream();
	void WorkRefreshListClient();

  // подробная обработка запросов
	unsigned char RegisterClient(TR_Try_Connect_To_Server* tryConnect, unsigned int ip, unsigned short port);
  void WorkBalanser();
  void MakeRoom();
  unsigned short GenerateID_Map();
	
	void SendEcho(unsigned int ip, unsigned short port);

  static int ServerTank::SortFresh(const void* p1, const void* p2);
  
  volatile bool flgNeedRequest;
  void WorkRequest();

protected:

	bool FindNickInArrClient(char* sNick);

  bool SetListTank(TA_Get_List_Tank *answerListTank, unsigned int ip, unsigned short port);


  void WriteTransport(nsServerStruct::TClient* pClient,TBasePacket *packet);
  void WriteTransport(nsServerStruct::TPacketServer* pServerPacket,TBasePacket *packet);
  void WriteTransport(unsigned int ip, unsigned short port,TBasePacket *packet);

  void WriteTransportS(nsServerStruct::TClient* pClient,TBasePacket *packet);
  void WriteTransportS(nsServerStruct::TPacketServer* pServerPacket,TBasePacket *packet);
  void WriteTransportS(unsigned int ip, unsigned short port,TBasePacket *packet);

protected:
	// обработчики WorkCmdRequest()
	int Appl_Type_R_Try_Connect_To_Server(nsServerStruct::TPacketServer**ppPacket, TBasePacket* pPacket);
	int Appl_Type_R_Get_List_Tank(nsServerStruct::TPacketServer**ppPacket, TBasePacket* pPacket);
	int Appl_Type_C_Choose_Tank(nsServerStruct::TPacketServer**ppPacket, TBasePacket* pPacket);
	int Appl_Type_R_In_Fight(nsServerStruct::TPacketServer**ppPacket, TBasePacket* pPacket);
	int Appl_Type_R_Exit_Wait(nsServerStruct::TPacketServer**ppPacket, TBasePacket* pPacket);
	int Appl_Type_R_Exit_Fight(nsServerStruct::TPacketServer**ppPacket, TBasePacket* pPacket);
  int RoomHandler(nsServerStruct::TPacketServer**ppPacket, TBasePacket* pPacket);
};

#endif