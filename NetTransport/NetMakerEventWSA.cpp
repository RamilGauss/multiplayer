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

#include <winsock2.h>

#include "NetMakerEventWSA.h"
#include "HiTimer.h"
#include "INetControl.h"
//#include <winsock.h>


using namespace std;

TNetMakerEventWSA::TNetMakerEventWSA()
{
  flgActive   = false;
  flgNeedStop = false;
}
//-----------------------------------------------------------------
TNetMakerEventWSA::~TNetMakerEventWSA()
{

}
//-----------------------------------------------------------------
void TNetMakerEventWSA::Add(int sock, INetControl* pControl)
{
  //ahEvents[0] = WSACreateEvent(); 
}
//-----------------------------------------------------------------
void TNetMakerEventWSA::Remove(int sock)
{

}
//-----------------------------------------------------------------
void* ThreadMakerEventWSA(void*p)
{
  ((TNetMakerEventWSA*)p)->Engine();
  return NULL;
}
//----------------------------------------------------------------------------------
void TNetMakerEventWSA::Engine()
{
  flgNeedStop = false;
  flgActive = true;
  while(!flgNeedStop)
  {
    Work();
  }
  flgActive = false;
}
//----------------------------------------------------------------------------------
void TNetMakerEventWSA::Start()
{
  thread = g_thread_create(ThreadMakerEventWSA,
    (gpointer)this,
    true,
    NULL);
  while(IsActive()==false)
    ht_msleep(eWaitFeedBack);
}
//----------------------------------------------------------------------------------
void TNetMakerEventWSA::Stop()
{
  flgNeedStop = true;
  while(IsActive())
  {
    ht_msleep(eWaitFeedBack);
  }
}
//----------------------------------------------------------------------------------
void TNetMakerEventWSA::Work()
{
  WSAEVENT ahEvents[1]; 
  DWORD dwEvent = WSAWaitForMultipleEvents( 2, ahEvents, FALSE, eTimeRefreshEngine, FALSE); 
  //if(SOCKET_ERROR == WSAEnumNetworkEvents(ahSocket[dwEvent], ahEvents[dwEvent], &NetworkEvents)) 

  //WSAEVENT ahEvents[1]; 
  //DWORD dwEvent; 
  //WSANETWORKEVENTS NetworkEvents; 
}
//----------------------------------------------------------------------------------
void TNetMakerEventWSA::WSA_Event2Control( int event, list<INetControl::eTypeEvent>& lEvent)
{
  lEvent.clear();
  if(event&FD_READ)
    lEvent.push_back(INetControl::eRead);
  if(event&FD_WRITE)
    lEvent.push_back(INetControl::eWrite);
  if(event&FD_ACCEPT)
    lEvent.push_back(INetControl::eAccept);
  if(event&FD_CONNECT)
    lEvent.push_back(INetControl::eConnect);
  if(event&FD_CLOSE)
    lEvent.push_back(INetControl::eClose);
}
//----------------------------------------------------------------------------------
void TNetMakerEventWSA::Control2WSA_Event( list<INetControl::eTypeEvent>& lEvent, int& event )
{
  event = 0;
  list<INetControl::eTypeEvent>::iterator bit = lEvent.begin();
  list<INetControl::eTypeEvent>::iterator eit = lEvent.end();
  while(bit!=eit)
  {
    switch(*bit)
    {
      case INetControl::eRead:
        event |= FD_READ;
        break;
      case INetControl::eWrite:
        event |= FD_WRITE;
        break;
      case INetControl::eConnect:
        event |= FD_CONNECT;
        break;
      case INetControl::eAccept:
        event |= FD_ACCEPT;
        break;
      case INetControl::eClose:
        event |= FD_CLOSE;
        break;
    }
    bit++;
  }
}
//----------------------------------------------------------------------------------
void TNetMakerEventWSA::SetTypeEvent( int sock, list<INetControl::eTypeEvent>& lEvent)
{
  //int rc = WSAEventSelect(ahSocket[0], ahEvents[0], FD_CONNECT  ); 
}
//----------------------------------------------------------------------------------