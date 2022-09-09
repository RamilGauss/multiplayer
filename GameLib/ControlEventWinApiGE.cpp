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

#include "ControlEventWinApiGE.h"
#include "BL_Debug.h"

using namespace std;


TControlEventWinApiGE::TControlEventWinApiGE()
{

}
//---------------------------------------------------------------------------------------------------------
TControlEventWinApiGE::~TControlEventWinApiGE()
{

}
//---------------------------------------------------------------------------------------------------------
void TControlEventWinApiGE::Notify(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  int key = GetValByKey(uMsg);
  switch(key)
  {
    case eCameraRotate2XY:
      RotateCamera2XY(wParam, lParam);
      break;
    case eCameraRotate2XZ:
      RotateCamera2XZ(wParam, lParam);
      break;
    case eCameraRotate2YZ:
      RotateCamera2YZ(wParam, lParam);
      break;
    default:;
  }
}
//---------------------------------------------------------------------------------------------------------
string TControlEventWinApiGE::GetDesc(int key)
{
  return std::string();
}
//---------------------------------------------------------------------------------------------------------
void TControlEventWinApiGE::RotateCamera2XY(WPARAM wParam, LPARAM lParam)
{
  int xPos = ( short )LOWORD( lParam );
  int yPos = ( short )HIWORD( lParam );
  int a = 0;
}
//---------------------------------------------------------------------------------------------------------
void TControlEventWinApiGE::RotateCamera2XZ(WPARAM wParam, LPARAM lParam)
{

}
//---------------------------------------------------------------------------------------------------------
void TControlEventWinApiGE::RotateCamera2YZ(WPARAM wParam, LPARAM lParam)
{

}
//---------------------------------------------------------------------------------------------------------
