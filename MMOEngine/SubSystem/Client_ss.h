/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef CLIENT_SS_H
#define CLIENT_SS_H

#include "ContainerContextSc.h"

namespace nsMMOEngine
{
	class TClient_ss
	{
    unsigned int mID_SessionMaster;

    TContainerContextSc mC;

    unsigned int mIDClient;

  public:	

		TClient_ss();

    TContainerContextSc* GetC();

    void SetID_SessionMaster(unsigned int id_session);
    unsigned int GetID_SessionMaster();

    void SetIDClient(unsigned int id_client);
    unsigned int GetIDClient();
	};
}  
#endif