/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef NetDeviceTCPH
#define NetDeviceTCPH

#include "INetDevice.h"
#include <boost/asio/ip/tcp.hpp>

class TNetDeviceTCP : public INetDevice
{
  boost::asio::ip::tcp::socket mSocket;
public:
  TNetDeviceTCP(boost::asio::io_service& io_service);
  virtual ~TNetDeviceTCP();

  virtual bool Open( unsigned short port, unsigned char numNetWork = 0 ); // ret local sock
  virtual void Close();

  virtual bool SetRecvBuffer(unsigned int size);
  virtual bool SetSendBuffer(unsigned int size);

  boost::asio::ip::tcp::socket* GetSocket(){return &mSocket;}
protected:
  void OffNagl();
  void SetReUse();
  void Set_HardClose();
};


#endif