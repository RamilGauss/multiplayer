#include <winsock2.h> 
#include <conio.h> 
#include <time.h> 
#include <stdio.h>
#include "HiTimer.h"

#pragma comment(lib, "ws2_32.lib") 

#define SOCKET_ERRNO   WSAGetLastError() 
#define ADDRESS "127.0.0.1" 
#define PORT 1234 

SOCKET ConnectToPort() 
{ 
   struct sockaddr_in addr; 
   SOCKET hSocket; 
   u_long arg; int err; 

   // Create socket 
   hSocket = socket( PF_INET, SOCK_STREAM, 0 ); 
   if( hSocket == INVALID_SOCKET ) 
   { 
      printf( "socket() error %d\n", SOCKET_ERRNO ); 
      exit(1); 
   } 
   // Connexion setting for local connexion 
   addr.sin_family = AF_INET ; 
   addr.sin_addr.s_addr = inet_addr(ADDRESS); 
   addr.sin_port = htons (PORT); 

   // Connect 
	 unsigned int start = ht_GetMSCount();
   if( connect( hSocket, (struct sockaddr *)&addr, sizeof(addr) ) == SOCKET_ERROR ) 
   { 
			start = ht_GetMSCount() - start;	
      // As we are in non-blocking mode we'll always have the error 
      // WSAEWOULDBLOCK whichis actually not one 
      if( SOCKET_ERRNO != WSAEWOULDBLOCK ) 
      { 
         printf( "connect() error (%d)\n", SOCKET_ERRNO ); 
         exit(1); 
      } 
   } 
   return hSocket; 
} 

void main() 
{ 
//####
	//{
	//	struct sockaddr_in addr; 
	//	// Connection setting for local connexion 
	//	addr.sin_family = PF_INET; 
	//	addr.sin_addr.s_addr = inet_addr(ADDRESS); 
	//	addr.sin_port = htons (PORT); 

	//	SOCKET hSocketTCP = socket( PF_INET, SOCK_STREAM, IPPROTO_IP/*IPPROTO_TCP*/ ); //IPPROTO_IP
	//	if( hSocketTCP == INVALID_SOCKET ) 
	//	{ 
	//		printf( "socket() error %d\n", SOCKET_ERRNO ); 
	//		exit(1); 
	//	} 
	//	// bind socket 
	//	int resBind = bind( hSocketTCP, (struct sockaddr *)&addr, sizeof(addr));
	//  //----------------------------------------------------------------------------
	//	SOCKET hSocketUDP = socket( PF_INET, SOCK_DGRAM, IPPROTO_IP/*IPPROTO_UDP*/ ); //IPPROTO_IP
	//	if( hSocketUDP == INVALID_SOCKET ) 
	//	{ 
	//		printf( "socket() error %d\n", SOCKET_ERRNO ); 
	//		exit(1); 
	//	} 
	//	// bind socket 
	//	resBind = bind( hSocketUDP, (struct sockaddr *)&addr, sizeof(addr));
	//	int a = 0;
	//}

//####
   int initClockTime; 
   WSADATA stack_info; 
   SOCKET ahSocket[1]; 
   WSAEVENT ahEvents[1]; 
   DWORD dwEvent; 
   WSANETWORKEVENTS NetworkEvents; 
   int rc; 

   //// Initialize Winsock 
   WSAStartup(MAKEWORD(2,0), &stack_info) ; 
   // Create event 
   ahEvents[0] = WSACreateEvent(); 
   // Create and connect a socket on the server socket 
   ahSocket[0]= ConnectToPort(); 

   // the application wants to receive notification of a completed connection 
   rc = WSAEventSelect(ahSocket[0], ahEvents[0], FD_CONNECT  ); 
   if( rc == SOCKET_ERROR ) 
   { 
      printf( "WSAEventSelect() error %d\n", SOCKET_ERRNO ); 
      exit(1); 
   } 
   while (TRUE) 
   { 
      // look for events 
      dwEvent = WSAWaitForMultipleEvents( 1, ahEvents, FALSE, 1000, FALSE); 
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
					 //if there is one dwEvent-WSA_WAIT_EVENT_0 has to be substracted so as to dwEvent correspond to the index of the concerned socket 
					 dwEvent -= WSA_WAIT_EVENT_0; 
					 // enumeration of the events on the socket[dwEvent] 
					 if (SOCKET_ERROR == WSAEnumNetworkEvents(ahSocket[dwEvent], ahEvents[dwEvent], &NetworkEvents)) 
					 { 
							printf("WSAEnumNetworkEvent: %d lNetworkEvent %X\n", WSAGetLastError(), NetworkEvents.lNetworkEvents); 
							NetworkEvents.lNetworkEvents = 0; 
					 } 
					 else  
					 { 
							if (FD_CONNECT & NetworkEvents.lNetworkEvents) 
							{ 
								 //connection is OK 
								 printf( "FD_CONNECT ok (dwEvent=%d)\n", dwEvent ); 
								 // now that we are connected we want to send data or be aware when the other socket is disconnected 
								 rc = WSAEventSelect(ahSocket[dwEvent], ahEvents[dwEvent], FD_CLOSE | FD_WRITE ); 
								 if( rc == SOCKET_ERROR ) 
								 { 
										printf( "WSAEventSelect() error %d\n", SOCKET_ERRNO ); 
										exit(1); 
								 } 
							} 
							if (FD_CLOSE & NetworkEvents.lNetworkEvents) 
							{ 
								 printf( "FD_CLOSE ok (dwEvent=%d)\n", dwEvent ); 
								 printf( "press a key to exit\n" ); 
								 _getch(); 
								 WSACloseEvent( ahEvents[0] ); 
								 exit(0); 
							} 
							if(FD_WRITE & NetworkEvents.lNetworkEvents) 
							{ 
								 char szBuffer[256]; int cbBuffer; 
								 printf( "FD_WRITE ok (dwEvent=%d)\n", dwEvent ); 
								 // create string and return the size 
								 cbBuffer = sprintf( szBuffer, "Hello, World!", dwEvent ); 
								 // send the string with 0 at the end 
								 rc = send( ahSocket[dwEvent], szBuffer, cbBuffer + 1, 0 ); 
								 if(SOCKET_ERROR ==  rc) 
								 { 
										printf("WSAEnumNetworkEvent: %d lNetworkEvent %X\n",  WSAGetLastError(), NetworkEvents.lNetworkEvents); 
								 } 
								 // not sure if I have to use it 
								 //WSAResetEvent(ahEvents[0]); 
							} 

					 } 
      } 
   } 
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