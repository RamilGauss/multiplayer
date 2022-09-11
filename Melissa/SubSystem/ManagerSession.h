/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
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