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


#ifndef NetMakerEventThreadWSAH
#define NetMakerEventThreadWSAH

#include "GCS.h"
#include "INetMakerEventThread.h"
#include "MapDual.h"

class TNetMakerEventWin;

class TNetMakerEventThreadWSA : public INetMakerEventThread
{
  // ��� ����� ���� ����� ������ ���������� ���� ������������ list.size()
  volatile int mCntSocket;

  volatile bool flgActive;
  volatile bool flgNeedStop;

  //GThread* thread;

  enum
  {
    eTimeRefreshEngine    = 100, // ������� ���������� ������, ��
    eWaitFeedBack         = 50, // ����� ���� �������������� ���������, ��
  };

  typedef std::map<int,int> TMapIntInt;
  typedef TMapIntInt::iterator TMapII_It;

  TMapIntInt mMapSockEvent;

  struct TDescSocket
  {
    int sock;
    INetControl* pControl;
  };

  typedef std::vector<int>         TVecInt;
  typedef std::vector<TDescSocket> TVecDS;

  TVecInt mVecEvent;
  TVecDS  mVecDSock;

  TNetMakerEventWin* mMaster;

public:
	TNetMakerEventThreadWSA(TNetMakerEventWin* pMaster, int maxCountSocket);
	virtual ~TNetMakerEventThreadWSA();

	virtual void Start(); // blocking
	virtual void Stop();  // blocking
  virtual void Sleep(); // blocking
  virtual void WakeUp();// blocking

  virtual bool IsActive(){return flgActive;}

  virtual int GetCount();
  virtual void Add(int sock, INetControl* pControl, 
                   std::list<nsNetTypeEvent::eTypeEvent>& lEvent);
  virtual void Remove(int sock);

protected:
  void Done();

  void SetTypeEvent( int sock, std::list<nsNetTypeEvent::eTypeEvent>& lEvent);

  friend void* NetMakerEventThread_Thread(void*p);
  void Engine();

  void Work();
  void WaitSocketEvent();

  // ��� ���������� � ��������
  // � Engine ���������� lock - ����� ������������ �� �������� �������
  // ����� unlock - � � ������������ ������ ���������� �������������
  // ���� ���������� ��� �������� �������
  GCS mMutexSleep;
  void lockSleep()  {mMutexSleep.lock();}
  void unlockSleep(){mMutexSleep.unlock();}

  void WSA_Event2Control( int event, std::list<nsNetTypeEvent::eTypeEvent>& lEvent);
  void Control2WSA_Event( std::list<nsNetTypeEvent::eTypeEvent>& lEvent, int& event );
};


#endif