/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef CLIENT_SLAVE_H
#define CLIENT_SLAVE_H

#include "ContainerContextSc.h"

namespace nsMMOEngine
{
  // в Slave для контроля выполнения сценариев на сессии клиента
	class TClient_slave
	{
    TContainerContextSc mC;

    unsigned int mClientKey;// ключ, с которым ассоциирован клиент
    unsigned int mID_SessionDown;
  public:	
		TClient_slave();

    TContainerContextSc* GetC();
    
    void SetID_SessionDown(unsigned int v);
    unsigned int GetID_SessionDown();

    void SetKey(unsigned int v);
    unsigned int GetKey();
	};
}  
#endif