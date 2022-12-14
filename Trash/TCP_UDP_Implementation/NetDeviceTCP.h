/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
??????? ?????? ????????? 
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


#ifndef NetDeviceTCPH
#define NetDeviceTCPH

#include "INetDevice.h"

class TNetDeviceTCP : public INetDevice
{
  enum{
    eCountListen = 2000,
  };

public:
  TNetDeviceTCP();
  virtual ~TNetDeviceTCP();

  virtual int Open(bool flgListen, unsigned short port, unsigned char numNetWork = 0 );// ret local sock
  virtual bool Connect(int sock_local, unsigned int ip, unsigned short port);
  virtual int  Accept(int sock_local, unsigned int& ip, unsigned short& port);// ret remote socket
  
  virtual void Close(int sock);

  virtual int  Read(int sock, char* buffer, int len, 
    unsigned int &ip, unsigned short &port);
  virtual bool Send(int sock, char* buffer, int  size, 
    unsigned int ip, unsigned short port);

protected:
  void OffNagl( int sock );
  void SetReUse( int sock);
  void Set_HardClose( int sock );
};


#endif