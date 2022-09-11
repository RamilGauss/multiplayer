/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MASTER_H
#define MASTER_H

#include "ActiveServer.h"

class TSetOrderElement;
namespace nsMMOEngine
{
  class IScenario;
  class TManagerContextMoreDownClientConnection;
  class TManagerContextDownConnection_Slave;
  class TManagerContextClientLogining;
  class TManagerGroupClient;
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
      //TSlave_master* pSlave;
    };
    typedef std::list<TDescChoiseSlave> TListSlavePrior;

    enum{
      // по степени нагрузки CPU Slave
      eLimitMoreEmpty = 10,// %
      eLimitMoreHalf  = 50,// %

      eLimitLoadProcentOnSlaveForAdd = 75,
      eLimitLoadProcentOnSlaveForAdd_ClientInGroup = 70, // для Клиента, состоящего в Группе процент другой

      eLimitCountClientWaitFreeSpace = 2000,// максимальный размер очереди ожидающих
    };

    // DOWN
    //клиенты, которые уже в системе
    boost::scoped_ptr<TManagerContextMoreDownClientConnection> mMngContextClient;
    // Slaves
    boost::scoped_ptr<TManagerContextDownConnection_Slave>     mMngContextSlave;
    // клиенты, которые находятся в процессе авторизации
    boost::scoped_ptr<TManagerContextClientLogining>           mMngContextClientLogining;
    // группы клиентов
    boost::scoped_ptr<TManagerGroupClient>                     mMngGroup;
    // ID клиентов, которые ожидают в очереди, по причине загруженности Slave
    boost::scoped_ptr<TSetOrderElement>                        mSetClientKeyInQueue;
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
    virtual void NeedContextDisconnectClient(unsigned int id_client);
    virtual void NeedContextLoginClientBySession(unsigned int id_session);
    virtual void NeedContextLoginClientByClientKey(unsigned int id_key_client);
    virtual void NeedNumInQueueLoginClient(unsigned int id_session);
    //--------------------------------------------------------------------------
    virtual void NeedContextLoginSlave(unsigned int id_session);
    virtual void NeedContextRcm(unsigned int id_session);
    virtual void NeedContextSynchroSlave(unsigned int id_session);
	protected:
    virtual void EndDisconnectClient(IScenario*);
    virtual void EndLoginMaster(IScenario* pSc);
    virtual void EndLoginSlave(IScenario* pSc);
    virtual void EndLoginClient(IScenario* pSc);
    virtual void EndRcm(IScenario* pSc);
    virtual void EndSynchroSlave(IScenario* pSc);
  private:
    bool EvalCreateGroupNow(std::list<unsigned int>& l_id_client, 
                            unsigned int& id_group);

		//###
		//TSlave_master* AnalizListSlaveForGroup(TListSlavePrior& l_slave_prior);
  //  unsigned int CreateGroup(TSlave_master* pSlave,std::list<unsigned int>& l_id_client);
	//###
    bool DisconnectSuperServer(unsigned int id_session); 
    bool DisconnectClientWait(unsigned int id_session);
    bool DisconnectSlave(unsigned int id_session);

		// при освобождении места на Slave попытаться добавить Клиента, который ждет в очереди
		void TryAddClientFromQueue();
    void Done();
  };
}

#endif