/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_CONTEXT_SC_LOGIN_MASTER_H
#define MELISSA_CONTEXT_SC_LOGIN_MASTER_H

#include "IContextScenario.h"
#include "BreakPacket.h"

namespace nsMelissa
{
  // ��������, ������������ � ���������� ������� �����������
  class TContextScLoginMaster : public IContextScenario
	{
    bool flgConnect;
    unsigned int mTimeWait;
	public:
		TContextScLoginMaster();
    virtual ~TContextScLoginMaster();

    void SetConnect(bool v);
    bool IsConnect();

    void SetTimeWait(unsigned int v);
    unsigned int GetTimeWait();

  private:
	};
}  
#endif