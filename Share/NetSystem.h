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

char* ns_getHostIP( const char* name, int numNetWork = 0 ); // получение ip-адреса по имени хоста
char* ns_getSelfIP(int numNetWork=0);                   // получение ip-адреса
char* ns_getSelfHost();                 // получение имени хоста

//получение сетевой маски по ip-адресу
char* ns_getNetMask( const char* ip_str );

//получение сетевой маски свой сети
char* ns_getSelfNetMask();

// функци€-обертка дл€ inet_addr()
unsigned long ns_inet_addr( const char* addr );

// функци€-обертка дл€ inet_ntoa()
char* ns_str_addr( unsigned long addr );

// преобразовать им€ носта или строку с его ip-адресом в число
// –езультат: двоичный код адреса с сетевым расположением байт или INADDR_NONE (-1)
unsigned long ns_HostOrIPtoAddr( const char* hostOrIp );

//  онвертаци€ значени€ из машинного в сетевой пор€док байт
unsigned short ns_htons( unsigned short value );
unsigned long  ns_htonl( unsigned long value );

//  онвертаци€ значени€ из сетевого в машинный пор€док байт
unsigned short ns_ntohs( unsigned short value );
unsigned long  ns_ntohl( unsigned long value );

// получить сетевой адрес дл€ сетевого адаптера с заданным именем
bool get_ip_for_net_interface( const char* interface_name, char* out_buf );  

// поиск первого доступного с именем интерфейса ethN
bool get_ip_first_eth(char* out_buf);

#endif
