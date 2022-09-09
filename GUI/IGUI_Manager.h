/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012
===========================================================================
                        Common Information
"Tanks" GPL Source Code

This file is part of the "Tanks" GPL Source Code.

"Tanks" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"Tanks" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "Tanks" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "Tanks" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "Tanks" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 

#ifndef IGUI_ManagerFormH
#define IGUI_ManagerFormH

#include <string>
#include <map>

class IGUI_ManagerForm
{
  typedef std::map<std::string, void*> TMapNamePtr;
  TMapNamePtr mMapNameForm;
  typedef std::map<void*,std::string> TMapPtrName;
  TMapPtrName mMapFormName;

public:

  // реализация класса не зависит от GUI
  IGUI_ManagerForm();
  virtual ~IGUI_ManagerForm();

  // добавление
  void Add(std::string& name, void* pForm);

  // поиск
  void* GetFormByName(std::string& name);
  const char* GetNameForm(void* pForm);// если не найдет вернет NULL

  // удаление
  bool RemoveFormByName(std::string name);
  bool RemoveForm(void*);

  // чистка
  void Clear();
protected:


};

#endif
