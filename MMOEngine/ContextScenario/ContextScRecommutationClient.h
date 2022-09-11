/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef CONTEXT_SC_RECOMMUTATION_CLIENT_H
#define CONTEXT_SC_RECOMMUTATION_CLIENT_H

#include "IContextScenario.h"

namespace nsMMOEngine
{
  class TContextScRecommutationClient : public IContextScenario
	{
    unsigned int mID_SessionMasterSlave;

	public:
		TContextScRecommutationClient();
    virtual ~TContextScRecommutationClient();

    // ������ ��� Slave, � ��������� ������� ��� ������� � ������� ������������ G(S)etID_Session
    unsigned int GetID_SessionClientSlave();
    void SetID_SessionClientSlave(unsigned int id_session);

    unsigned int GetID_SessionMasterSlave();
    void SetID_SessionMasterSlave(unsigned int id_session);

  private:
	};
}  
#endif