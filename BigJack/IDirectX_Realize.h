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

#ifndef IDirectX_RealizeH
#define IDirectX_RealizeH

#include <windows.h>

class TBigJack;
class TGraphicEngineGUI;
//class D3DPRESENT_PARAMETERS;

class IDirectX_Realize
{
protected:

  TBigJack * pGE;

public:
  IDirectX_Realize(TBigJack * _pGE){pGE=_pGE;}
  virtual ~IDirectX_Realize(){};

  virtual void* GetWndProc() = 0;
  virtual bool  IsFullScreen() = 0;
  virtual void  ToggleFullScreen() = 0;
  virtual void  SetTitleWindow(const char* sTitle) = 0;
  virtual void  GetSizeWindow(int &w, int &h) = 0;

  virtual HRESULT Init() = 0;
  virtual void    Work() = 0;
  virtual int     Done()  = 0;

  virtual float   GetFPS() = 0;

  virtual HWND    GetHWND() = 0;
  
  virtual D3DPRESENT_PARAMETERS* GetDevicePresentParameters9() = 0;
  virtual IDirect3D9*       GetD3D9Object() = 0;
  virtual IDirect3DDevice9* GetD3D9Device() = 0;

  //virtual void*   GetFuncEventGUI() = 0;
  //virtual void    SetSrcGUI(TGraphicEngineGUI* pForm) = 0;

protected:
};


#endif
