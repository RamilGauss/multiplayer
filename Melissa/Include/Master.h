/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_MASTER_H
#define MELISSA_MASTER_H

#include "ActiveServer.h"

#include <list>
#include <map>
#include <set>

namespace nsMelissa
{
  class IScenario;
  class TClientForMasterPrivate;
  class TSlavePrivate;
  class TGroupClientPrivate;
  class MELISSA_EI TMaster : public TActiveServer
  {
    // ��� �������, ��� �������� ������
    struct TDescChoiseSlave
    {
      typedef enum{
        eEmpty,
        eGroup,
        eMGroup,
        eFree,
        eGroupFree,
        eMFree,
      } eType;
      eType type;
      TSlavePrivate* pSlave;
    };
    typedef std::list<TDescChoiseSlave> TListSlavePrior;

    enum{
      // �� ������� �������� CPU Slave
      eLimitMoreEmpty = 10,// %
      eLimitMoreHalf  = 50,// %

      eLimitLoadProcentOnSlaveForAdd = 90,
      eLimitCountClientWaitFreeSpace = 2000,// ������������ ������ ������� ���������
    };

    // DOWN
    // Slave
    typedef std::map<unsigned int,TSlavePrivate*> TMapUintSlavePtr;
    typedef TMapUintSlavePtr::iterator TMapUintSlavePtrIt;
    TMapUintSlavePtr mMapID_SessionSlave;

    // GroupClient
    typedef std::map<unsigned int,TGroupClientPrivate*> TMapUintGroupClientPtr;
    typedef TMapUintGroupClientPtr::iterator TMapUintGroupClientPtrIt;
    TMapUintGroupClientPtr mMapIDGroupClient;
    unsigned int mCurCounterGroup;
    
    // Client
    typedef std::map<unsigned int,TClientForMasterPrivate*> TMapUintClientPtr;
    typedef TMapUintClientPtr::iterator TMapUintClientPtrIt;
    TMapUintClientPtr mMapKeyClient;
    
    typedef std::map<unsigned int,unsigned int> TMapUintUint;
    typedef TMapUintUint::iterator TMapUintUintIt;
    // ����� ������ � �������� ����������, �� ����� ��� �� id_session ����� ���
    // ������� ����, �� ���� ���� �������
    TMapUintUint mIDSessionKey;
    // ����� ������ �������� ��������������, �� ���������������� ��� ����� �� ������
    // ���� ������ SetResultLogin, ����� ��� �������� ����
    TMapUintClientPtr mMapID_SessionClientWait;
  public:
    typedef enum
    {
      eAccept,
      eReject,
      eRejectOverLimit,
      eQueue,
    }tResultLogin;

    TMaster();
    virtual ~TMaster();
    
    virtual bool TryCreateGroup(std::list<unsigned int>& l_id_client, unsigned int& id_group); // -
    virtual void DestroyGroup(unsigned int id_group);// +
    virtual void LeaveGroup(unsigned int id_client);// +
    virtual void GetListForGroup(unsigned int id_group, std::list<unsigned int>& l);// +
    virtual void SetResultLogin(bool res, unsigned int id_session, 
                                unsigned int id_client, // ����, �� �� ��������
                                void* resForClient, int sizeResClient);// +
    virtual unsigned int GetSlaveSessionByGroup(unsigned int id_group);// -
   
		// BaseServer
    virtual void SendByClientKey(std::list<unsigned int>& l, TBreakPacket bp);
		struct TDescDownMaster
		{
			unsigned int id_session;
		};
		virtual int  GetCountDown();
		virtual bool GetDescDown(int index, void* pDesc, int& sizeDesc);// pDesc ����� ��� TDescDownMaster*

    virtual void SendDown(unsigned int id_session, TBreakPacket bp, bool check = true);

    // ActiveServer      
    virtual void ConnectUp(unsigned int ip, unsigned short port);
	protected:
    // Base
    virtual void DisconnectInherit(unsigned int id_session);
		virtual void WorkInherit();

	protected:// like slots
    virtual void NeedContextLoginClient(unsigned int id_session);
    virtual void NeedContextLoginSlave(unsigned int id_session);
    virtual void NeedContextRcm(unsigned int id_session);
    virtual void NeedContextSynchroSlave(unsigned int id_session);

    virtual void EndLoginMaster(IScenario* pSc);
    virtual void EndLoginSlave(IScenario* pSc);
    virtual void EndLoginClient(IScenario* pSc);
    virtual void EndRcm(IScenario* pSc);
    virtual void EndSynchroSlave(IScenario* pSc);

    void EndLoginClientBySlave(IScenario* pSc);
  private:
    TClientForMasterPrivate* GetClientByIDSession(unsigned int id);
    TClientForMasterPrivate* GetClientByIDSessionWait(unsigned int id);
    TClientForMasterPrivate* GetClientByKey(unsigned int key);

    TClientForMasterPrivate* GetClientByID(TMapUintClientPtr& m, unsigned int id);

    TSlavePrivate* GetSlaveByIDSession(unsigned int id);
    
    TGroupClientPrivate* GetGroupByKey(unsigned int key);

    bool EvalCreateGroupNow(std::list<unsigned int>& l_id_client, 
                            unsigned int& id_group);
    TSlavePrivate* AnalizListSlaveForGroup(TListSlavePrior& l_slave_prior);
    unsigned int CreateGroup(TSlavePrivate* pSlave,std::list<unsigned int>& l_id_client);

    void DisconnectSuperServer(); 
    void DisconnectClientWait(TClientForMasterPrivate* pClient);
    void DisconnectClient(TClientForMasterPrivate* pClient);
    void DisconnectSlave(TSlavePrivate* pSlave);

    void Done();
  };
}

#endif