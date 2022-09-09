/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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

#ifndef MELISSA_ITRANSPORT_H
#define MELISSA_ITRANSPORT_H

#include "CallBackRegistrator.h"

namespace nsMelissa
{
  class MELISSA_EI ITransport
  {

  public:
    //���� callback �������
    typedef enum{
      eRcvPacket  = 0,		   
      eRcvStream  = 1,
      eDisconnect = 2,
    }tTypeEventTransport;

    ITransport(){}
    virtual ~ITransport(){}
    
    virtual void InitLog(char* pPathLog) = 0;
    virtual bool Open(unsigned char subNet, unsigned short port ) = 0;
   	virtual void Write(unsigned int ip, unsigned short port, void* packet, int size, bool check = true) = 0;
    virtual void Register(TCallBackRegistrator::TCallBackFunc pFunc, int type) = 0;
    virtual void Unregister(TCallBackRegistrator::TCallBackFunc pFunc, int type) = 0;
    virtual void Start() = 0;
    virtual void Stop()  = 0;
    virtual bool Synchro(unsigned int ip, unsigned short port) = 0; // ����� ������ ��� �������
  };
}

#endif