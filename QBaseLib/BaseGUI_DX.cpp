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
the "Tanks" Source Code.  If not, please request a copy in writing from id Software at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 

#include "BaseGUI_DX.h"

#include <QMessageBox>
#define WIN32_LEAN_AND_MEAN 
#include <d3d9.h> 
#include <d3dx9.h> 

// Constructor 
template <class Robert, class Client, class BigJack>
TBaseGUI_DX<Robert, Client, BigJack>::TBaseGUI_DX( QWidget* pParent) 
: TBaseGUI<Robert, Client, BigJack>( pParent ) 
{ 
  mD3D = NULL; 
  mDevice = NULL; 

  setMinimumSize( 400, 400); 
  setAttribute( Qt::WA_OpaquePaintEvent, true);  
  setAttribute( Qt::WA_PaintOnScreen, true); 

  Setup(); 
} 
//-------------------------------------------------------------------------
// Destructor 
template <class Robert, class Client, class BigJack>
TBaseGUI_DX<Robert, Client, BigJack>::~TBaseGUI_DX() 
{ 
  Close(); 
} 
//-------------------------------------------------------------------------
// Initialized the D3D environment 
template <class Robert, class Client, class BigJack>
void TBaseGUI_DX<Robert, Client, BigJack>::Setup() 
{ 
  HWND windowHandle = winId(); 

  // create Direct3D9 object 
  mD3D = Direct3DCreate9( D3D_SDK_VERSION); 
  if( NULL == mD3D) 
    QMessageBox::critical(this,
    "ERROR",
    "Failed to create D3D object",
    QMessageBox::Ok);

  // create D3D device 
  D3DPRESENT_PARAMETERS d3dpp;    // create a struct to hold various device information

  ZeroMemory(&d3dpp, sizeof(d3dpp));    // clear out the struct for use
  d3dpp.Windowed = TRUE;                // program windowed, not fullscreen
  d3dpp.hDeviceWindow = windowHandle;    // set the window to be used by Direct3D
  d3dpp.BackBufferHeight = 100;
  d3dpp.BackBufferWidth  = 100;
  d3dpp.BackBufferCount  = 1;
  d3dpp.SwapEffect             = D3DSWAPEFFECT_COPY;
  d3dpp.EnableAutoDepthStencil = TRUE;
  d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

  HRESULT hr = mD3D->CreateDevice(D3DADAPTER_DEFAULT,
    D3DDEVTYPE_HAL,
    windowHandle,
    D3DCREATE_SOFTWARE_VERTEXPROCESSING,
    &d3dpp,
    &mDevice);

  if( FAILED( hr)) 
    QMessageBox::critical(this,
    "ERROR",
    "Failed to create D3D device",
    QMessageBox::Ok);
} 
//-------------------------------------------------------------------------
// Destroys the D3D environment 
template <class Robert, class Client, class BigJack>
void TBaseGUI_DX<Robert, Client, BigJack>::Close() 
{ 
  if( mDevice ) 
    mDevice->Release(); 
  if( mD3D ) 
    mD3D->Release(); 
} 
//-------------------------------------------------------------------------
// paints the scene 
template <class Robert, class Client, class BigJack>
void TBaseGUI_DX<Robert, Client, BigJack>::paintEvent( QPaintEvent* pEvent ) 
{ 
  VisualEvent(pEvent);
#if 0
  // clear render buffer 
  HRESULT hr = mDevice->Clear(0, NULL, 
                              D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
                              D3DCOLOR_XRGB(250, 40, 40), 1.0f, 0);
  if( FAILED( hr)) 
    QMessageBox::critical(this,
    "ERROR",
    "Failed to clear backbuffer.",
    QMessageBox::Ok);

  mDevice->BeginScene();    // begins the 3D scene



  mDevice->EndScene();    // ends the 3D scene
#endif
  // and show the result 
  HRESULT hr = mDevice->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen
  if( FAILED( hr)) 
    QMessageBox::critical(this,
    "ERROR",
    "Failed to Present().",
    QMessageBox::Ok);

  // trigger another update as soon as possible 
  update(); 
} 
//-------------------------------------------------------------------------
