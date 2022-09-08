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
the "Tanks" Source Code.  If not, please request a copy in writing from id Software at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 

#ifndef BaseGUI_DXH
#define BaseGUI_DXH

#include "BaseGUI.h"

struct IDirect3D9; 
struct IDirect3DDevice9; 

template <class Robert, class Client, class BigJack>
class TBaseGUI_DX : public TBaseGUI<Robert, Client, BigJack>
{ 
public: 
  /** Constructor */ 
  TBaseGUI_DX( QWidget* pParent = NULL); 

  /** Destructor */ 
  ~TBaseGUI_DX(); 

  /** a hint to Qt to give the widget as much space as possible */ 
  QSizePolicy sizePolicy() const { return QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ); } 
  /** a hint to Qt that we take care of the drawing for ourselves, thankyouverymuch */ 
  QPaintEngine *paintEngine() const { return NULL; } 

protected: 
  /** Initialized the D3D environment */ 
  void Setup(); 

  /** Destroys the D3D environment */ 
  void Close(); 

  /** paints the scene */ 
  virtual void paintEvent( QPaintEvent* pEvent); 

  virtual void VisualEvent(QPaintEvent* pEvent) = 0;
  //mManagerDirectX->VisualEvent(mDevice);// ������ �������������

protected: 
  /** D3D stuff */ 
  IDirect3D9*       mD3D; 
  IDirect3DDevice9* mDevice; 
}; 

#endif // _QD3DWIDGET_H_ 