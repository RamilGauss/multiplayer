/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_CONTEXT_SC_DISCONECT_CLIENT_H
#define MELISSA_CONTEXT_SC_DISCONECT_CLIENT_H

#include "IContextScenario.h"
#include "BreakPacket.h"
#include "ShareMisc.h"

namespace nsMelissa
{
  class TContextScDisconnectClient : public IContextScenario
	{
	public:
		TContextScDisconnectClient();
    virtual ~TContextScDisconnectClient();
  private:
	};
}  
#endif