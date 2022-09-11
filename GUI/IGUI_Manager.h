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

#ifndef IGUI_ManagerFormH
#define IGUI_ManagerFormH

#include <string>
#include <boost/bimap/bimap.hpp>

class IGUI_ManagerForm
{

  typedef boost::bimaps::bimap< std::string, void* > bmStrPtr;
  bmStrPtr mMapNameForm;

public:

  // ���������� ������ �� ������� �� GUI
  IGUI_ManagerForm();
  virtual ~IGUI_ManagerForm();

  // ����������
  void Add(std::string& name, void* pForm);

  // �����
  void* GetFormByName(std::string& name);
  std::string GetNameForm(void* pForm);// ���� �� ������ ������ NULL

  // ��������
  void RemoveFormByName(std::string name);
  void RemoveForm(void*);

  // ������
  void Clear();
protected:


};

#endif
