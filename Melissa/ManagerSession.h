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

#ifndef MELISSA_MANAGER_SESSION_H
#define MELISSA_MANAGER_SESSION_H

#include <map>

#include "ShareMisc.h"
#include "Session.h"
#include "IMakerTransport.h"
#include "GCS.h"
#include "NavigateSession.h"
#include "CallBackRegistrator.h"
#include "DescRecvSession.h"

namespace nsMelissa
{
  class TManagerSession
  {
    enum{
      eDefTimeLive = 30*1000,// мс
    };

    unsigned int mTimeLiveSession;

		TNavigateSession mNavigateSession;

    unsigned int mLastID_Session;
   
    IMakerTransport* mMakerTransport;
    INetTransport*   mTransport;

    TCallBackRegistrator1<int>         mCallBackDiconnect;
    TCallBackRegistrator1<TDescRecvSession*> mCallBackRecv;

    GCS mMutexAccessMapSession;
    void lockAccessSession(){mMutexAccessMapSession.lock();}
    void unlockAccessSession(){mMutexAccessMapSession.unlock();}
  public:
    TManagerSession();
    ~TManagerSession();
		// для поддержания работы
    void SetMakerTransport(IMakerTransport* pMakerTransport);

    TCallBackRegistrator1<TDescRecvSession*>* GetCallbackRecv(){return &mCallBackRecv;}
    TCallBackRegistrator1<int >* GetCallbackDisconnect(){return &mCallBackDiconnect;}
    
    bool Start(unsigned short port, unsigned char subNet = 0);
    void Work();
		// для работы с сетью
    unsigned int Send(unsigned int ip, unsigned short port, TBreakPacket bp, bool check = true);// только если не установлено соединение
		void Send(unsigned int ID_Session, TBreakPacket bp, bool check = true);
    unsigned int GetSessionID(unsigned int ip, unsigned short port);
    bool GetInfo(unsigned int ID_Session, TIP_Port& ip_port_out);
    void CloseSession(unsigned int ID_Session);
    bool IsExist(unsigned int ID_Session);
    // настройка
    void SetTimeLiveSession(unsigned int time_ms);

  protected:
    friend void FuncRecvFromTransport( void* p, int size);
    friend void FuncDisconnectFromTransport( void* p, int size);
    
    void Stop();

    void Recv( INetTransport::TDescRecv* pDescRecv );
    void Disconnect(TIP_Port* ip_port);

    TSession* NewSession(TIP_Port& ip_port);
  };
}

#endif