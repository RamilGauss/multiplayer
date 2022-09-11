/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_SCENARIO_LOGIN_CLIENT_H
#define MELISSA_SCENARIO_LOGIN_CLIENT_H

#include "IScenario.h"
#include "ContextScLoginClient.h"
#include "MakerScenario.h"
#include "ShareMisc.h"
#include "CallBackRegistrator.h"

namespace nsMelissa
{
  class TScenarioLoginClient : public IScenario
  {
    enum{eToMaster,
         eAcceptFromMaster,
         eRejectFromMaster,

    };
    struct THeaderLoginClient : public IScenario::TBaseHeader
    {
      THeaderLoginClient(){type=TMakerScenario::eLoginClient;}
    };
    //-------------------------------------------------
    struct THeaderToMaster : public THeaderLoginClient
    {
      THeaderToMaster(){subType=eToMaster;}
    };
    //-------------------------------------------------
    struct THeaderResultFromMaster : public THeaderLoginClient
    {
      THeaderResultFromMaster(){lenRes=0;}
      unsigned char lenRes;
    };
    //-------------------------------------------------
    struct THeaderAcceptFromMaster : public THeaderResultFromMaster
    {
      THeaderAcceptFromMaster(){subType=eAcceptFromMaster;}
      unsigned int key;
    };
    //-------------------------------------------------
    struct THeaderRejectFromMaster : public THeaderResultFromMaster
    {
      THeaderRejectFromMaster(){subType=eRejectFromMaster;}
    };
    //-------------------------------------------------
    enum{eTimeWait=20000,// мс
    };

    TCallBackRegistrator1<unsigned int> mCallBackChangeSession;
  public:
    TScenarioLoginClient();
    virtual ~TScenarioLoginClient();
    
    virtual void Recv(TDescRecvSession* pDesc);

    template <typename F, class C>
    void RegisterOnChangeSession(F f, C pObject)// когда сценарий будет общаться со Slave, а не с Client
    {mCallBackChangeSession.Register(f,pObject);}

    void Start();
    // начало авторизации Клиентом
    void Connect(unsigned int ip, unsigned short port, 
                 void* data, int size);
    // решение Мастера
    void Reject(void* resForClient, int sizeResClient);
    void Accept(unsigned int key, void* resForClient, int sizeResClient, 
                TIP_Port& ip_port_Slave);
    //void Queue();
  protected:
    virtual void Work();
  protected:
    TContextScLoginClient* Context(){return (TContextScLoginClient*)mCurContext;}
  };
}
#endif