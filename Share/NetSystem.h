#ifndef NetSystemH
#define NetSystemH

#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32)

#else
  #define SOCKET          int
  #define INVALID_SOCKET  -1
  #define SOCKET_ERROR    -1

  #define closesocket     close  
#endif
bool ns_Init();
void ns_Done();

char* ns_getHostIP( const char* name, int numNetWork = 0 ); // ��������� ip-������ �� ����� �����
char* ns_getSelfIP(int numNetWork=0);                   // ��������� ip-������
char* ns_getSelfHost();                 // ��������� ����� �����

//��������� ������� ����� �� ip-������
char* ns_getNetMask( const char* ip_str );

//��������� ������� ����� ���� ����
char* ns_getSelfNetMask();

// �������-������� ��� inet_addr()
unsigned long ns_inet_addr( const char* addr );

// �������-������� ��� inet_ntoa()
char* ns_str_addr( unsigned long addr );

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
