/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef GROUP_CLIENT_MASTER_H
#define GROUP_CLIENT_MASTER_H

#include <set>

namespace nsMMOEngine
{
  class TClient_master;
	class TGroupClient_master
	{
    typedef std::set<TClient_master*> TSetUintPtr;
    TSetUintPtr mSetClient;

    unsigned int mID;
  public:	
    typedef enum{
      eReady,// подключенные 
      eConnect,// подключающиеся 
    }tState;

		TGroupClient_master();

    void SetID(unsigned int id);
    unsigned int GetID();

    void SetClient(std::set<TClient_master*>& setClient);
    std::set<TClient_master*>& GetClient();

    void SetState(tState state);
    tState GetState();
  protected:
    tState mState;
	};
}  
#endif