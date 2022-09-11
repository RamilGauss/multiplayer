/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
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

// �������-������� ��� inet_addr()
SHARE_EI unsigned long ns_inet_addr( const char* addr );

// �������-������� ��� inet_ntoa()
SHARE_EI char* ns_str_addr( unsigned long addr );

// ������������� ��� ����� ��� ������ � ��� ip-������� � �����
// ���������: �������� ��� ������ � ������� ������������� ���� ��� INADDR_NONE (-1)
unsigned long ns_HostOrIPtoAddr( const char* hostOrIp );

// ����������� �������� �� ��������� � ������� ������� ����
unsigned short SHARE_EI ns_htons( unsigned short value );
unsigned long  ns_htonl( unsigned long value );

// ����������� �������� �� �������� � �������� ������� ����
unsigned short SHARE_EI ns_ntohs( unsigned short value );
unsigned long  ns_ntohl( unsigned long value );

// �������� ������� ����� ��� �������� �������� � �������� ������
bool get_ip_for_net_interface( const char* interface_name, char* out_buf );  

// ����� ������� ���������� � ������ ���������� ethN
bool get_ip_first_eth(char* out_buf);

#endif
