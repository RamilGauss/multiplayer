/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_CONTEXT_SC_DISCONECT_SLAVE_H
#define MELISSA_CONTEXT_SC_DISCONECT_SLAVE_H

#include "IContextScenario.h"
#include "BreakPacket.h"
#include "ShareMisc.h"

namespace nsMelissa
{
  class TContextScDisconnectSlave : public IContextScenario
	{
	public:
		TContextScDisconnectSlave();
    virtual ~TContextScDisconnectSlave();
  private:
	};
}  
#endif