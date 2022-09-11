/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MASTER_SS_H
#define MASTER_SS_H

#include <set>
#include <map>

#include "ContainerContextSc.h"

namespace nsMMOEngine
{
  class TClient_ss;
	class TMaster_ss
	{
    unsigned int mID_SessionMasterSS;

    typedef std::map<unsigned int,TClient_ss*> TMapUintPtr;
    typedef TMapUintPtr::iterator TMapUintPtrIt;

    TMapUintPtr mMapKeyClient;

    TContainerContextSc mC;
  public:	

		TMaster_ss();
		virtual ~TMaster_ss();

    TContainerContextSc* GetC();

    TClient_ss* FindClient(unsigned int id_client);
    void AddClient(TClient_ss* pClient);
    void DeleteClient(unsigned int id_client);

    int GetCountClient();

    unsigned int GetID_Session();
    void SetID_Session(unsigned int id);
	};
}  
#endif