/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_CONTEXT_SC_SEND_TO_CLIENT_H
#define MELISSA_CONTEXT_SC_SEND_TO_CLIENT_H

#include "IContextScenario.h"
#include "BreakPacket.h"

namespace nsMelissa
{
  // ��������, ������������ � ���������� ������� �����������
  class TContextScSendToClient : public IContextScenario
	{
	public:
		TContextScSendToClient();
    virtual ~TContextScSendToClient();
  private:
	};
}  
#endif