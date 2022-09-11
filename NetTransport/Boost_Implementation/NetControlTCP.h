/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/


#ifndef NetControlTCPH
#define NetControlTCPH

#include <map>

#include "INetControl.h"
#include "NetDeviceTCP.h"
#include "GCS.h"
#include "HistoryPacketTCP.h"

class TNetControlTCP : public INetControl
{
  TNetDeviceTCP mDevice;// ��� ���������� � ��������
  THistoryPacketTCP mHistory;

  enum{
       eSizeBuffer = 64000,  
       eWaitConnect = 40,// ��
			 eTimeRepeatSend = 20,// ��
  };

  char mBuffer[eSizeBuffer];
  int mReadSize;

  volatile bool flgWaitConnect;
  volatile bool flgResConnect;

  GCS gcsSendAccept;
  void lockSA(){gcsSendAccept.lock();}
  void unlockSA(){gcsSendAccept.unlock();}
public:

  TNetControlTCP(boost::asio::io_service& io_service);
  virtual ~TNetControlTCP();

  // TNetTransport_XXX
  virtual void Init();
  virtual bool Open( unsigned short port, unsigned char numNetWork = 0);
  virtual bool Connect(unsigned int ip, unsigned short port);
  virtual void Send(unsigned int ip, unsigned short port, TBreakPacket bp);
  virtual void Close();
  virtual TNetDeviceTCP* GetDevice(){return &mDevice;}

  virtual char* GetBuffer(){return &mBuffer[0];}
  virtual int   GetSize()  {return eSizeBuffer;}
protected:
  void Done();

  // asio event
  void RecvEvent(const boost::system::error_code& error,size_t bytes_transferred);
  void SendEvent(const boost::system::error_code& error,size_t bytes_transferred);
  void ConnectEvent(const boost::system::error_code& error);

  void ReadyRecv();
  void RequestSend(char* data, int size);
  void RequestConnect(TIP_Port& ip_port);

  boost::asio::ip::tcp::endpoint mConnectEndpoint;
};


#endif