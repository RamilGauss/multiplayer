/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef CLIENT_MASTER_H
#define CLIENT_MASTER_H

#include "ContainerContextSc.h"

namespace nsMMOEngine
{
  class TSlave_master;
	class TClient_master
	{
    TSlave_master* mSlave;
    unsigned int mGroupID;
    unsigned int mKey;

    TContainerContextSc mC;
  public:	
    typedef enum{
      eLogin,// подключающиеся 
      eReady,  // подключенные, готов к перекоммутации
      eRcm,    // коммутация
    }tState;

		TClient_master();

    TContainerContextSc* GetC();

    void SetSlave(TSlave_master* pSlave);
    TSlave_master* GetSlave();

    void SetGroupID(unsigned int id);
    unsigned int GetGroupID();

    void SetKey(unsigned int k);
    unsigned int GetKey();

    void SetState(tState state);
    tState GetState();
  protected:
    tState mState;
	};
}  
#endif