/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_BASE_EVENT_H
#define MELISSA_BASE_EVENT_H

#include "Base.h"
#include "Container.h"

namespace nsMelissa
{
#if defined( WIN32 )
#pragma pack(push, 1)
#endif
  struct MELISSA_EI TBaseEvent
  {
    // переназначить в наследуемом классе
    TBase::tTypeEvent mType;
		TContainer c;
  };
#if defined( WIN32 )
#pragma pack(pop)
#endif

}

#endif