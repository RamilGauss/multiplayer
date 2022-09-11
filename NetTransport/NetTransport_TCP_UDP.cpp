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

using namespace std;
using namespace nsNetTransportStruct;

//----------------------------------------------------------------------------
TNetTransport_TCP_UDP::TNetTransport_TCP_UDP(char* pPathLog) : INetTransport(pPathLog)
{

}
//----------------------------------------------------------------------------------
TNetTransport_TCP_UDP::~TNetTransport_TCP_UDP()
{

}
//----------------------------------------------------------------------------------
bool TNetTransport_TCP_UDP::Open(unsigned short port, unsigned char numNetWork)
{

	return false;
}
//----------------------------------------------------------------------------------
void TNetTransport_TCP_UDP::Send(unsigned int ip, unsigned short port, 
							                   TBreakPacket& packet, bool check)
{

}
//----------------------------------------------------------------------------------
void* ThreadTransportTU(void*p)
{
	((TNetTransport_TCP_UDP*)p)->Engine();
	return NULL;
}
//----------------------------------------------------------------------------------
void TNetTransport_TCP_UDP::Engine()
{
	flgNeedStop = false;
	flgActive = true;
	while(!flgNeedStop)
	{

	}
	flgActive = false;
}
//----------------------------------------------------------------------------------
void TNetTransport_TCP_UDP::Start()
{
  thread = g_thread_create(ThreadTransportTU,
    (gpointer)this,
    true,
    NULL);
	while(IsActive()==false)
		ht_msleep(eWaitActivation);
}
//----------------------------------------------------------------------------------
void TNetTransport_TCP_UDP::Stop()
{
	flgNeedStop = true;
	while(IsActive())
	{
		ht_msleep(eWaitActivation);
	}
	mUDP.close();
}
//----------------------------------------------------------------------------------
bool TNetTransport_TCP_UDP::IsActive()
{
	return flgActive;
}
//--------------------------------------------------------------------------
void TNetTransport_TCP_UDP::Register(TCallBackRegistrator::TCallBackFunc pFunc, eTypeCallback type)
{
  switch(type)
  {
		case eRecv:
      mCallBackRecv.Register(pFunc);
      break;
		case eLostPacket:
			mCallBackLostPacket.Register(pFunc);
			break;
    case eDisconnect:
      mCallBackDisconnect.Register(pFunc);
      break;
  }
}
//----------------------------------------------------------------------------------
void TNetTransport_TCP_UDP::Unregister(TCallBackRegistrator::TCallBackFunc pFunc, eTypeCallback type)
{
  switch(type)
  {
		case eRecv:
			mCallBackRecv.Unregister(pFunc);
			break;
		case eLostPacket:
			mCallBackLostPacket.Unregister(pFunc);
			break;
		case eDisconnect:
			mCallBackDisconnect.Unregister(pFunc);
			break;
  }
}
//----------------------------------------------------------------------------------
bool TNetTransport_TCP_UDP::Synchro(unsigned int ip, unsigned short port)
{
  return false;
}
//----------------------------------------------------------------------------------
void TNetTransport_TCP_UDP::InitLog(char* pPathLog)
{  
	if(pPathLog)
	{		
		char pPathLogEvent[400];
		sprintf(pPathLogEvent,"%sEvent",pPathLog);

		mLogRcvSend.ReOpen(pPathLog);
		mLogEvent.ReOpen(pPathLogEvent);
	}
}
//----------------------------------------------------------------------------------
