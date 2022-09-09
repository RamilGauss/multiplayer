/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
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

#include "GUI_Class.h"


#include "GUI_DescWindow.h"



TGUI_Class::TGUI_Class()
{

}
//------------------------------------------------------------------------
TGUI_Class::~TGUI_Class()
{
  Destroy();
}
//------------------------------------------------------------------------
bool TGUI_Class::AddWindow(TGUIDescWindow* descWindow)
{
  bool res =
#ifdef WIN32
  mMyGUI.addWindow(descWindow->hwnd,
                   descWindow->pD3D,
                   descWindow->pD3DDevice,
                   descWindow->pD3DPP,
                   descWindow->windowed);
#else // MacOS
#endif
  return res;
}
//------------------------------------------------------------------------
void TGUI_Class::Render()
{
  mMyGUI.render();
}
//------------------------------------------------------------------------
void TGUI_Class::Resize()
{
  mMyGUI._windowResizedPartly();
}
//------------------------------------------------------------------------
void TGUI_Class::Destroy()
{
  mMyGUI.destroyPartly();
}
//------------------------------------------------------------------------
void TGUI_Class::LostDevice()
{
  mMyGUI.lostDevice();
}
//------------------------------------------------------------------------
void TGUI_Class::AddResourceLocation(const std::string& _name, bool _recursive)
{
  mMyGUI.addResourceLocation(mMyGUI.getRootMedia() + _name,_recursive);
}
//------------------------------------------------------------------------
