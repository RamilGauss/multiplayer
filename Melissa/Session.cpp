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

#include "Session.h"
#include "HiTimer.h"

namespace nsMelissa
{

TSession::TSession(unsigned int time_live_ms )
{
  mTimeLive = time_live_ms;

  mLastTimeActive = ht_GetMSCount();
  mTransport = NULL;
}
//---------------------------------------------------------------------
TSession::~TSession()
{
}
//---------------------------------------------------------------------
void TSession::Work()
{
  unsigned int now_ms = ht_GetMSCount();
  if(mLastTimeActive + mTimeLive < now_ms)
  {
    SendEcho();
    RefreshLastTime();
  }
}
//---------------------------------------------------------------------
void TSession::Send(TBreakPacket bp, bool check)
{
  SendData(ePacket, bp, check);
  RefreshLastTime();
}
//---------------------------------------------------------------------
void TSession::SetTransport(INetTransport* pTransport)
{
  mTransport = pTransport;
}
//---------------------------------------------------------------------
void TSession::GetInfo(TIP_Port& pDesc)
{
  pDesc = mIP_Port;
}
//---------------------------------------------------------------------
void TSession::SetInfo(TIP_Port& pDesc)
{
  mIP_Port = pDesc;
}
//---------------------------------------------------------------------
void TSession::Recv()
{
  RefreshLastTime();
}
//---------------------------------------------------------------------
void TSession::SendEcho()
{
  TBreakPacket bp;
  SendData(eEcho, bp);
}
//---------------------------------------------------------------------
void TSession::RefreshLastTime()
{
  mLastTimeActive = ht_GetMSCount();
}
//---------------------------------------------------------------------
void TSession::SendData(char type, TBreakPacket& bp, bool check)
{
  THeader h(type);
  bp.PushFront((char*)&h, sizeof(THeader));
  mTransport->Send(mIP_Port.ip, mIP_Port.port, bp, check);
}
//---------------------------------------------------------------------
}

