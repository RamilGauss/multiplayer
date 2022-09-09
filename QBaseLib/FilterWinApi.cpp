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

#include "FilterWinApi.h"

#include "qwindowdefs_win.h"
#include <wtypes.h>

typedef LRESULT (*DeclQtWndProc)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

static DeclQtWndProc QtWndProc = NULL;

//-------------------------------------------------------------------------------
bool TFilterWinApi::WinApiEvent(void* phWnd, void* puMsg, void* pwParam, void* plParam)
{
  if(QtWndProc==NULL) return false;

  HWND hWnd = *((HWND*)phWnd);
  UINT uMsg = *((UINT*) puMsg);
  WPARAM wParam = *((WPARAM*) pwParam);  
  LPARAM lParam = *((LPARAM*) plParam);

  LRESULT result = 0;
  switch( uMsg )
  {
    //case WM_PAINT:
      //break;
    default:
      //result = QtWndProc(hWnd,uMsg,wParam,lParam);
      ;
  }

  return false;
}
//-------------------------------------------------------------------------------
void TFilterWinApi::SetWndProc(void* pHWND)
{
  HWND hWnd = *((HWND*)pHWND);
  LONG_PTR p = GetWindowLongPtr(hWnd, GWLP_WNDPROC);

  QtWndProc = (DeclQtWndProc)p;
}
//-------------------------------------------------------------------------------