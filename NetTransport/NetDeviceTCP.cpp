/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012, 2013
===========================================================================
                        Common Information
"TornadoEngine" GPL Source Code

This file is part of the "TornadoEngine" GPL Source Code.

"TornadoEngine" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"TornadoEngine" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "TornadoEngine" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "TornadoEngine" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "TornadoEngine" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 
#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32)
  #include <Winsock2.h>
  #include <winsock.h>
  #include <mswsock.h>
  #include <ws2ipdef.h>
#endif
#include "NetSystem.h"
#include <stdio.h>

#ifndef WIN32
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  #include <netdb.h>
  #include <fcntl.h>
  #include <signal.h>
  #include <stdio.h>
  #include <sys/stat.h>
  #include <sys/uio.h>
  #include <sys/wait.h>
  #include <sys/un.h>
  #include <sys/ioctl.h>
#define SOCKET int
#else
#include <errno.h>
#define ioctl ioctlsocket
#endif
#include "common_defs.h"

#include "NetDeviceTCP.h"
#include "BL_Debug.h"
#include "HiTimer.h"

#include "Logger.h"
#include "INetTransport.h"

#ifdef WIN32
#define GET_ERROR WSAGetLastError()
#else
#define GET_ERROR errno
#endif

#define PRINTF(X,F) GetLogger()->Get(STR_NAME_NET_TRANSPORT)->WriteF_time(X,F)
#define PRINTF_0(X) PRINTF(X,0)

TNetDeviceTCP::TNetDeviceTCP()
{

}
//--------------------------------------------------------------------------------
TNetDeviceTCP::~TNetDeviceTCP()
{

}
//--------------------------------------------------------------------------------
int TNetDeviceTCP::Open( bool flgListen, unsigned short port, unsigned char numNetWork )
{
#ifdef WIN32
  char* sLocalHost = ns_getSelfIP(numNetWork);
#else
  char sLocalHost[100];
  get_ip_first_eth(sLocalHost);
#endif
  unsigned long ip = ns_inet_addr(sLocalHost); 

  int sock = socket( PF_INET, SOCK_STREAM, 0 ); 
  if( sock != INVALID_SOCKET ) 
  { 
    SetReUse(sock);
    OffNagl(sock);

    struct sockaddr_in addr; 
    addr.sin_family = AF_INET ; 
    addr.sin_addr.s_addr = ip; 
    addr.sin_port = ns_htons(port); 

    if( bind( sock, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR ) 
    {
      Close(sock);
      PRINTF("Open TCP bind FAIL %u.\n",GET_ERROR);
    }

    if(flgListen)
    {
      if(listen( sock, eCountListen)==SOCKET_ERROR)
      {
        PRINTF("Open TCP listen FAIL %u.\n",GET_ERROR);
        return SOCKET_ERROR;
      }
    }
  }
  else
  {
    PRINTF("Open TCP socket FAIL %u.\n",GET_ERROR);
  }

  return sock;
}
//--------------------------------------------------------------------------------
bool TNetDeviceTCP::Connect(int sock_local, unsigned int ip, unsigned short port)
{
  struct sockaddr_in addr; 
  addr.sin_family = AF_INET ; 
  addr.sin_addr.s_addr = ip; 
  addr.sin_port = ns_htons(port); 

  return (connect( sock_local, (struct sockaddr *)&addr, sizeof(addr) )==0);
}
//--------------------------------------------------------------------------------
void TNetDeviceTCP::Close(int sock)
{
#ifdef WIN32
  closesocket( sock );
#else
  ::close( sock );
#endif
}
//--------------------------------------------------------------------------------
int TNetDeviceTCP::Read(int sock, char* buffer, int len, 
                        unsigned int &ip, unsigned short &port)// ip and port were ignored
{
  int res = recv( sock, buffer, len, 0 ); 
	if(res==SOCKET_ERROR)
	{
    PRINTF("Read TCP FAIL %u.\n",GET_ERROR);
		return 0;
	}
	return res;
}
//--------------------------------------------------------------------------------
bool TNetDeviceTCP::Send(int sock, char* buffer, int size, 
                         unsigned int ip, unsigned short port)// ip and port were ignored
{
l_repeat:
  int resSend = send( sock, buffer, size, 0 );
  if(resSend==SOCKET_ERROR)
  {
    DWORD err = WSAGetLastError();
    if(err==WSAEWOULDBLOCK)
    {
      ht_msleep(eWaitCanWrite);// ждать когда канал даст возможность писать в него
      goto l_repeat;
    }
  }
  return (resSend!=SOCKET_ERROR);
}
//--------------------------------------------------------------------------------
int TNetDeviceTCP::Accept(int sock_local, unsigned int& ip, unsigned short& port)
{
  struct sockaddr_in addrAccept; 
  int lenAccept = sizeof( addrAccept ); 
  int sock_remote = accept(sock_local, (struct sockaddr *)&addrAccept, &lenAccept); 
  if(sock_remote==SOCKET_ERROR)
  {
    PRINTF("Accept TCP FAIL %u.\n",GET_ERROR);
  }
  OffNagl(sock_remote);
  port = ns_htons(addrAccept.sin_port);
  ip = addrAccept.sin_addr.s_addr;
  return sock_remote;
}
//--------------------------------------------------------------------------------
void TNetDeviceTCP::OffNagl( int sock )
{
  bool flg = true; 
  int resSet = setsockopt( sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&flg, sizeof(flg));
  if(resSet!=0)
  {
    PRINTF("OffNagl TCP FAIL %u.\n",GET_ERROR);
  }
}
//--------------------------------------------------------------------------------
void TNetDeviceTCP::SetReUse( int sock)
{
  bool flg = true; 
  int resSet = setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&flg, sizeof(flg));
  if(resSet!=0)
  {
    PRINTF("SetReUse TCP FAIL %u.\n",GET_ERROR);
  }
}
//--------------------------------------------------------------------------------
