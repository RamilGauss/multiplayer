/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef BASE_EVENT_H
#define BASE_EVENT_H

#include "Base.h"
#include "Container.h"

namespace nsMMOEngine
{
#if defined( WIN32 )
#pragma pack(push, 1)
#endif
  struct MMO_ENGINE_EI TBaseEvent
  {
    // ������������� � ����������� ������
    TBase::tTypeEvent mType;
		TContainer c;
  };
#if defined( WIN32 )
#pragma pack(pop)
#endif

}

#endif