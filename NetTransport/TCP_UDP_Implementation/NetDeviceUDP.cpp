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

#include "NetDeviceUDP.h"
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


TNetDeviceUDP::TNetDeviceUDP()
{

}
//--------------------------------------------------------------------------------
TNetDeviceUDP::~TNetDeviceUDP()
{

}
//--------------------------------------------------------------------------------
int TNetDeviceUDP::Open( bool flgListen, unsigned short port, unsigned char numNetWork )
{
#ifdef WIN32
  char* sLocalHost = ns_getSelfIP(numNetWork);
#else
  char sLocalHost[100];
  get_ip_first_eth(sLocalHost);
#endif
  unsigned long ip = ns_inet_addr(sLocalHost); 

  bool res = false;
  int sock = socket( AF_INET, SOCK_DGRAM, 0 );
  if( sock != INVALID_SOCKET )
  {
    sockaddr_in localAddr;
    memset( &localAddr, 0, sizeof(localAddr) ); // ???
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = ns_htons( port );
    localAddr.sin_addr.s_addr = ip;

    if(!bind( sock, (sockaddr*)&localAddr, sizeof(localAddr) ) == 0 )
    {
      Close(sock);
      PRINTF("Open UDP bind FAIL %u.\n",GET_ERROR);
    }
  } 
  else
  {
    PRINTF("Open UDP socket FAIL %u.\n",GET_ERROR);
  }

  return sock;
}
//--------------------------------------------------------------------------------
bool TNetDeviceUDP::Connect(int sock_local, unsigned int ip, unsigned short port)
{
  PRINTF_0("Try connect on UDP.\n");
  return false;
}
//--------------------------------------------------------------------------------
void TNetDeviceUDP::Close(int sock)
{
#ifdef WIN32
  closesocket( sock );
#else
  ::close( sock );
#endif
}
//--------------------------------------------------------------------------------
/**
 * Читает данные из последовательного устройства в буфер <buf> размером <size> байт.
 * Чтение прерывается если данные не поступили в течении timeout миллисекунд.
 * Чтение может быть прервано вызовом BreakRead ДО до истечения тайм-аута.
 * 
 * Возвращает > 0 - размер прочитанных данных
 * RR_TIMEOUT - если данных получено не было
 * RR_ERROR - если произошла ошибка чтения
 * RR_BREAK - если произошло асинхронное прерывание чтения
 */
int TNetDeviceUDP::Read(int sock, char* buffer, int len, 
                         unsigned int &ip, unsigned short &port)
{
  unsigned long szMsg;
l_checkQueue:
  if( ioctl( sock, FIONREAD, &szMsg ) == -1 )
  {
    PRINTF("Read UDP FAIL %u.\n", GET_ERROR);
    return RR_ERROR;
  }

  if( !szMsg )
  {
    fd_set rset;
    struct timeval tv;

    tv.tv_sec  = 0;
    tv.tv_usec = 0;

l_repeatRead:
    FD_ZERO(&rset);
    FD_SET(sock, &rset);

    int rc = (int)select( sock+1, &rset, NULL, NULL, &tv );
    if( !rc ) 
      return RR_TIMEOUT;

    if( rc < 0 )
    {
      if( errno == EINTR ) 
        goto l_repeatRead;

      PRINTF("Read UDP FAIL %u.\n", GET_ERROR);
      return RR_ERROR;
    }
    else goto l_checkQueue;
  }   

  if( len > int(szMsg) ) len = szMsg;

  sockaddr_in SenderAddr;
  int SenderAddrSize = sizeof(SenderAddr);

  int res = recvfrom(	sock,(char*)buffer, len, 0,(SOCKADDR *)&SenderAddr,&SenderAddrSize);

  ip   = SenderAddr.sin_addr.s_addr;
  port = ns_ntohs(SenderAddr.sin_port);
  if( res == -1 ) // == 
  {
    PRINTF("Read UDP FAIL %u.\n", GET_ERROR);
    return RR_ERROR;
  }

  return res;
}
//--------------------------------------------------------------------------------
bool TNetDeviceUDP::Send(int sock, char* buffer, int  size, unsigned int ip, unsigned short port)
{
  if( !size ) return true;
  sockaddr_in remoteAddr;
  remoteAddr.sin_family = AF_INET;
  remoteAddr.sin_port = ns_htons( port );
  remoteAddr.sin_addr.s_addr = ip;

l_repeat:
  int resSend = sendto( sock, (const char*)buffer, size, 0, (struct sockaddr*)&remoteAddr, sizeof(remoteAddr) );
  if(resSend==SOCKET_ERROR)
  {
    DWORD err = WSAGetLastError();
    if(err==WSAEWOULDBLOCK)
    {
      ht_msleep(eWaitCanWrite);// ждать когда канал даст возможность писать в него
      goto l_repeat;
    }
  }
  return (resSend == (int)size);
}
//--------------------------------------------------------------------------------
int TNetDeviceUDP::Accept(int sock_local, unsigned int& ip, unsigned short& port)
{
  PRINTF_0("Try accept on UDP FAIL %u.\n");
  return false;
}
//--------------------------------------------------------------------------------
