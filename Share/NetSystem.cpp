/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
√удаков –амиль —ергеевич 
2011, 2012
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



// ќпределенные проблемы может вызвать перенос программ из Unix в Windows,
// так как там вместо read и write используютс€ операторы recv и send,
// вместо ioctl - ioctlsocket, а вместо close - closesocket.
//#define FD_SETSIZE      2

#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32)

  //необходимо, т.к. #include <winsock2.h> -> #include <pshpack4.h> нарушает выравнивание
  #if !defined(WIN32)
    #define WIN32
  #endif

  #if defined(__BORLANDC__)
    #pragma option push -w
      #pragma warn -8061
      #pragma warn -8060
      #include <winsock2.h>
      #include <Ws2tcpip.h>
    #pragma option pop
  #else
    #include <winsock2.h>
    #include <Ws2tcpip.h>
  #endif

  #include "ErrorReg.h"

  #define ioctl  ioctlsocket
  #define close  closesocket

  #define TYPE_SO_BROADCAST BOOL

  #define GET_ERROR()         WSAGetLastError()
  #define SET_ERR_NET( err )  errSDK_SET( err )
#else
  #include <sys/socket.h>
  #include <sys/types.h>
  #include <netinet/in.h>
  #include <netdb.h>
  #include <sys/ioctl.h>
  #include <unistd.h>
  #include <errno.h>
  #include <arpa/inet.h>
  #include "ErrorReg.h"
  #include <string.h>
  #include <stdlib.h>
  #include <stdio.h>

  #include <features.h>
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <sys/ioctl.h>
  #include <netinet/in.h>
  #include <net/if.h>
  #include <net/if_arp.h>
  #include <errno.h>
  #include <fcntl.h>
  #include <ctype.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
  #include <netdb.h>

  #ifndef SOCKET
    #define SOCKET int
  #endif
  
  #define INVALID_SOCKET  -1
  #define SOCKET_ERROR    -1

  #define TYPE_SO_BROADCAST int


  #define GET_ERROR()         h_errno
  #define SET_ERR_NET( err )  errSTD_SET( err )

#endif

#include "NetSystem.h"
#include "BL_Debug.h"
//---------------------------------------------------------------------------
#include <glib.h>
#include <stdio.h>
template <class TItem> class TThreadStorage
{
  GPrivate* key;
public:
  TThreadStorage()  { key = NULL; }
  TItem* get()
  {
    if( !key ) key = g_private_new( g_free );
    TItem* item = (TItem*)g_private_get( key );

    if ( !item )
    {
      item = g_new( TItem, 1 );
      g_private_set( key, item );
    }
    return item;
  }
  size_t size()
    { return sizeof(TItem); }
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

static void* errSource;
static bool ns_IsInit;
bool ns_Init()
{
  if( ns_IsInit ) return true;
  ns_IsInit = true;
  errSource = err_RegisterSource( "—етева€ служба" );
#if defined(TD_WINDOWS)
  WSADATA data;
  #if defined(__BORLANDC__)
  #pragma option push -w
    #pragma warn -8084
  if( WSAStartup( MAKEWORD(2,0), &data ) )
  #pragma option pop
  #else
      if( WSAStartup( MAKEWORD(2,0), &data ) )
  #endif
  {
    errSDK_SETSOURCE( WSAGetLastError(), errSource );
    return false;
  }
  else
    if( LOBYTE( data.wVersion ) != 2 )
    {
      errStr_SETSOURCE( "WinSock2 не поддерживаетс€ системой", errSource );
      return false;
    }
#endif
  return true;
}
//---------------------------------------------------------------------------
void ns_Done()
{
#if defined(TD_WINDOWS)
  WSACleanup();
#endif
}
//---------------------------------------------------------------------------
/*void ns_getaddrinfo()
{
  BL_Log( "//..........................." );
  BL_Log( "//...  ns_getaddrinfo()  ...." );
  BL_Log( "//..........................." );
  char name[255];
  if( !gethostname( name, sizeof(name) ) )
  {
    struct addrinfo aiHints;
    memset( &aiHints, 0, sizeof(aiHints) );
    aiHints.ai_family = AF_INET;
    aiHints.ai_socktype = SOCK_STREAM;
    aiHints.ai_protocol = IPPROTO_TCP;
    struct addrinfo *aiList = NULL;
    if( !getaddrinfo( "127.0.0.1", NULL, &aiHints, &aiList) != 0 )
    //  if( !getaddrinfo( name, NULL, &aiHints, &aiList) != 0 )
    {
      addrinfo * curAddr = aiList;
      while( curAddr )
      {
        if( curAddr->ai_family == AF_INET )
        {
          sockaddr_in* sai = (sockaddr_in*)(curAddr->ai_addr);
          char* ip = inet_ntoa( sai->sin_addr );
          if( ip )
          {
            BL_Log( "ip:" );
            BL_Log( ip );
          }
        }
        curAddr = curAddr->ai_next;
      }
      freeaddrinfo( aiList );
    }
  }
  BL_Log( "//..........................." );
}*/
//---------------------------------------------------------------------------
//получение ip-адреса по имени хоста
char* ns_getHostIP( const char* name, int numNetWork )
{
	int foundNet = 0;
  hostent* he = gethostbyname( name );
  if( he )
  {
    bool isLocalHost = false;
    char* ip = NULL;
    struct sockaddr ** ppAddr = (sockaddr **)he->h_addr_list;
    int i = 0;
    while( ppAddr[i] )
    {
      ip = inet_ntoa( *(in_addr*)ppAddr[i] );
      if( ip )
      {
        if( strcmp( ip, "127.0.0.1" ) == 0 )
          isLocalHost = true;
        else
					if(numNetWork==foundNet)
						return ip;
					else
						foundNet++;
      }
      else SET_ERR_NET( GET_ERROR() );
      i++;
    }
    if( isLocalHost )
      return "127.0.0.1";
  }
  return NULL;
}
//---------------------------------------------------------------------------
//получение ip-адреса
char* ns_getSelfIP(int numNetWork)
{
  char name[255];
  if( !gethostname( name, sizeof(name) ) )
  {
    return ns_getHostIP( name, numNetWork );
  }
  else SET_ERR_NET( GET_ERROR() );
  return NULL;
}
//---------------------------------------------------------------------------
//получение имени хоста
char* ns_getSelfHost()
{
  typedef char THostName[255];
  TThreadStorage<THostName> hostname;

  if( !gethostname( (char*)hostname.get(), hostname.size() ) )
  {
    return (char*)hostname.get();
  }
  else SET_ERR_NET( GET_ERROR() );
  return NULL;
}
//---------------------------------------------------------------------------
//получение сетевой маски
char* ns_getNetMask( const char* ip_str )
{
  typedef char TIPString[16];
  TThreadStorage<TIPString> result;

  if( ip_str )
  {
    const char* pSrc = ip_str;

    char* pDest = (char*)result.get();
    char* pPoint = NULL;
    int count = -1;
    while( (unsigned int)++count <= strlen(ip_str) )
    {
      *pDest = *pSrc++;
      if( *pDest++ == '.' ) pPoint = pDest;
    }
    if( pPoint ) // указывает на символ, следующий после '.' или NULL
    {
      *pPoint++ = '2';*pPoint++ = '5';*pPoint++ = '5';
      *pPoint++ = '\0';
      return (char*)result.get();
    }
  }
  return NULL;
}
//---------------------------------------------------------------------------
//получение сетевой маски свой сети
char* ns_getSelfNetMask()
{
  return ns_getNetMask( ns_getSelfIP() );
}
//---------------------------------------------------------------------------
unsigned long ns_inet_addr( const char* addr )
{
  return inet_addr( addr );
}
//---------------------------------------------------------------------------
char* ns_str_addr( unsigned long addr )
{
  sockaddr_in addrOuter;
  addrOuter.sin_addr.s_addr = addr;
  return inet_ntoa(addrOuter.sin_addr);
}
//---------------------------------------------------------------------------
unsigned long ns_HostOrIPtoAddr( const char* hostOrIp )
{
  // интерпретируем hostOrIp как ip-адрес хоста и преобразуем его в число
  unsigned long ret = hostOrIp ? inet_addr( hostOrIp ) : INADDR_NONE;  
  if( ret == INADDR_NONE )
  {  
    // интерпретируем hostOrIp как им€ хоста и запрашиваем его ip-адрес
    const char* ip_str = ns_getHostIP( hostOrIp );
    if( ip_str )
      ret = inet_addr( ip_str );
  }
  return ret;
}
//---------------------------------------------------------------------------
//  онвертаци€ значени€ из машинного в сетевой пор€док байт
unsigned short ns_htons( unsigned short value )
{
  return htons( value );
}
unsigned long ns_htonl( unsigned long value )
{
  return htonl( value );
}
//---------------------------------------------------------------------------
//  онвертаци€ значени€ из сетевого в машинный пор€док байт
unsigned short ns_ntohs( unsigned short value )
{
  return ntohs( value );
}
unsigned long ns_ntohl( unsigned long value )
{
  return ntohl( value );
}
//-----------------------------------------------------------------------------
// получить сетевой адрес дл€ сетевого адаптера с заданным именем
bool get_ip_for_net_interface( const char* interface_name, char* out_buf )
{
  #if defined(__WIN32__) || defined(_WIN32) || defined(WIN32)
  #else
    // получаем сокет дл€ IPv4
    int fd = socket( AF_INET, SOCK_DGRAM, 0 );
    if( fd >= 0 ) 
    {
      // забиваем название интерфейса и название семейства протоколов
      struct ifreq ifr;
      strcpy( ifr.ifr_name, interface_name );
      ifr.ifr_addr.sa_family = AF_INET;
      
      // обращаемс€ к устройству и получаем информацию о сетевом адресе
      if( !ioctl(fd, SIOCGIFADDR, &ifr) ) 
      {
        struct sockaddr_in* addr = (struct sockaddr_in*)(&ifr.ifr_addr);
        sprintf( out_buf, inet_ntoa( addr->sin_addr ) );
      }
    }
  #endif

  return strlen( out_buf ) != 0;
}
//-----------------------------------------------------------------------------
bool get_ip_first_eth(char* out_buf)
{
  char interface_name[100];
  for(int i = 0 ; i < 3/*???*/ ; i++)
  {
    sprintf(interface_name,"eth%d",i);
    bool res = get_ip_for_net_interface((const char*)interface_name,out_buf);
    if(res) return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
