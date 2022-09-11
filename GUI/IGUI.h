/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef IGUIH
#define IGUIH

#include "IGUI_Core.h"
#include "IGUI_Manager.h"

class IGUI : public IGUI_Core,
             public IGUI_ManagerForm
{
public:

  IGUI(){};
  virtual ~IGUI(){}

};

#endif