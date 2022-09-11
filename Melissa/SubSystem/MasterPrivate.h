/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_MASTER_PRIVATE_H
#define MELISSA_MASTER_PRIVATE_H

#include <set>

#include "ContainerContextSc.h"

namespace nsMelissa
{
	class TMasterPrivate
	{
    unsigned int mID_SessionDown;
    TContainerContextSc mC;
  public:	

		TMasterPrivate();

    TContainerContextSc* GetC();

    bool SearchClient(unsigned int id_client);
    int GetCountClient();
	};
}  
#endif