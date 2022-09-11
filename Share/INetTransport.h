/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef INetTransportH
#define INetTransportH

#define STR_NAME_NET_TRANSPORT "NT"

#include "CallBackRegistrator.h"
#include "TypeDef.h"
#include "ShareMisc.h"
#include "BreakPacket.h"

// ���������
class SHARE_EI INetTransport
{
public:
  //���� callback �������
	typedef enum{
		eTcp  = 0,		   
		eUdp  = 1,
	}eTypeRecv;
  // callback ������ ������ ���������
  struct TDescRecv
  {
    TIP_Port ip_port;
    char* data;
    int   sizeData;
		eTypeRecv type;
  };

  INetTransport();
  virtual ~INetTransport();
  virtual bool Open(unsigned short port, unsigned char numNetWork = 0) = 0;

  // � �������� ������������ ������ ��������� ������, �������
  // �������� ������� ������
	virtual void Send(unsigned int ip, unsigned short port,
                    TBreakPacket packet, bool check = true) = 0;

	// ������ - ���������������
  virtual TCallBackRegistrator1<TDescRecv*>* GetCallbackRecv()       = 0;
  virtual TCallBackRegistrator1<TIP_Port* >* GetCallbackDisconnect() = 0;

	// ����� � ���� ������
	virtual void Start() = 0;
	virtual void Stop()  = 0;
	virtual bool IsActive() = 0;

  // ���������� �������
	// �������� �� ������ Start()
  virtual bool Connect(unsigned int ip, unsigned short port) = 0; // ����� ������ ��� �������

	virtual void Close(unsigned int ip, unsigned short port) = 0;
};


#endif