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

SHARE_EI char* ns_getHostIP( const char* name, int numNetWork = 0 ); // получение ip-адреса по имени хоста
SHARE_EI char* ns_getSelfIP(int numNetWork=0);                   // получение ip-адреса

// функция-обертка для inet_addr()
SHARE_EI unsigned long ns_inet_addr( const char* addr );

// функция-обертка для inet_ntoa()
SHARE_EI char* ns_str_addr( unsigned long addr );

// преобразовать имя носта или строку с его ip-адресом в число
// Результат: двоичный код адреса с сетевым расположением байт или INADDR_NONE (-1)
unsigned long ns_HostOrIPtoAddr( const char* hostOrIp );

// Конвертация значения из машинного в сетевой порядок байт
unsigned short SHARE_EI ns_htons( unsigned short value );
unsigned long  ns_htonl( unsigned long value );

// Конвертация значения из сетевого в машинный порядок байт
unsigned short SHARE_EI ns_ntohs( unsigned short value );
unsigned long  ns_ntohl( unsigned long value );

// получить сетевой адрес для сетевого адаптера с заданным именем
bool get_ip_for_net_interface( const char* interface_name, char* out_buf );  

// поиск первого доступного с именем интерфейса ethN
bool get_ip_first_eth(char* out_buf);

#endif
