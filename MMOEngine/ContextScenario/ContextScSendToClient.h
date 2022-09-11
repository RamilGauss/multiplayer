/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef CONTEXT_SC_SEND_TO_CLIENT_H
#define CONTEXT_SC_SEND_TO_CLIENT_H

#include "IContextScenario.h"
#include "BreakPacket.h"

namespace nsMMOEngine
{
  // сценарии, содержащиеся в контейнере взаимно блокируются
  class TContextScSendToClient : public IContextScenario
	{
	public:
		TContextScSendToClient();
    virtual ~TContextScSendToClient();
  private:
	};
}  
#endif