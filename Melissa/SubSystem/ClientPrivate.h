/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_CLIENT_PRIVATE_H
#define MELISSA_CLIENT_PRIVATE_H

#include "ContainerContextSc.h"

namespace nsMelissa
{
  // в Slave для контроля выполнения сценариев на сессии клиента
	class TClientPrivate
	{
    TContainerContextSc mC;

    unsigned int mClientKey;// ключ, с которым ассоциирован клиент
    unsigned int mID_SessionDown;
  public:	
		TClientPrivate();

    TContainerContextSc* GetC();
    
    void SetID_SessionDown(unsigned int v);
    unsigned int GetID_SessionDown();

    void SetKey(unsigned int v);
    unsigned int GetKey();
	};
}  
#endif