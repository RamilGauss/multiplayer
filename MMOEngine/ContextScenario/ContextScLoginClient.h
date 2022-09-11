/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef CONTEXT_SC_LOGIN_CLIENT_H
#define CONTEXT_SC_LOGIN_CLIENT_H

#include "IContextScenario.h"

#include <map>

#include "BreakPacket.h"

namespace nsMMOEngine
{
  // ��������, ������������ � ���������� ������� �����������
  class TContextScLoginClient : public IContextScenario
	{
  public:
    typedef enum
    {
      eUndef,
      eReject,
      eAccept,
    }tStateResult;
  private:
    tStateResult mState;

    int mNumInQueue;

    unsigned int mTimeWaitAnswer;

    typedef std::map<std::string, unsigned int> TMapStrUint;
    typedef TMapStrUint::iterator TMapStrUintIt;
    TMapStrUint mMapStrID_Session;

    TContainer mAcceptData;

    unsigned int mIDClient;
	public:
		TContextScLoginClient();
    virtual ~TContextScLoginClient();

    unsigned int GetIDClient();
    void SetIDClient(unsigned int id);

    void SetTimeWait(unsigned int v);
    unsigned int GetTimeWait();

    void Accept();
    void Reject();
    
    bool IsReject();
    bool IsAccept();

    unsigned int GetID_SessionClientMaster();
    void SetID_SessionClientMaster(unsigned int id);
    unsigned int GetID_SessionClientSlave();
    void SetID_SessionClientSlave(unsigned int id);
    unsigned int GetID_SessionMasterSlave();
    void SetID_SessionMasterSlave(unsigned int id);
    unsigned int GetID_SessionMasterSS();
    void SetID_SessionMasterSS(unsigned int id);

    void SetNumInQueue(int val);
    int  GetNumInQueue();

    void SaveAcceptData(void* resForClient, int sizeResClient);
    void* GetSaveAcceptDataPtr();
    int   GetSaveAcceptDataSize();

  private:
    void SetID_Session(std::string& name, unsigned int id);
    unsigned int GetID_Session(std::string& name);
	};
}  
#endif