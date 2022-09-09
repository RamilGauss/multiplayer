/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
2011, 2012
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
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 

#ifndef GUI_DescWindowH
#define GUI_DescWindowH

#ifdef WIN32
  #include <windows.h>
  #include <d3d9types.h>
  #include <d3d9.h>
#else 

#endif

struct TGUIDescWindow
{
#ifdef WIN32
  HWND                   hwnd;
  IDirect3D9*            pD3D;         
  IDirect3DDevice9*      pD3DDevice;   
  D3DPRESENT_PARAMETERS* pD3DPP;
#else // MacOS

#endif
  bool windowed;
};

struct TGUIDescToggleWindow
{
#ifdef WIN32
  HWND hWnd;
#else
#endif
  int width;
  int height;
  bool fullScreen;
};


#endif