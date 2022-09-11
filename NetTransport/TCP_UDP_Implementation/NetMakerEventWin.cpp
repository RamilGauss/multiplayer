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

#include <algorithm>
#include <boost/thread/thread.hpp>

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

TNetMakerEventWin::TNetMakerEventWin()
{
  flgActive   = false;
  flgNeedStop = false;
  // Init
  mInternalEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
  BL_ASSERT(mInternalEvent);

  AddThread();
}
//-----------------------------------------------------------------
TNetMakerEventWin::~TNetMakerEventWin()
{
  Done();
}
//-----------------------------------------------------------------
void TNetMakerEventWin::Add(int sock, INetControl* pControl, list<eTypeEvent>& lEvent)
{
  // добавить на обработку
  TEvent* pEvent   = new TEvent(TEvent::eAdd);
  pEvent->socket   = sock;
  pEvent->pControl = pControl; 
  pEvent->lEvent   = lEvent;
  mListEvent.Add(pEvent);
  // будим главный поток
  WakeUpMainThread();
}
//-----------------------------------------------------------------
void TNetMakerEventWin::Remove(INetMakerEventThread* pThread, int sock)
{
  // добавить на обработку
  TEvent* pEvent   = new TEvent(TEvent::eRemove);
  pEvent->socket   = sock;
  pEvent->pThread  = pThread;
  mListEvent.Add(pEvent);
  // будим главный поток
  WakeUpMainThread();
}
//-----------------------------------------------------------------
void* ThreadMakerEventWSA(void*p)
{
  ((TNetMakerEventWin*)p)->Engine();
  return NULL;
}
//----------------------------------------------------------------------------------
void TNetMakerEventWin::Engine()
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
void TNetMakerEventWin::Start()
{
  boost::thread work_thread(ThreadMakerEventWSA, this);

  while(IsActive()==false)
    ht_msleep(eWaitFeedBack);
}
//----------------------------------------------------------------------------------
void TNetMakerEventWin::Stop()
{
  flgNeedStop = true;
  while(IsActive())
    ht_msleep(eWaitFeedBack);
}
//----------------------------------------------------------------------------------
void TNetMakerEventWin::Work()
{
  WaitEventFromOtherThread();
  HandleInternalEvent();
}
//----------------------------------------------------------------------------------
void TNetMakerEventWin::Done()
{
  TListPtrIt bit = mListThread.begin();
  TListPtrIt eit = mListThread.end();
  while(bit!=eit)
  {
    TListPtrIt nit = bit;
    nit++;
    delete (*bit);
    bit = nit;
  }
  //------------------------------------------------------
  BOOL resClose = CloseHandle(mInternalEvent);
  BL_ASSERT(resClose);
}
//----------------------------------------------------------------------------------
void TNetMakerEventWin::HandleInternalEvent()
{
  // проверить наличие событий
  TEvent** ppEvent = mListEvent.GetFirst();
  while(ppEvent)
  {
    TEvent* pEvent = *ppEvent;
    switch(pEvent->type)
    {
      case TEvent::eAdd:
        AddWithoutDelay(pEvent->socket, pEvent->pControl, pEvent->lEvent);
        break;
      case TEvent::eRemove:
        RemoveWithoutDelay(pEvent->pThread,pEvent->socket);
        RemoveSockIP(pEvent->socket);
        break;
    }
    mListEvent.Remove(ppEvent);
    ppEvent = mListEvent.GetFirst();
  }
}
//----------------------------------------------------------------------------------
void TNetMakerEventWin::WaitEventFromOtherThread()
{
  // ждать пока в другом потоке не произойдет событие
  DWORD dwEvent = WaitForMultipleObjects(1,&mInternalEvent,FALSE,eTimeRefreshEngine);
  switch(dwEvent)
  { 
    case WSA_WAIT_FAILED: 
      PRINTF("WaitEventFromOtherThread() WaitForMultipleObjects FAIL %u.\n", GetLastError());
      break; 
    case WAIT_IO_COMPLETION: 
    case WSA_WAIT_TIMEOUT: 
      break; 
    default:;
  }
}
//----------------------------------------------------------------------------------
INetMakerEventThread* TNetMakerEventWin::AddThread()
{
  INetMakerEventThread* pNewThread = new TNetMakerEventThreadWSA(this, eMaxCountSocket);
  pNewThread->Start();
  mListThread.push_back(pNewThread);

  PRINTF("Add Thread c=%d.\n",mListThread.size());
  return pNewThread;
}
//----------------------------------------------------------------------------------
void TNetMakerEventWin::HandleFromWorkThread(INetMakerEventThread* pThread,
                                             INetControl* pControl, int sock, 
                                             list<eTypeEvent>& lEvent)
{
  pControl->Work(pThread, sock, lEvent);
}
//----------------------------------------------------------------------------------
INetMakerEventThread* TNetMakerEventWin::FindThreadWithLowerCount()
{
  INetMakerEventThread* pThread_minCount = NULL;
  int minCount = eMaxCountSocket;

  TListPtrIt bit = mListThread.begin();
  TListPtrIt eit = mListThread.end();
  while(bit!=eit)
  {
    if( (*bit)->GetCount() < minCount )
    {
      minCount = (*bit)->GetCount();
      pThread_minCount = *bit;
      if(minCount==0)
        break;
    }
    bit++;
  }
  // если не нашли, то массивы потоков переполнены - создать новый
  if(pThread_minCount==NULL)
    return AddThread();

  return pThread_minCount;
}
//----------------------------------------------------------------------------------
void TNetMakerEventWin::Sleep() 
{
  lockSleep();
}
//----------------------------------------------------------------------------------
void TNetMakerEventWin::WakeUp()
{
  unlockSleep();
}
//----------------------------------------------------------------------------------
void TNetMakerEventWin::AddWithoutDelay( int sock, INetControl* pControl, 
                                        std::list<eTypeEvent>& lEvent)
{
  INetMakerEventThread* pThread = FindThreadWithLowerCount();
  pThread->Sleep();
    pThread->Add(sock, pControl, lEvent);
  pThread->WakeUp();
}
//----------------------------------------------------------------------------------
void TNetMakerEventWin::RemoveWithoutDelay( INetMakerEventThread* pThread, int socket)
{
  pThread->Sleep();
    pThread->Remove(socket);
  pThread->WakeUp();
}
//----------------------------------------------------------------------------------
void TNetMakerEventWin::WakeUpMainThread()
{
  BOOL resSet = SetEvent(mInternalEvent);
  BL_ASSERT(resSet);
}
//----------------------------------------------------------------------------------
