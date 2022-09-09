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

#ifndef ControlEventWinApiGEH
#define ControlEventWinApiGEH

#include "IControlEventWinApi.h"


class TControlEventWinApiGE : public IControlEventWinApi
{
  
  enum{
       // ������� ������ � ���� �������� �������
       eCameraRotate2XY=0,
       eCameraRotate2XZ,
       eCameraRotate2YZ,

       // ������� ������ � ���� �������� ������� 
       eCameraRotate3,
       eCameraOnManage,// �������� ���������� �������

       eCameraForward,
       eCameraBackward,
       eCameraLeft,
       eCameraRight,
  };


public:
  TControlEventWinApiGE();
  virtual ~TControlEventWinApiGE();


  virtual void Notify(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
  virtual std::string GetDesc(int value);

protected:
  void RotateCamera2XY(WPARAM wParam, LPARAM lParam);
  void RotateCamera2XZ(WPARAM wParam, LPARAM lParam);
  void RotateCamera2YZ(WPARAM wParam, LPARAM lParam);


};


#endif