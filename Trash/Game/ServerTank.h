/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
??????? ?????? ????????? 
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
the "Tanks" Source Code.  If not, please request a copy in writing from  at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef ServerTankH
#define ServerTankH


#include "TransportProtocol.h"
#include <glib/gthread.h>
#include "List.h"
#include "ServerStruct.h"
#include "CallBackRegistrator.h"
#include "ApplicationProtocolPacketRequest.h"
#include "StatisticValue.h"
#include "ApplicationProtocolPacketAnswer.h"
#include "Client.h"

class ServerTank
{

  typedef TStatType_double TStatLoad;
  TStatLoad statistica;


	TSaveOnHDD mLogLoad;
	
	TCallBackRegistrator mCallBackRequestListClient; // 

	TTransportProtocol mTransport;

	GThread* threadStreamAnswerCommand;

	friend void* ThreadProcServer( void* p);

  bool flgActive;
  bool flgNeedStop;

  enum{eWaitThread    = 100, // ??
       eTimeSleep     = 100, // ??
       eMaxLoadServer = 75,  // ????????
  };


  guint32 mLastTime;

	// ?????? ??????
	TList<TRoom>  mListRoom;// ### ???????? ?? list<TRoom>
	// ?????? ????????
  TArrayObject mArrClients;

  // ?????? Delete ???????????? Unlink!
  TArrayObject mArrClientsFight;//????????? ?? ??? ??? ? ???
  TArrayObject mArrClientsWait;

  // ??????? ? ??????? ?? ????????? ??????????
  TList<nsServerStruct::TPacketServer> mListPacket;
  // ????? ?? ??????? (?????????? ???????)
  TList<nsServerStruct::TPacketServer> mListStream;
	// ?????????????? ??????????? ?????? ?? ??????????, ??????????? ????????????
	TList<TIP_Port> mListDisconnectClient;

public:

  // ???????
	struct TDefClient
	{
		char name[TClient::eMaxLenNick];
		unsigned int ip;
		unsigned int port;
		int state;
    bool flgDisconnect;
	};
  // ???????
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
      delete[] mArrClient;
      mArrClient = NULL;
      cntRoom = 0;
      delete[] mArrRoom;
      mArrRoom = NULL;
    }
  };

	ServerTank(int numNetWork = 0);
	virtual ~ServerTank();

	void start();
	void stop();

  void requestListClient();

  void registerRequest(TCallBackRegistrator::TCallBackFunc func){mCallBackRequestListClient.Register(func);};
  void unregisterRequest(TCallBackRegistrator::TCallBackFunc func){mCallBackRequestListClient.Unregister(func);};
  
protected:

  void SendPacket_A_InFight(TClient* pClient);

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

  // ????????? ????????? ????????
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


  void WriteTransport(TClient* pClient,TBasePacket *packet);
  void WriteTransport(nsServerStruct::TPacketServer* pServerPacket,TBasePacket *packet);
  void WriteTransport(unsigned int ip, unsigned short port,TBasePacket *packet);

  void WriteTransportS(TClient* pClient,TBasePacket *packet);
  void WriteTransportS(nsServerStruct::TPacketServer* pServerPacket,TBasePacket *packet);
  void WriteTransportS(unsigned int ip, unsigned short port,TBasePacket *packet);

protected:
	// ??????????? WorkCmdRequest()
	int Appl_Type_R_Try_Connect_To_Server(nsServerStruct::TPacketServer**ppPacket, TBasePacket* pPacket);
	int Appl_Type_R_Get_List_Tank(nsServerStruct::TPacketServer**ppPacket, TBasePacket* pPacket);
	int Appl_Type_C_Choose_Tank(nsServerStruct::TPacketServer**ppPacket, TBasePacket* pPacket);
	int Appl_Type_R_In_Fight(nsServerStruct::TPacketServer**ppPacket, TBasePacket* pPacket);
	int Appl_Type_R_Exit_Wait(nsServerStruct::TPacketServer**ppPacket, TBasePacket* pPacket);
	int Appl_Type_R_Exit_Fight(nsServerStruct::TPacketServer**ppPacket, TBasePacket* pPacket);
  int RoomHandler(nsServerStruct::TPacketServer**ppPacket, TBasePacket* pPacket);
};

#endif