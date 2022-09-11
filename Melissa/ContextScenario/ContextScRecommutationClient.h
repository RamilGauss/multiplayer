/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_CONTEXT_SC_RECOMMUTATION_CLIENT_H
#define MELISSA_CONTEXT_SC_RECOMMUTATION_CLIENT_H

#include "IContextScenario.h"
#include "BreakPacket.h"

namespace nsMelissa
{
  class TClientForMasterPrivate;
  class TContextScRecommutationClient : public IContextScenario
	{
    TClientForMasterPrivate* mClient;
	public:
		TContextScRecommutationClient();
    virtual ~TContextScRecommutationClient();

    TClientForMasterPrivate* GetClient();
    void SetClient(TClientForMasterPrivate* p);
  private:
	};
}  
#endif