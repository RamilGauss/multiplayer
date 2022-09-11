/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_CLIENT_FOR_MASTER_PRIVATE_H
#define MELISSA_CLIENT_FOR_MASTER_PRIVATE_H

#include "ContainerContextSc.h"

namespace nsMelissa
{
  class TSlavePrivate;
	class TClientForMasterPrivate
	{
    TSlavePrivate* mSlave;
    unsigned int mGroupID;
    unsigned int mKey;

    TContainerContextSc mC;
  public:	
    typedef enum{
      eLogin,// подключающиеся 
      eReady,  // подключенные, готов к перекоммутации
      eRcm,    // коммутация
    }tState;

		TClientForMasterPrivate();

    TContainerContextSc* GetC();

    void SetSlave(TSlavePrivate* pSlave);
    TSlavePrivate* GetSlave();

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