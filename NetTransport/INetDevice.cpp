/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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

#include "INetDevice.h"
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
#include "BL_Debug.h"

#include "Logger.h"
#include "INetTransport.h"

#ifdef WIN32
  #define GET_ERROR WSAGetLastError()
#else
  #define GET_ERROR errno
#endif

#define PRINTF(X,F) GetLogger()->Get(STR_NAME_NET_TRANSPORT)->WriteF_time(X,F)
#define PRINTF_0(X) PRINTF(X,0)


bool INetDevice::SetRecvBuffer(int sock, unsigned int size)
{
	int res = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)&size, sizeof(int));
	bool resSet = ( res != SOCKET_ERROR );
  if(resSet==false)
  {
    PRINTF("SetRecvBuffer FAIL %u.\n",GET_ERROR);
  }
	return resSet;
}
//-----------------------------------------------------------------------------
bool INetDevice::SetSendBuffer(int sock, unsigned int size)
{
  int res = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char*)&size, sizeof(int));
  bool resSet = ( res != SOCKET_ERROR );
  if(resSet==false)
  {
    PRINTF("SetSendBuffer FAIL %u.\n",GET_ERROR);
  }
  return resSet;
}
//-----------------------------------------------------------------------------
unsigned int INetDevice::GetMaxSizeBufferForSocket(int sock)
{
	unsigned int size = 0;
	int retSize = sizeof(int);
	int resGet = getsockopt(sock, SOL_SOCKET, SO_MAX_MSG_SIZE, (char*)&size, &retSize);
	if( resGet != SOCKET_ERROR ) size = 0;
  if(resGet==false)
  {
    PRINTF("GetMaxSizeBufferForSocket FAIL %u.\n",GET_ERROR);
  }
	return size;
}
//-----------------------------------------------------------------------------
INetDevice::eTypeSocket INetDevice::GetType(int sock)
{
  int type;
  int retSize = sizeof(type);
  int resGet = getsockopt(sock, SOL_SOCKET, SO_TYPE, (char*)&type, &retSize);
  switch(type)
  {
    case SOCK_STREAM:
      return eTypeTCP;
    case SOCK_DGRAM:
      return eTypeUDP;
  }

  PRINTF("GetType Socket FAIL %u.\n",GET_ERROR);
  return eUndefType;
}
//-----------------------------------------------------------------------------
