/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MANAGER_SESSION_H
#define MANAGER_SESSION_H

#include <map>

#include "ShareMisc.h"
#include "Session.h"
#include "GCS.h"
#include "NavigateSession.h"
#include "CallBackRegistrator.h"
#include "DescRecvSession.h"
#include "ManagerTransport.h"
#include "ManagerContextCrypto.h"

namespace nsMMOEngine
{
  class TManagerSession
  {
    enum
    {
      eDefTimeLive               = 5*1000,// ��
      eSleepForWaitUp            = 40,  // ��
      eLimitWaitTimeAnswerFromUp = 2000,// ��
    };

    unsigned int mTimeLiveSession;
    unsigned int mLastID_Session;
   
    // ������ ���� ��� ���������, ��� �� �������������� ������� ����������� ��������
		TNavigateSession  *mNavigateSession;
    TManagerTransport *mMngTransport;
    bool flgStart;

    bool flgUseCryptTCP;
    TManagerContextCrypto mMngCtxCrypto;

    // ���� �������� - ���� ������� ����������
    bool flgNeedAnswerFromUp;
    bool flgGetAnswerFromUp;
    TIP_Port mIP_PortUp;

    TCallBackRegistrator1<int>               mCallBackDiconnect;
    TCallBackRegistrator1<TDescRecvSession*> mCallBackRecv;

    GCS mMutexAccessMapSession;
    void lockAccessSession(){mMutexAccessMapSession.lock();}
    void unlockAccessSession(){mMutexAccessMapSession.unlock();}

    // ������ �� ������� ����������� ������ ����� ��� ���� ���
    GCS mMutexConnectUp;
    void lockConnectUp(){mMutexConnectUp.lock();}
    void unlockConnectUp(){mMutexConnectUp.unlock();}
  public:
    TManagerSession();
    ~TManagerSession();
		// ��� ����������� ������
    void SetMakerTransport(IMakerTransport* pMakerTransport);

    TCallBackRegistrator1<TDescRecvSession*>* GetCallbackRecv(){return &mCallBackRecv;}
    TCallBackRegistrator1<int>* GetCallbackDisconnect(){return &mCallBackDiconnect;}
    
		bool Start(TDescOpen* pDesc, int count = 1);
    void Work();
		// ��� ������ � �����
    unsigned int Send(unsigned int ip, unsigned short port, TBreakPacket bp, unsigned char subNet, bool check = true);// ������ ���� �� ����������� ����������
		void Send(unsigned int id_session, TBreakPacket bp, bool check = true);
    unsigned int GetSessionID(unsigned int ip, unsigned short port);
    bool GetInfo(unsigned int id_session, TIP_Port& ip_port_out);
    void CloseSession(unsigned int id_session);
    bool IsExist(unsigned int id_session);
    // ���������
    void SetTimeLiveSession(unsigned int time_ms);

  protected:
    friend class TReciverTransport;

		bool StartTransport(unsigned short port, unsigned char subNet);
    void Recv( INetTransport::TDescRecv* pDescRecv, INetTransport* pTransport);
    void Disconnect(TIP_Port* ip_port);

    TSession* NewSession(TIP_Port& ip_port, INetTransport* pTransport);

    void RecvPacket(TDescRecvSession& descRecvSession, TSession* pSession);
    void RecvKeyRSA(TDescRecvSession& descRecvSession, TSession* pSession);
    void RecvKeyAES(TDescRecvSession& descRecvSession);

    bool WaitAnswerFromUp();
    void SetupFlagsForWaitUp();
    void CleanFlagsForWaitUp();
    void SendKeyRSA_Up(TSession* pSession);

    void Send(TSession* pSession, TBreakPacket bp, bool check);

    void FixHack(char* sMsg);

    // ������������ ���������� ��� �������� TCP
    void SetUseCryptTCP(bool v);
    bool GetUseCryptTCP();
  };
}

#endif