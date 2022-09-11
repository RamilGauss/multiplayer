/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
√удаков –амиль —ергеевич 
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

#include <algorithm>

#include "NetMakerEventWSA.h"
#include "HiTimer.h"
#include "INetControl.h"
#include "BL_Debug.h"
#include "Logger.h"

#define PRINTF(X,F) GetLogger()->Get(STR_NAME_NET_TRANSPORT)->WriteF_time(X,F)
#define PRINTF_0(X) PRINTF(X,0)

#define GET_ERROR WSAGetLastError()

using namespace std;

TNetMakerEventWSA::TNetMakerEventWSA()
{
  flgActive   = false;
  flgNeedStop = false;
}
//-----------------------------------------------------------------
TNetMakerEventWSA::~TNetMakerEventWSA()
{
  Done();
}
//-----------------------------------------------------------------
void TNetMakerEventWSA::Add(int sock, INetControl* pControl)
{
  // подстраховка
  TMapII_It fit = mMapSockEvent.find(sock);
  if(fit!=mMapSockEvent.end()) 
    return;
  //-------------------------------------------------
  int reserveSize = mVecEvent.size() ? mVecEvent.size()*2 : eDefReserveSizeVector;
  int maxSize = mVecEvent.capacity();
  if( maxSize < reserveSize )
  {
    mVecEvent.reserve(reserveSize);
    mVecSocket.reserve(reserveSize);
  }
  //-------------------------------------------------
  int event = (int)WSACreateEvent();
  if((HANDLE)event==WSA_INVALID_EVENT)
  {
    PRINTF("WSA: create event FAIL %u.\n", GET_ERROR);
    return;
  }
  //-------------------------------------------------
  mMapSockEvent.insert(TMapII_It::value_type(sock, event));

  TDescSocket ds;
  ds.sock     = sock;
  ds.pControl = pControl;
  mVecSocket.push_back(ds);
  mVecEvent.push_back(event);
}
//-----------------------------------------------------------------
void TNetMakerEventWSA::Remove(int sock)
{
  // удалить из map
  TMapII_It fit = mMapSockEvent.find(sock);
  if(fit==mMapSockEvent.end()) 
    return;
  
  TVecInt::iterator fit_event = find(mVecEvent.begin(), mVecEvent.end(), fit->second);
  if(fit_event==mVecEvent.end())
  {
    PRINTF_0("Remove from Vec event. Not found event.\n");
    //BL_FIX_BUG();
    return;
  }
  int index = fit_event - mVecEvent.begin();
  
  BOOL resClose = WSACloseEvent((WSAEVENT)*fit_event);
  if(resClose==FALSE)
  {
    PRINTF("Remove from Vec event. Close event FAIL %u.\n", GET_ERROR);
    //BL_FIX_BUG();
  }
  mVecEvent.erase(fit_event);
  mVecSocket.erase(mVecSocket.begin()+index);
  mMapSockEvent.erase(fit);
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
  WSAEVENT* pEvents = (WSAEVENT*)&mVecEvent[0]; 
  int cEvents = mVecEvent.size();
  DWORD dwEvent = WSAWaitForMultipleEvents( cEvents, pEvents, FALSE, eTimeRefreshEngine, FALSE); 
  switch (dwEvent) 
  { 
    case WSA_WAIT_FAILED: 
      PRINTF("Work() WSAWaitForMultipleEvents FAIL %u.\n", GET_ERROR);
      break; 
    case WAIT_IO_COMPLETION: 
    case WSA_WAIT_TIMEOUT: 
      break; 
    default: 
    {
      dwEvent -= WSA_WAIT_EVENT_0; 
      int sock = mVecSocket[dwEvent].sock;
      WSANETWORKEVENTS NetworkEvents;
      int res = WSAEnumNetworkEvents( sock, pEvents[dwEvent], &NetworkEvents);
      // формируем список событий
      list<INetControl::eTypeEvent> lEvent;
      WSA_Event2Control( NetworkEvents.lNetworkEvents, lEvent);
      // отдать на обработку
      mVecSocket[dwEvent].pControl->Work(sock, lEvent);
    }
  }
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
  // найти по сокету соответствующее ему событие
  TMapII_It fit = mMapSockEvent.find(sock);
  if(fit==mMapSockEvent.end()) 
  {
    PRINTF_0("SetTypeEvent not found in mapSockEvent.\n");
    return;
  }
  WSAEVENT Events = (WSAEVENT)fit->second;

  // формируем маску событий
  int event;
  Control2WSA_Event(lEvent, event);

  int rc = WSAEventSelect( sock, Events, event ); 
  if(rc!=0)
  {
    PRINTF("SetTypeEvent WSAEventSelect FAIL %u.\n", GET_ERROR);
  }
}
//----------------------------------------------------------------------------------
void TNetMakerEventWSA::Done()
{
  while(mVecSocket.size())
  {
    int sock = mVecSocket.begin()->sock;
    Remove(sock);
  }
}
//----------------------------------------------------------------------------------
