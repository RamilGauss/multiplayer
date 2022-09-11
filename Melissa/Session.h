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

#ifndef MELISSA_SESSION_H
#define MELISSA_SESSION_H

#include "BreakPacket.h"
#include "INetTransport.h"

namespace nsMelissa
{
  class TSession
  {
    unsigned int mTimeLive;// ��
    unsigned int mID;
    TIP_Port mIP_Port;
    unsigned int mLastTimeActive;
    INetTransport* mTransport;
  protected:
    friend class TManagerSession;

    enum{
      eEcho   = 'e',
      ePacket = 'p',
    };
    struct THeader
    {
      char type;
      THeader(char t = ePacket)
      {
        type = t;
      }
    };
  public:
    TSession(unsigned int time_live_ms);
    ~TSession();
    
    void Work();
    void Send(TBreakPacket bp, bool check = true);
    void SetTransport(INetTransport* pTransport);
    void GetInfo(TIP_Port& pDesc);
    void SetInfo(TIP_Port& pDesc);
    unsigned int GetID(){return mID;}
    void SetID(unsigned int id){mID=id;}
    void Recv();
  protected:
    void SendEcho();
    void RefreshLastTime();
    void SendData(char type, TBreakPacket& bp, bool check = true);
  };
}

#endif