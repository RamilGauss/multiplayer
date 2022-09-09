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

#include "BaseGUI_DX.h"
#include "ManagerObjectCommon.h"

TBaseGUI_DX::TBaseGUI_DX( QWidget* pParent) 
: TBaseGUI( pParent ) 
{ 
  //###
  // эксперимент
  HWND hwnd = winId();
  TFilterWinApi::SetWndProc(&hwnd);
  //###
  setMinimumSize( 400, 400); 
  setAttribute( Qt::WA_OpaquePaintEvent, true);  
  setAttribute( Qt::WA_PaintOnScreen, true); 

  connect(&mTimer, SIGNAL(timeout()), this, SLOT(sl_Rendering()));
} 
//-------------------------------------------------------------------------
TBaseGUI_DX::~TBaseGUI_DX()
{
  if(pGame)
    pGame->Done();
}
//-------------------------------------------------------------------------
void TBaseGUI_DX::SetupEvent()
{
  HWND hwnd = winId();
  pGame->Init(hwnd);
}
//---------------------------------------------------------
void TBaseGUI_DX::sl_Rendering()
{
  pGame->Work();
}
//---------------------------------------------------------
void TBaseGUI_DX::showGUI()
{
  TBaseGUI::show();
  mTimer.start();
}
//---------------------------------------------------------------------------------------------
void TBaseGUI_DX::hideGUI()
{
  TBaseGUI::hide();
  mTimer.stop();
}
//---------------------------------------------------------------------------------------------