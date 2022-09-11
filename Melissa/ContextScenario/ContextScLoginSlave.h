/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_CONTEXT_SC_LOGIN_SLAVE_H
#define MELISSA_CONTEXT_SC_LOGIN_SLAVE_H

#include "IContextScenario.h"
#include "BreakPacket.h"
#include "ShareMisc.h"

namespace nsMelissa
{
  // ��������, ������������ � ���������� ������� �����������
  class TContextScLoginSlave : public IContextScenario
	{
    unsigned int mTimeWaitAnswerFromMaster;
    bool flgConnectToMaster;
	public:
		TContextScLoginSlave();
    virtual ~TContextScLoginSlave();

    void SetConnect(bool v);
    bool IsConnect();

    unsigned int GetTimeWait();
    void SetTimeWait(unsigned int v);
  private:
	};
}  
#endif