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

#include "IGUI_Manager.h"

IGUI_ManagerForm::IGUI_ManagerForm()
{
}
//-------------------------------------------------------------------
IGUI_ManagerForm::~IGUI_ManagerForm()
{
  Clear();
}
//-------------------------------------------------------------------
void IGUI_ManagerForm::Add(std::string& name, void* pForm)
{
  mMapNameForm.insert(TMapNamePtr::value_type(name, pForm));
  mMapFormName.insert(TMapPtrName::value_type(pForm,name));
}
//-------------------------------------------------------------------
void* IGUI_ManagerForm::GetFormByName(std::string& name)
{
  TMapNamePtr::iterator fitName = mMapNameForm.find(name);
  TMapNamePtr::iterator eitName = mMapNameForm.end();
  if(fitName!=eitName)
  {
    return fitName->second;
  }
  return NULL;
}
//-------------------------------------------------------------------
const char* IGUI_ManagerForm::GetNameForm(void* pForm)
{
  TMapPtrName::iterator fitPtr = mMapFormName.find(pForm);
  TMapPtrName::iterator eitPtr = mMapFormName.end();
  if(fitPtr!=eitPtr)
  {
    return fitPtr->second.data();
  }
  return NULL;
}
//-------------------------------------------------------------------
bool IGUI_ManagerForm::RemoveFormByName(std::string name)
{
  bool res = false;
  TMapNamePtr::iterator fitName = mMapNameForm.find(name);
  TMapNamePtr::iterator eitName = mMapNameForm.end();
  if(fitName!=eitName)
  {
    TMapPtrName::iterator fitPtr = mMapFormName.find(fitName->second);
    TMapPtrName::iterator eitPtr = mMapFormName.end();
    if(fitPtr!=eitPtr)
    {
      mMapNameForm.erase(fitName);
      mMapFormName.erase(fitPtr);
      res = true;
    }
  }
  return res;
}
//-------------------------------------------------------------------
bool IGUI_ManagerForm::RemoveForm(void* pForm)
{
  bool res = false;
  TMapPtrName::iterator fitPtr = mMapFormName.find(pForm);
  TMapPtrName::iterator eitPtr = mMapFormName.end();
  if(fitPtr!=eitPtr)
  {
    TMapNamePtr::iterator fitName = mMapNameForm.find(fitPtr->second);
    TMapNamePtr::iterator eitName = mMapNameForm.end();
    if(fitName!=eitName)
    {
      mMapNameForm.erase(fitName);
      mMapFormName.erase(fitPtr);
      res = true;
    }
  }
  return res;
}
//-------------------------------------------------------------------
void IGUI_ManagerForm::Clear()
{
  mMapNameForm.clear();
  mMapFormName.clear();
}
//-------------------------------------------------------------------
