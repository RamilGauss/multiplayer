#include <winsock2.h> 
#include <conio.h> 
#include <time.h> 
#include <stdio.h>
#include "HiTimer.h"
#include "NetSystem.h"
#include "glib/gthread.h"

#pragma comment(lib, "ws2_32.lib") 

#define SOCKET_ERRNO   WSAGetLastError() 
#define ADDRESS "192.168.23.226"//"127.0.0.1" 
#define PORT 1234

SOCKET ahSocket[1]; 

SOCKET ConnectToPort() 
{ 
   SOCKET hSocket = socket( PF_INET, SOCK_STREAM, 0 ); 
   if( hSocket == INVALID_SOCKET ) 
   { 
      printf( "socket() error %d\n", SOCKET_ERRNO ); 
      exit(1); 
   } 
   // Connexion setting for local connexion 
   struct sockaddr_in addr; 
	 addr.sin_family = AF_INET ; 
   addr.sin_addr.s_addr = inet_addr(ADDRESS); 
   addr.sin_port = htons(6666); 

	 // bind socket 
	 if( bind( hSocket, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR ) 
	 { 
		 printf( "bind() error %d\n", SOCKET_ERRNO ); 
		 exit(1); 
	 } 
	 addr.sin_family = AF_INET ; 
	 addr.sin_addr.s_addr = inet_addr(ADDRESS); 
	 addr.sin_port = htons (PORT); 

   // Connect 
   if( connect( hSocket, (struct sockaddr *)&addr, sizeof(addr) ) == SOCKET_ERROR ) 
   { 
      if( SOCKET_ERRNO != WSAEWOULDBLOCK ) 
      { 
         printf( "connect() error (%d)\n", SOCKET_ERRNO ); 
         exit(1); 
      } 
   } 
   return hSocket; 
} 
//-------------------------------------------------------------------------------------------
void* ThreadTransport(void*p)
{
	int i = 0;
	while(true)
	{
		ht_msleep(40);
		char szBuffer[256]; int cbBuffer; 
		// create string and return the size 
		cbBuffer = sprintf_s( szBuffer, "Hello, World!%d", i ); 
		// send the string with 0 at the end 
		int resSend = send( ahSocket[0], szBuffer, cbBuffer + 1, 0 ); 
		if(SOCKET_ERROR ==  resSend) 
		{ 
			printf("Send FAIL!!! 0x%X\n",  WSAGetLastError()); 
			exit(1);
		} 
   	printf( "Send ok %d  %d\n", resSend, i); 
		i++;
	}
	return NULL;
}
//-------------------------------------------------------------------------------------------
#include "NetDeviceTCP.h"
int main() 
{ 
  // Initialize Winsock 
  ns_Init();
  //###
  TNetDeviceTCP tcp;
  int s2 = tcp.Open(true, 12345);
  int s1 = tcp.Open(false, 12345);
  char* sLocalHost = ns_getSelfIP(0);
  unsigned long ip_server = ns_inet_addr(sLocalHost); 
  unsigned short port_server = 6666;
  bool resCon = tcp.Connect(s1, ip_server, port_server);
  if(resCon==false) printf( "connect() error %d\n", SOCKET_ERRNO ); 
  char buffer[1000];
  bool resSend = tcp.Send(s1, buffer, sizeof(buffer), ip_server, port_server);
  //###

	WSAEVENT ahEvents[1]; 
	DWORD dwEvent; 
	WSANETWORKEVENTS NetworkEvents; 

	// Create event 
	ahEvents[0] = WSACreateEvent(); 
	// Create and connect a socket on the server socket 
	ahSocket[0] = ConnectToPort(); 

	// the application wants to receive notification of a completed connection 
	int rc = WSAEventSelect(ahSocket[0], ahEvents[0], FD_CONNECT  ); 
	if( rc == SOCKET_ERROR ) 
	{ 
		printf( "WSAEventSelect() error %d\n", SOCKET_ERRNO ); 
		return 0; 
	} 
	while(true) 
  { 
    dwEvent = WSAWaitForMultipleEvents( 1, ahEvents, FALSE, 20, FALSE); 
    switch (dwEvent) 
    { 
			case WSA_WAIT_FAILED: 
				 printf("WSAEventSelect: %d\n", WSAGetLastError()); 
				 break; 
			case WAIT_IO_COMPLETION: 
			case WSA_WAIT_TIMEOUT: 
				 break; 
			default: 
				printf("while\n"); 
				dwEvent -= WSA_WAIT_EVENT_0; 
				if(SOCKET_ERROR == WSAEnumNetworkEvents(ahSocket[dwEvent], ahEvents[dwEvent], &NetworkEvents)) 
				{ 
					printf("WSAEnumNetworkEvent: %d lNetworkEvent %X\n", WSAGetLastError(), NetworkEvents.lNetworkEvents); 
					NetworkEvents.lNetworkEvents = 0; 
				} 
				else  
				{ 
							if(FD_CONNECT & NetworkEvents.lNetworkEvents) 
							{ 
								 //connection is OK 
								 printf( "FD_CONNECT ok (dwEvent=%d)\n", dwEvent ); 
								 // now that we are connected we want to send data or be aware when the other socket is disconnected 
								 rc = WSAEventSelect(ahSocket[dwEvent], ahEvents[dwEvent], FD_CLOSE | FD_WRITE ); 
								 if( rc == SOCKET_ERROR ) 
								 { 
										printf( "WSAEventSelect() error %d\n", SOCKET_ERRNO ); 
										return 0;
								 } 
							} 
							if (FD_CLOSE & NetworkEvents.lNetworkEvents) 
							{ 
								 printf( "FD_CLOSE ok (dwEvent=%d)\n", dwEvent ); 
								 printf( "press a key to exit\n" ); 
								 _getch(); 
								 WSACloseEvent( ahEvents[0] ); 
								 return 0; 
							} 
							if(FD_WRITE & NetworkEvents.lNetworkEvents) 
							{ 
								g_thread_init( NULL );
								g_thread_create(ThreadTransport, NULL, false, NULL);
								//char szBuffer[256]; int cbBuffer; 
								//printf( "FD_WRITE ok (dwEvent=%d)\n", dwEvent ); 
								//// create string and return the size 
								//cbBuffer = sprintf_s( szBuffer, "Hello, World!", dwEvent ); 
								//// send the string with 0 at the end 
								//rc = send( ahSocket[dwEvent], szBuffer, cbBuffer + 1, 0 ); 
								//if(SOCKET_ERROR ==  rc) 
								//{ 
								//	printf("WSAEnumNetworkEvent: %d lNetworkEvent %X\n",  WSAGetLastError(), NetworkEvents.lNetworkEvents); 
								//} 
							} 

					 } 
    } 
  } 
  return 0;
}
/*
#include <conio.h>
#include <stdio.h>
#include <map>
#include <string>
#include <iostream>
#include <windows.h>

#include "MakerNetTransport.h"
#include "INetTransport.h"
#include "..\Share\GlobalParams.h"
#include "..\glib-2.0\glib\gthread.h"
#include "..\GBaseLib\ErrorReg.h"
#include "..\GBaseLib\HiTimer.h"
#include "..\share\NetSystem.h"
#include "..\CheckTransport_Share\share_test.h"
#include "SaveOnHDD.h"
#include "TimerLog.h"
#include "ListPtr.h"
#include "DoserPacket.h"
#include "DoserProtocolEnum.h"
#include "DoserProtocolPacket.h"
#include "BreakPacket.h"
#include "LoadFromHDD.h"
#include <functional>
#include "Queue2Thread.h"

#define TIMER_LOG_ENABLE

using namespace std;

int numNetWork = 0;

struct TArgData
{
  unsigned int ip;
  int cnt;
  int time_sleep;
  TArgData()
  {
    ip = ns_inet_addr(ns_getSelfIP(numNetWork));
    cnt = 1470;//190;
    time_sleep = 20;
  }
};
//-------------------------------------------------------------------
void GetByArg(int argc, char** argv, TArgData &d);
//-------------------------------------------------------------------
int main(int argc, char** argv)
{
  Init();
  TL_START(true);

  INetTransport* pNetTransport = g_MakerNetTransport.New();TL_POINT("Make Net");
  pNetTransport->InitLog("client.txt");TL_POINT("InitLog");

  bool res = pNetTransport->Open(PORT_CLIENT);TL_POINT("Open");
  pNetTransport->Register(Recv,       INetTransport::eRecv);
  pNetTransport->Register(Disconnect, INetTransport::eDisconnect);
	pNetTransport->Register(LostPacket, INetTransport::eLostPacket);

  TArgData d;
  GetByArg(argc,argv,d);
  printf("TimeSleep=%d,cnt=%d\n",d.time_sleep, d.cnt);

  TBreakPacket packetForSend;
	if(pNetTransport->Synchro(d.ip, PORT_SERVER))
	{
		pNetTransport->Start(); TL_POINT("Start");

		TL_POINT("Before send");
		unsigned int start = ht_GetMSCount();

		for(int i = 0 ; i < CNT_RECV_PACKET ;)
		{
			for(int j = 0 ; j < d.cnt ; j++ )
			{
        packetForSend.UnlinkPart();
        packetForSend.PushFront(&packet[0], sizeof(packet));
        pNetTransport->Send(d.ip, PORT_SERVER, packetForSend, true);
				i++;
				if(i==CNT_RECV_PACKET)
					break;
			}
			ht_msleep(d.time_sleep);
		}
		start = ht_GetMSCount() - start;

		TL_POINT("Send");
		TL_PRINT("end",true);

		printf("time=%d ms, v=%f \n",start,float(sizeof(packet)*CNT_RECV_PACKET)/(start*1000));
		printf("cntLostPacket=%d\n",GetCntLostPacket());
		_getch();

		pNetTransport->Stop();
	}

  pNetTransport->Unregister(Recv,       INetTransport::eRecv);
  pNetTransport->Unregister(Disconnect, INetTransport::eDisconnect);
	pNetTransport->Unregister(LostPacket, INetTransport::eLostPacket);

  g_MakerNetTransport.Delete(pNetTransport);

  return 0;
}
//-----------------------------------------------------------------------
void GetByArg(int argc, char** argv, TArgData &d)
{
  if(argc<2) return;
  d.ip = ns_inet_addr(argv[1]);
  if(argc<4) return;
  d.time_sleep = atoi(argv[2]);
  d.cnt = atoi(argv[3]);
}
//-----------------------------------------------------------------------
*/