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


#ifndef NetMakerEventWSAH
#define NetMakerEventWSAH

#include <list>

#include "glib/gthread.h"

#include "INetMakerEvent.h"
#include "ContainerRise.h"

class INetControl;

class TNetMakerEventWSA : public INetMakerEvent
{

  volatile bool flgActive;
  volatile bool flgNeedStop;

  GThread* thread;

  enum
  {
    eTimeRefreshEngine = 60,  // частота обновления движка, мс
    eWaitFeedBack    = 20,// ждать пока активизируется двигатель, мс
  };

  TContainerRise mArrEvent;

public:
	TNetMakerEventWSA();
	virtual ~TNetMakerEventWSA();

	virtual void Start();
	virtual void Stop();

  virtual bool IsActive(){return flgActive;}

  virtual void Add(int sock, INetControl* pControl);
  virtual void Remove(int sock);

  virtual void SetTypeEvent( int sock, std::list<INetControl::eTypeEvent>& lEvent);

protected:

  friend void* ThreadMakerEventWSA(void*p);
  void Engine();

  void Work();
  void WSA_Event2Control( int event, std::list<INetControl::eTypeEvent>& lEvent);
  void Control2WSA_Event( std::list<INetControl::eTypeEvent>& lEvent, int& event );
};


#endif