/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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

#ifndef MakerGUIH
#define MakerGUIH

#include "IBaseGUI.h"
#include <string>
#include <map>
#include "IXML.h"
#include <vector>

class TMakerGUI
{
  // ��������� �������
  IXML* mXML;

  typedef std::map<std::string, nsTypeGUI::tTypeGUI> TMapClassNameTypeGUI;
  TMapClassNameTypeGUI mMap; 
public:
  enum tTypeBindGUI
  {
    min=0,// � ������� ������� 0 <- x
    max,  // � ������� ������� x -> max
  };// �������� � ����� �� ������ ����
  enum tPolicyGUI
  {
    fixed=0,
    preffered,
  };// �������� ��������
  //-----------------------------------------------------------------
  struct TPlaceGUI
  {
    tTypeBindGUI type;// �������� � ����� �� ������ ����
    tPolicyGUI policy;// �������� ��������
    int value;        // �������� �������� (���� %, ���� px)
    TPlaceGUI()
    {
      value  = 0;
      type   = min;
      policy = fixed;
    }
  };
  //-----------------------------------------------------------------
  struct TDefGUI
  {
    IBaseGUI*   pPublicGUI;
    void*       pPrivateGUI;
    std::string name;
    TPlaceGUI   place[2];
};
  
  
  TMakerGUI();
  virtual ~TMakerGUI();

  bool Load(const char* sFullPathXML);
  int GetCount();
  const TDefGUI* Get(int i) const;

protected:
  IBaseGUI* New(nsTypeGUI::tTypeGUI typeGUI);
  IBaseGUI* New(std::string nameClassGUI);

  typedef std::vector<TDefGUI> TVectorDefGUI;
  TVectorDefGUI mVectorDefGUI;

protected:
  bool AddDef(int i);
  bool ReadPlace(TDefGUI &vDef, int i);
  
  void SetupSpecificPropertyGUI(IBaseGUI* pBaseGUI);
  bool LoadGUI(const char* sFullPathXML);
  void SetParam(const char* nameParam,const char* param,IBaseGUI* pBaseGUI);

  IBaseGUI* pCurBaseGUI;
  void* pCurBaseGUI_Private;
};

#endif
