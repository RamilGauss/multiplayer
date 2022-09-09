/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
2011, 2012, 2013
===========================================================================
                        Common Information
"TornadoEngine" GPL Source Code

This file is part of the "TornadoEngine" GPL Source Code.

"TornadoEngine" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"TornadoEngine" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "TornadoEngine" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "TornadoEngine" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "TornadoEngine" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 

#include "IGUI_Manager.h"

using namespace std;

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
  mMapNameForm.Add(name, pForm);
}
//-------------------------------------------------------------------
void* IGUI_ManagerForm::GetFormByName(string& name)
{
  void* p = NULL;
  mMapNameForm.GetValueByKey(name,p);
  return p;
}
//-------------------------------------------------------------------
string IGUI_ManagerForm::GetNameForm(void* pForm)
{
  string str;
  mMapNameForm.GetValueByKey(str,pForm);
  return str;
}
//-------------------------------------------------------------------
bool IGUI_ManagerForm::RemoveFormByName(string name)
{
  return mMapNameForm.RemoveValueByKey(name);
}
//-------------------------------------------------------------------
bool IGUI_ManagerForm::RemoveForm(void* pForm)
{
  return mMapNameForm.RemoveValue(pForm);
}
//-------------------------------------------------------------------
void IGUI_ManagerForm::Clear()
{
  mMapNameForm.Clear();
}
//-------------------------------------------------------------------
