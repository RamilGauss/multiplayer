/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MASTER_H
#define MASTER_H

#include "ActiveServer.h"

#include <list>
#include <map>
#include <set>

namespace nsMMOEngine
{
  class IScenario;
  class TClient_master;
  class TSlave_master;
  class TGroupClient_master;
  class MMO_ENGINE_EI TMaster : public TActiveServer
  {
    // для анализа, при создании группы
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
      TSlave_master* pSlave;
    };
    typedef std::list<TDescChoiseSlave> TListSlavePrior;

    enum{
      // по степени нагрузки CPU Slave
      eLimitMoreEmpty = 10,// %
      eLimitMoreHalf  = 50,// %

      eLimitLoadProcentOnSlaveForAdd = 90,
      eLimitCountClientWaitFreeSpace = 2000,// максимальный размер очереди ожидающих
    };

    // DOWN
    // Slave
    typedef std::map<unsigned int,TSlave_master*> TMapUintSlavePtr;
    typedef TMapUintSlavePtr::iterator TMapUintSlavePtrIt;
    TMapUintSlavePtr mMapID_SessionSlave;

    // GroupClient
    typedef std::map<unsigned int,TGroupClient_master*> TMapUintGroupClientPtr;
    typedef TMapUintGroupClientPtr::iterator TMapUintGroupClientPtrIt;
    TMapUintGroupClientPtr mMapIDGroupClient;
    unsigned int mCurCounterGroup;
    
    // Client
    typedef std::map<unsigned int,TClient_master*> TMapUintClientPtr;
    typedef TMapUintClientPtr::iterator TMapUintClientPtrIt;
    TMapUintClientPtr mMapKeyClient;
    
    typedef std::map<unsigned int,unsigned int> TMapUintUint;
    typedef TMapUintUint::iterator TMapUintUintIt;
    // когда клиент в процессе соединения, то найти его по id_session можно так
    // находим ключ, по нему ищем клиента
    TMapUintUint mIDSessionKey;
    // когда клиент пытается авторизоваться, то идентифицировать его можно по сессии
    // ждет вызова SetResultLogin, тогда ему присвоят ключ
    TMapUintClientPtr mMapID_SessionClientWait;
  public:
    typedef enum
    {
      eAccept,
      eReject,
    }tResultLogin;

    TMaster();
    virtual ~TMaster();
    
    virtual bool TryCreateGroup(std::list<unsigned int>& l_id_client, unsigned int& id_group); // -
    virtual void DestroyGroup(unsigned int id_group);// +
    virtual void LeaveGroup(unsigned int id_client);// +
    virtual void GetListForGroup(unsigned int id_group, std::list<unsigned int>& l);// +
    virtual void SetResultLogin(bool res, unsigned int id_session, 
                                unsigned int id_client, // ключ, из БД например
                                void* resForClient, int sizeResClient);// +
    virtual bool FindSlaveSessionByGroup(unsigned int id_group, unsigned int& id_session);// +
   
		// BaseServer
    virtual void SendByClientKey(std::list<unsigned int>& lKey, TBreakPacket bp);
		struct TDescDownMaster
		{
			unsigned int id_session;
		};
		virtual int  GetCountDown();
		virtual bool GetDescDown(int index, void* pDesc, int& sizeDesc);// pDesc имеет тип TDescDownMaster*

    virtual void SendDown(unsigned int id_session, TBreakPacket bp, bool check = true);

    // ActiveServer      
    virtual void ConnectUp(unsigned int ip, unsigned short port);
	protected:
    // Base
    virtual void DisconnectInherit(unsigned int id_session);
		virtual void WorkInherit();

	protected:// like slots
    virtual void NeedContextLoginClient(unsigned int id_session);
    virtual void NeedContextLoginClientByClientKey(unsigned int id_key_client);
    virtual void NeedLeaveFromQueue(unsigned int id_session);
    virtual void NeedNumInQueue(unsigned int id_session);
    //--------------------------------------------------------------------------
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
    //unsigned int    FindIDClientByIDSession(unsigned int id);
    //TClient_master* FindClientByIDSession( unsigned int id);
    //TClient_master* FindClientByIDClient( unsigned int id);


    TClient_master* FindClientByIDSession(unsigned int id);
    TClient_master* FindClientByIDSessionWait(unsigned int id);
    TClient_master* FindClientByKey(unsigned int key);
    TClient_master* FindClientByID(TMapUintClientPtr& m, unsigned int id);

    TSlave_master* FindSlaveByIDSession(unsigned int id);
    
    TGroupClient_master* FindGroupByKey(unsigned int key);
  private:
    bool EvalCreateGroupNow(std::list<unsigned int>& l_id_client, 
                            unsigned int& id_group);
    TSlave_master* AnalizListSlaveForGroup(TListSlavePrior& l_slave_prior);
    unsigned int CreateGroup(TSlave_master* pSlave,std::list<unsigned int>& l_id_client);

    void DisconnectSuperServer(); 
    void DisconnectClientWait(TClient_master* pClient);
    void DisconnectClient(TClient_master* pClient);
    void DisconnectSlave(TSlave_master* pSlave);

    void Done();
  };
}

#endif