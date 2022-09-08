///////////////////////////////////////////////////////////
//  UdpDevice.h
//  Implementation of the Class UdpDevice
//  Created on:      19-окт-2009 13:17:58
//  Original author: KostMJ
///////////////////////////////////////////////////////////

#if !defined(EA_1739A613_F421_4f4a_BABC_19C44CED496A__INCLUDED_)
#define EA_1739A613_F421_4f4a_BABC_19C44CED496A__INCLUDED_

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

	NO_COPY_CLASS(UdpDevice);
};
#endif // !defined(EA_1739A613_F421_4f4a_BABC_19C44CED496A__INCLUDED_)
