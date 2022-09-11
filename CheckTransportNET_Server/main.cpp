#include <winsock2.h> 
#include <Windows.h> 
#include <conio.h> 
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib") 

#define SOCKET_ERRNO WSAGetLastError() 
#define ADDRESS "127.0.0.1" 
#define PORT 1234 

static SOCKET ListenFirstFreePort() 
{ 
	struct sockaddr_in addr; 
	int len = sizeof(addr);    
	SOCKET hSocket; 
	// Create socket 
	hSocket = socket( PF_INET, SOCK_STREAM, 0 ); 
	if( hSocket == INVALID_SOCKET ) 
	{ 
		printf( "socket() error %d\n", SOCKET_ERRNO ); 
		exit(1); 
	} 
	// Connection setting for local connexion 
	addr.sin_family = AF_INET ; 
	addr.sin_addr.s_addr = inet_addr(ADDRESS); 
	addr.sin_port = htons (PORT); 
	// bind socket 
	if( bind( hSocket, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR ) 
	{ 
		printf( "bind() error %d\n", SOCKET_ERRNO ); 
		exit(1); 
	} 
	// listen 
	if( listen( hSocket, 100) == SOCKET_ERROR ) 
	{ 
		printf( "listen() error %d\n", SOCKET_ERRNO ); 
		exit(1); 
	} 
	return hSocket; 
} 
//----------------------------------------------------
void main() 
{ 
	WSADATA stack_info; 
	SOCKET ahSocket[2]; 
	WSAEVENT ahEvents[2]; 
	DWORD dwEvent; 
	WSANETWORKEVENTS NetworkEvents; 
	int rc; 
	// Initialize Winsock 
	WSAStartup(MAKEWORD(2,0), &stack_info) ; 

	// Create events 
	ahEvents[0] = WSACreateEvent(); 
	ahEvents[1] = WSACreateEvent(); 

	// Create listening socket 
	ahSocket[0] = ListenFirstFreePort(); 
	rc = WSAEventSelect(ahSocket[0], ahEvents[0], FD_ACCEPT ); 
	if( rc == SOCKET_ERROR ) 
	{ 
		printf( "WSAEventSelect() error %d\n", SOCKET_ERRNO ); 
		exit(1); 
	} 
	while (TRUE) 
	{ 
		// look for events 
		dwEvent = WSAWaitForMultipleEvents( 2, ahEvents, FALSE, WSA_INFINITE, FALSE); 
		switch (dwEvent) 
		{ 
			case WSA_WAIT_FAILED: 
				printf("WSAEventSelect: %d\n", WSAGetLastError()); 
				break; 
			case WAIT_IO_COMPLETION: 
			case WSA_WAIT_TIMEOUT: 
				break; 
			default: 
				//if there is one dwEvent-WSA_WAIT_EVENT_0 has to be substracted so as to dwEvent correspond to the index of the concerned socket 
				dwEvent -= WSA_WAIT_EVENT_0; 
				// enumeration of the events on the socket[dwEvent] 
				if (SOCKET_ERROR == WSAEnumNetworkEvents(ahSocket[dwEvent], ahEvents[dwEvent], &NetworkEvents)) 
				{ 
					printf("WSAEnumNetworkEvent: %d lNetworkEvent %X\n", 
						WSAGetLastError(), NetworkEvents.lNetworkEvents); 
					NetworkEvents.lNetworkEvents = 0; 
				} 
				else  
				{ 
					if(FD_CLOSE & NetworkEvents.lNetworkEvents) 
					{ 
						printf( "FD_CLOSE ok (dwEvent=%d)\n", dwEvent ); 
						printf( "press a key to exit\n" ); 
						_getch(); // require conio.h 
						WSACloseEvent( ahEvents[0] ); 
						WSACloseEvent( ahEvents[1] ); 
						exit(0); 
					} 
					if (FD_READ & NetworkEvents.lNetworkEvents) 
					{ 
						char szBuffer[256]; int cbRecv; 
						// Only the second socket expect to receive data 
						printf( "FD_READ ok (dwEvent=%d)\n", dwEvent ); 
						// read data 
						cbRecv = recv( ahSocket[dwEvent], szBuffer, sizeof(szBuffer) - 1, 0 ); 
						if( cbRecv <= 0 ) 
						{ 
							printf( "recv() error %d\n", SOCKET_ERRNO ); 
							exit(1); 
						} 
						// we put the 0 in case it has been cut - unlikey to happen on local network 
						szBuffer[cbRecv] = 0; 
						// write data in console window 
						printf( "socket %d : '%s'\n", dwEvent, szBuffer ); 
					} 
				} 
				if (FD_ACCEPT & NetworkEvents.lNetworkEvents) 
				{ 
					struct sockaddr_in addrAccept; 
					int lenAccept; 
					lenAccept = sizeof( addrAccept ); 
					// we should have dwEvent=0 
					printf( "accept ok (dwEvent=%d)\n", dwEvent ); 
					// we create another socket to accept the connexion with the client socket 
					ahSocket[1] = accept(ahSocket[dwEvent], (struct sockaddr *)&addrAccept, &lenAccept); 
					// we want to be informed on when we'll be able read data from it 
					rc = WSAEventSelect(ahSocket[1], ahEvents[1], FD_READ|FD_CLOSE  ); 
					if( rc == SOCKET_ERROR ) 
					{ 
						printf( "WSAEventSelect() error %d\n", SOCKET_ERRNO ); 
						exit(1); 
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
#include <windows.h>
#include "..\NetTransport\MakerNetTransport.h"
#include "INetTransport.h"
#include "..\Share\GlobalParams.h"
#include "..\glib-2.0\glib\gthread.h"
#include "..\GBaseLib\ErrorReg.h"
#include "..\GBaseLib\HiTimer.h"
#include "..\share\NetSystem.h"
#include "SaveOnHDD.h"
#include "..\CheckTransport_Share\share_test.h"

using namespace std;
//-----------------------------------------------------------------------
int numNetWork = 0;
int main(int argc, char** argv)
{
  Init();

  INetTransport* pNetTransport = g_MakerNetTransport.New();
  pNetTransport->InitLog("server.txt");

  bool res = pNetTransport->Open(PORT_SERVER);
  pNetTransport->Register(Recv,       INetTransport::eRecv);
  pNetTransport->Register(Disconnect, INetTransport::eDisconnect);

  pNetTransport->Start();

  _getch();

  pNetTransport->Stop();
  pNetTransport->Unregister(Recv,       INetTransport::eRecv);
  pNetTransport->Unregister(Disconnect, INetTransport::eDisconnect);

  g_MakerNetTransport.Delete(pNetTransport);
  return 0;
}
*/