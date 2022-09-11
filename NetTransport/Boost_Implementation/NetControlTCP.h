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


#ifndef NetControlTCPH
#define NetControlTCPH

#include <map>

#include "INetControl.h"
#include "NetDeviceTCP.h"
#include "GCS.h"
#include "HistoryPacketTCP.h"

class TNetControlTCP : public INetControl
{
  TNetDeviceTCP mDevice;// для соединения с сервером
  THistoryPacketTCP mHistory;

  enum{
       eSizeBuffer = 64000,  
       eWaitConnect = 40,// мс
			 eTimeRepeatSend = 20,// мс
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