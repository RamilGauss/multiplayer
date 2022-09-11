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

#include <boost/thread/thread.hpp>
#include <algorithm>

#include "NetMakerEventWin.h"
#include "NetMakerEventThreadWSA.h"

#include "HiTimer.h"
#include "INetControl.h"
#include "BL_Debug.h"
#include "Logger.h"

#define PRINTF(X,F) GetLogger()->Get(STR_NAME_NET_TRANSPORT)->WriteF_time(X,F)
#define PRINTF_0(X) PRINTF(X,0)

#define GET_ERROR WSAGetLastError()

using namespace std;
using namespace nsNetTypeEvent;

TNetMakerEventThreadWSA::TNetMakerEventThreadWSA(TNetMakerEventWin* pMaster, int maxCountSocket)
{
  mCntSocket = 0;
  mVecEvent.reserve(maxCountSocket); 
  mVecDSock.reserve(maxCountSocket);

  mMaster     = pMaster;
  flgActive   = false;
  flgNeedStop = false;
}
//-----------------------------------------------------------------
TNetMakerEventThreadWSA::~TNetMakerEventThreadWSA()
{
  Stop();
  Done();
}
//-----------------------------------------------------------------
void* NetMakerEventThread_Thread(void*p)
{
  ((TNetMakerEventThreadWSA*)p)->Engine();
  return NULL;
}
//----------------------------------------------------------------------------------
void TNetMakerEventThreadWSA::Engine()
{
  flgNeedStop = false;
  flgActive = true;
  while(!flgNeedStop)
  {
    lockSleep();
    Work();
    unlockSleep();
  }
  flgActive = false;
}
//----------------------------------------------------------------------------------
void TNetMakerEventThreadWSA::Start()
{
  boost::thread work_thread(NetMakerEventThread_Thread, this);

  while(IsActive()==false)
    ht_msleep(eWaitFeedBack);
}
//----------------------------------------------------------------------------------
void TNetMakerEventThreadWSA::Stop()
{
  flgNeedStop = true;
  while(IsActive())
    ht_msleep(eWaitFeedBack);
}
//----------------------------------------------------------------------------------
void TNetMakerEventThreadWSA::Work()
{
  if(mCntSocket)
    WaitSocketEvent();
  else
    ht_msleep(eWaitFeedBack);
}
//----------------------------------------------------------------------------------
void TNetMakerEventThreadWSA::Done()
{
  while(mVecDSock.size())
  {
    int sock = mVecDSock.begin()->sock;
    mVecDSock.begin()->pControl->Close(sock);
    // после закрытия на устройстве - можно удалять
    Remove(sock);
  }
}
//----------------------------------------------------------------------------------
void TNetMakerEventThreadWSA::WaitSocketEvent()
{
  WSAEVENT* pEvents = (WSAEVENT*)&mVecEvent[0]; 
  int cEvents = mVecEvent.size();
  DWORD dwEvent = WSAWaitForMultipleEvents( cEvents, pEvents, FALSE, eTimeRefreshEngine, FALSE); 
  switch(dwEvent)
  { 
    case WSA_WAIT_FAILED: 
      PRINTF("WaitSocketEvent() WSAWaitForMultipleEvents FAIL %u.\n", GET_ERROR);
      break; 
    case WAIT_IO_COMPLETION: 
    case WSA_WAIT_TIMEOUT: 
      break; 
    default: 
    {
      dwEvent -= WSA_WAIT_EVENT_0; 
      int sock = mVecDSock[dwEvent].sock;
      WSANETWORKEVENTS NetworkEvents;
      int res = WSAEnumNetworkEvents( sock, pEvents[dwEvent], &NetworkEvents);
      // формируем список событий
      list<eTypeEvent> lEvent;
      WSA_Event2Control( NetworkEvents.lNetworkEvents, lEvent);
      // отдать на обработку
      mMaster->HandleFromWorkThread(this, mVecDSock[dwEvent].pControl, sock, lEvent);
    }
  }
}
//----------------------------------------------------------------------------------
int TNetMakerEventThreadWSA::GetCount()
{
  return mCntSocket;
}
//----------------------------------------------------------------------------------
void TNetMakerEventThreadWSA::Add(int sock, INetControl* pControl, list<eTypeEvent>& lEvent)
{
  // подстраховка
  TMapII_It fit = mMapSockEvent.find(sock);
  if(fit!=mMapSockEvent.end()) 
    return;
  //-------------------------------------------------
  int event = (int)WSACreateEvent();
  if((HANDLE)event==WSA_INVALID_EVENT)
  {
    PRINTF("WSA: create event FAIL %u.\n", GET_ERROR);
    return;
  }
  //-------------------------------------------------
  TDescSocket ds;
  ds.sock     = sock;
  ds.pControl = pControl;
  mVecDSock.push_back(ds);
  mVecEvent.push_back(event);
  mMapSockEvent.insert(TMapIntInt::value_type(sock, event));
  mCntSocket++;

  SetTypeEvent(sock, lEvent);
}
//-----------------------------------------------------------------
void TNetMakerEventThreadWSA::Remove(int sock)
{
  // удалить из map
  TMapII_It fit = mMapSockEvent.find(sock);
  if(fit==mMapSockEvent.end()) 
    return;

  TVecInt::iterator fit_event = find(mVecEvent.begin(), mVecEvent.end(), fit->second);
  if(fit_event==mVecEvent.end())
  {
    PRINTF_0("Remove from Vec event. Not found event.\n");
    return;
  }
  int index = fit_event - mVecEvent.begin();

  BOOL resClose = WSACloseEvent((WSAEVENT)*fit_event);
  if(resClose==FALSE)
    PRINTF("Remove from Vec event. Close event FAIL %u.\n", GET_ERROR);

  mVecEvent.erase(fit_event);
  mVecDSock.erase(mVecDSock.begin()+index);
  mMapSockEvent.erase(fit);
  mCntSocket--;
}
//-----------------------------------------------------------------
void TNetMakerEventThreadWSA::SetTypeEvent( int sock, list<eTypeEvent>& lEvent)
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
void TNetMakerEventThreadWSA::Sleep()
{
  lockSleep();
}
//----------------------------------------------------------------------------------
void TNetMakerEventThreadWSA::WakeUp()
{
  unlockSleep();
}
//----------------------------------------------------------------------------------
void TNetMakerEventThreadWSA::WSA_Event2Control( int event, list<eTypeEvent>& lEvent)
{
  lEvent.clear();
  if(event&FD_READ)
    lEvent.push_back(eRead);
  if(event&FD_WRITE)
    lEvent.push_back(eWrite);
  if(event&FD_ACCEPT)
    lEvent.push_back(eAccept);
  if(event&FD_CONNECT)
    lEvent.push_back(eConnect);
  if(event&FD_CLOSE)
    lEvent.push_back(eClose);
}
//----------------------------------------------------------------------------------
void TNetMakerEventThreadWSA::Control2WSA_Event( list<eTypeEvent>& lEvent, int& event )
{
  event = 0;
  list<eTypeEvent>::iterator bit = lEvent.begin();
  list<eTypeEvent>::iterator eit = lEvent.end();
  while(bit!=eit)
  {
    switch(*bit)
    {
      case eRead:
        event |= FD_READ;
        break;
      case eWrite:
        event |= FD_WRITE;
        break;
      case eConnect:
        event |= FD_CONNECT;
        break;
      case eAccept:
        event |= FD_ACCEPT;
        break;
      case eClose:
        event |= FD_CLOSE;
        break;
    }
    bit++;
  }
}
//----------------------------------------------------------------------------------
