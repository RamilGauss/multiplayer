/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef CONTEXT_SC_RECOMMUTATION_CLIENT_H
#define CONTEXT_SC_RECOMMUTATION_CLIENT_H

#include "IContextScenario.h"
#include "BreakPacket.h"

namespace nsMMOEngine
{
  class TClient_master;
  class TContextScRecommutationClient : public IContextScenario
	{
    TClient_master* mClient;
	public:
		TContextScRecommutationClient();
    virtual ~TContextScRecommutationClient();

    TClient_master* GetClient();
    void SetClient(TClient_master* p);
  private:
	};
}  
#endif