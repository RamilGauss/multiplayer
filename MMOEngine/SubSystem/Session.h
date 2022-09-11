/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef SESSION_H
#define SESSION_H

#include "BreakPacket.h"
#include "INetTransport.h"

namespace nsMMOEngine
{
  class TSession
  {
    unsigned int mTimeLive;// мс
    unsigned int mID;
    TIP_Port mIP_Port;
    unsigned int mLastTimeActive;
    INetTransport* mTransport;
  protected:
    friend class TManagerSession;

    enum{
      eEcho   = 'e',
      ePacket = 'p',
    };
#if defined( WIN32 )
#pragma pack(push, 1)
#endif
    struct THeader
    {
      char type;
      THeader(char t = ePacket)
      {
        type = t;
      }
    };
#if defined( WIN32 )
#pragma pack(pop)
#endif

  public:
    TSession(unsigned int time_live_ms);
    ~TSession();
    
    void Work();
    void Send(TBreakPacket bp, bool check = true);
    void SetTransport(INetTransport* pTransport);
    void GetInfo(TIP_Port& pDesc);
    void SetInfo(TIP_Port& pDesc);
    void Recv();
    unsigned int GetID(){return mID;}
    void SetID(unsigned int id){mID=id;}
    void Close();
  protected:
    void SendEcho();
    void RefreshLastTime();
    void SendData(char type, TBreakPacket& bp, bool check = true);
  };
}

#endif