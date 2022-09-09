/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012
===========================================================================
                        Common Information
"Tanks" GPL Source Code

This file is part of the "Tanks" GPL Source Code.

"Tanks" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"Tanks" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "Tanks" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "Tanks" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "Tanks" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 

#ifndef UdpDeviceH
#define UdpDeviceH

#include "classTools.h"
#include "SaveOnHDD.h"

// Класс, реализующий прием/отправку данных по протоколу UDP
class UdpDevice
{
  TSaveOnHDD mLog;

  int mSocket;
  void*       olRead;     // OVERLAPPED*
  void*       olWrite;    // OVERLAPPED*
  int	        eventRead;  // идентификатор события OVERLAPPED
  int	        eventWrite; // идентификатор события OVERLAPPED


public:
  struct TParams 
  {
    unsigned long m_LocalHost;  // IP локального хоста
    short m_LocalPort;          // Номер порта локального хоста
  };
protected:
  TParams m_Params;
public:
  // Устанавливает параметры конфигурации порта, которые будут использованы
  // при его открытии
	void setPortSettings( const TParams &params ){m_Params=params;}
  const TParams& portSetting() { return m_Params; }		// параметры порта

public:
	UdpDevice();
	~UdpDevice();
	void close(void);
	bool open();
  int read(void *buffer, unsigned long len, unsigned long timeout, unsigned int &ip, unsigned short &port);
  bool write( const void* buf, unsigned long size, unsigned int ip, unsigned short port);
  bool isOpen(void);
private:

  // Сокет для соединения:
	// дескриптор устройства == дескриптор файла == дескриптор сокета
	// так-же используется для определения открытости устройства.
  union
  {
    void*   m_Socket_void; 
    int     m_Socket_int; 
    unsigned long  m_Socket_uint; 
  };

	NO_COPY_OBJECT(UdpDevice);
};
#endif // !defined(EA_1739A613_F421_4f4a_BABC_19C44CED496A__INCLUDED_)
