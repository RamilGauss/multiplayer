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

#include "NetTransport_TCP_UDP.h"
#include "common_defs.h"
#include "HiTimer.h"
#include "NetSystem.h"

#ifdef WIN32
  #include "NetMakerEventWin.h"
#else
#endif

#include "NetControlTCP.h"
#include "NetControlUDP.h"

using namespace std;

//----------------------------------------------------------------------------
TNetTransport_TCP_UDP::TNetTransport_TCP_UDP()
{
#ifdef WIN32
  mNetMakerEvent = new TNetMakerEventWin;
#else
#endif

  mTCP = new TNetControlTCP;
  mUDP = new TNetControlUDP;

  INetControl::SetMakerEvent(mNetMakerEvent);
}
//----------------------------------------------------------------------------------
TNetTransport_TCP_UDP::~TNetTransport_TCP_UDP()
{
  Done();
}
//----------------------------------------------------------------------------------
void TNetTransport_TCP_UDP::Done()
{
  delete mNetMakerEvent;
  mNetMakerEvent = NULL;
  delete mTCP;
  mTCP = NULL;
  delete mUDP;
  mUDP = NULL;
}
//----------------------------------------------------------------------------------
bool TNetTransport_TCP_UDP::Open(unsigned short port, unsigned char numNetWork)
{
  bool res = true;
  res &= mTCP->Open(port, numNetWork);
  res &= mUDP->Open(port, numNetWork);
  return res;
}
//----------------------------------------------------------------------------------
void TNetTransport_TCP_UDP::Send(unsigned int ip, unsigned short port, 
							                   TBreakPacket packet, bool check)
{
  lock();
  if(check)
    mTCP->Send(ip, port, packet);
  else
    mUDP->Send(ip, port, packet);
  unlock();
}
//----------------------------------------------------------------------------------
void TNetTransport_TCP_UDP::Start()
{
  mNetMakerEvent->Start();
}
//----------------------------------------------------------------------------------
void TNetTransport_TCP_UDP::Stop()
{
  mNetMakerEvent->Stop();
}
//----------------------------------------------------------------------------------
bool TNetTransport_TCP_UDP::IsActive()
{
	return mNetMakerEvent->IsActive();
}
//--------------------------------------------------------------------------
void TNetTransport_TCP_UDP::Register(TCallBackRegistrator::TCallBackFunc pFunc, eTypeCallback type)
{
  INetControl::Register(pFunc, type);
}
//----------------------------------------------------------------------------------
void TNetTransport_TCP_UDP::Unregister(TCallBackRegistrator::TCallBackFunc pFunc, eTypeCallback type)
{
  INetControl::Unregister(pFunc, type);
}
//----------------------------------------------------------------------------------
bool TNetTransport_TCP_UDP::Connect(unsigned int ip, unsigned short port)
{
  lock();
  bool res = mTCP->Connect(ip, port);
  unlock();
  return res;
}
//----------------------------------------------------------------------------------
void TNetTransport_TCP_UDP::Close(unsigned int ip, unsigned short port)
{
	mTCP->Close(ip, port);
}
//----------------------------------------------------------------------------------
