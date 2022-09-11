/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_CONTEXT_SC_SYNCHRO_SLAVE_H
#define MELISSA_CONTEXT_SC_SYNCHRO_SLAVE_H

#include "IContextScenario.h"
#include "BreakPacket.h"

namespace nsMelissa
{
  // ��������, ������������ � ���������� ������� �����������
  class TContextScSynchroSlave : public IContextScenario
	{
    unsigned char mLoadProcent;
	public:
		TContextScSynchroSlave();
    virtual ~TContextScSynchroSlave();

    void SetLoadProcent(unsigned char v);
    unsigned char GetLoadProcent();
  private:
	};
}  
#endif