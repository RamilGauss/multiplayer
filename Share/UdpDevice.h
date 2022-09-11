/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef UdpDeviceH
#define UdpDeviceH

#include "classTools.h"
#include "SaveOnHDD.h"

// �����, ����������� �����/�������� ������ �� ��������� UDP
class SHARE_EI UdpDevice
{
  TSaveOnHDD mLog;

  int mSocket;
  void*       olRead;     // OVERLAPPED*
  void*       olWrite;    // OVERLAPPED*
  int	        eventRead;  // ������������� ������� OVERLAPPED
  int	        eventWrite; // ������������� ������� OVERLAPPED


public:
  struct TParams 
  {
    unsigned long m_LocalHost;  // IP ���������� �����
    short m_LocalPort;          // ����� ����� ���������� �����
  };
protected:
  TParams m_Params;
public:
  // ������������� ��������� ������������ �����, ������� ����� ������������
  // ��� ��� ��������
	void setPortSettings( const TParams &params ){m_Params=params;}
  const TParams& portSetting() { return m_Params; }		// ��������� �����

public:
	UdpDevice();
	virtual ~UdpDevice();
	void close(void);
	bool open();
  int read(void *buffer, unsigned long len, unsigned long timeout, unsigned int &ip, unsigned short &port);
  bool write( const void* buf, unsigned long size, unsigned int ip, unsigned short port);
  bool isOpen(void);

  bool SetRecvBuffer(unsigned int size);
  bool SetSendBuffer(unsigned int size);
  
  unsigned int GetMaxSizeBufferForSocket();// ???

  int GetSocket(){return mSocket;}

private:

  // ����� ��� ����������:
	// ���������� ���������� == ���������� ����� == ���������� ������
	// ���-�� ������������ ��� ����������� ���������� ����������.
  union
  {
    void*   m_Socket_void; 
    int     m_Socket_int; 
    unsigned long  m_Socket_uint; 
  };

	NO_COPY_OBJECT(UdpDevice);
};
#endif // !defined(EA_1739A613_F421_4f4a_BABC_19C44CED496A__INCLUDED_)
