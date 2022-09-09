/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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


#ifndef NetSystemH
#define NetSystemH

#include "TypeDef.h"

#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32)

#else
  #define SOCKET          int
  #define INVALID_SOCKET  -1
  #define SOCKET_ERROR    -1

  #define closesocket     close  
#endif
bool SHARE_EI ns_Init();
void SHARE_EI ns_Done();

SHARE_EI char* ns_getHostIP( const char* name, int numNetWork = 0 ); // ��������� ip-������ �� ����� �����
SHARE_EI char* ns_getSelfIP(int numNetWork=0);                   // ��������� ip-������
SHARE_EI char* ns_getSelfHost();                 // ��������� ����� �����

//��������� ������� ����� �� ip-������
char* ns_getNetMask( const char* ip_str );

//��������� ������� ����� ���� ����
char* ns_getSelfNetMask();

// �������-������� ��� inet_addr()
SHARE_EI unsigned long ns_inet_addr( const char* addr );

// �������-������� ��� inet_ntoa()
SHARE_EI char* ns_str_addr( unsigned long addr );

// ������������� ��� ����� ��� ������ � ��� ip-������� � �����
// ���������: �������� ��� ������ � ������� ������������� ���� ��� INADDR_NONE (-1)
unsigned long ns_HostOrIPtoAddr( const char* hostOrIp );

// ����������� �������� �� ��������� � ������� ������� ����
unsigned short ns_htons( unsigned short value );
unsigned long  ns_htonl( unsigned long value );

// ����������� �������� �� �������� � �������� ������� ����
unsigned short ns_ntohs( unsigned short value );
unsigned long  ns_ntohl( unsigned long value );

// �������� ������� ����� ��� �������� �������� � �������� ������
bool get_ip_for_net_interface( const char* interface_name, char* out_buf );  

// ����� ������� ���������� � ������ ���������� ethN
bool get_ip_first_eth(char* out_buf);

#endif
