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

#include "DXUT_Class.h"
#include "BigJack.h"

using namespace std;

TDXUT* pDXUT_Realize = NULL;

//--------------------------------------------------------------------------------------
// Called during device initialization, this code checks the device for some 
// minimum set of capabilities, and rejects those that don't pass by returning false.
//--------------------------------------------------------------------------------------
bool CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat,
                                 D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
  if(pDXUT_Realize)
    return pDXUT_Realize->IsDeviceAcceptable( pCaps, AdapterFormat,BackBufferFormat, bWindowed, pUserContext );
  return false;
}
//--------------------------------------------------------------------------------------
bool TDXUT::IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat,
                                  D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
  if(pMDX)
    return pMDX->IsDeviceAcceptable( pCaps, AdapterFormat,BackBufferFormat, bWindowed, pUserContext );

  return false;
}
//--------------------------------------------------------------------------------------
// This callback function is called immediately before a device is created to allow the 
// application to modify the device settings. The supplied pDeviceSettings parameter 
// contains the settings that the framework has selected for the new device, and the 
// application can make any desired changes directly to this structure.  Note however that 
// DXUT will not correct invalid device settings so care must be taken 
// to return valid device settings, otherwise IDirect3D9::CreateDevice() will fail.  
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
  if(pDXUT_Realize)
    return pDXUT_Realize->ModifyDeviceSettings( pDeviceSettings, pUserContext );
  return false;
}
//--------------------------------------------------------------------------------------
bool TDXUT::ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
  if(pMDX)
    return pMDX->ModifyDeviceSettings( pDeviceSettings, pUserContext );
  return false;
}
//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has been 
// created, which will happen during application initialization and windowed/full screen 
// toggles. This is the best location to create D3DPOOL_MANAGED resources since these 
// resources need to be reloaded whenever the device is destroyed. Resources created  
// here should be released in the OnDestroyDevice callback. 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                 void* pUserContext )
{
  if(pDXUT_Realize)
    return pDXUT_Realize->OnCreateDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );
  return S_FALSE;
}
//--------------------------------------------------------------------------------------
HRESULT TDXUT::OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                 void* pUserContext )
{
  if(pMDX)
    return pMDX->OnCreateDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );
  return S_FALSE;
}
//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has been 
// reset, which will happen after a lost device scenario. This is the best location to 
// create D3DPOOL_DEFAULT resources since these resources need to be reloaded whenever 
// the device is lost. Resources created here should be released in the OnLostDevice 
// callback. 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice,
                               const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
  if(pDXUT_Realize)
    return pDXUT_Realize->OnResetDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );
  return S_FALSE;
}
//--------------------------------------------------------------------------------------
HRESULT TDXUT::OnResetDevice( IDirect3DDevice9* pd3dDevice,
                                const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
  if(pMDX)
    return pMDX->OnResetDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );
  return S_FALSE;
}
//--------------------------------------------------------------------------------------
// This callback function will be called once at the beginning of every frame. This is the
// best location for your application to handle updates to the scene, but is not 
// intended to contain actual rendering calls, which should instead be placed in the 
// OnFrameRender callback.  
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
  if(pDXUT_Realize)
    pDXUT_Realize->OnFrameMove(fTime, fElapsedTime, pUserContext);
}
//--------------------------------------------------------------------------------------
void TDXUT::OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
  if(pMDX)
    pMDX->OnFrameMove(fTime, fElapsedTime, pUserContext);
}
//--------------------------------------------------------------------------------------
// This callback function will be called at the end of every frame to perform all the 
// rendering calls for the scene, and it will also be called if the window needs to be 
// repainted. After this function has returned, DXUT will call 
// IDirect3DDevice9::Present to display the contents of the next buffer in the swap chain
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
  if(pDXUT_Realize)
    pDXUT_Realize->OnFrameRender( pd3dDevice, fTime, fElapsedTime, pUserContext );
}
//--------------------------------------------------------------------------------------
void TDXUT::OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
  if(pMDX)
    pMDX->OnFrameRender( pd3dDevice, fTime, fElapsedTime, pUserContext );
}
//--------------------------------------------------------------------------------------
// Before handling window messages, DXUT passes incoming windows 
// messages to the application through this callback function. If the application sets 
// *pbNoFurtherProcessing to TRUE, then DXUT will not process this message.
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
                          void* pUserContext )
{
  if(pDXUT_Realize)
    return pDXUT_Realize->MsgProc(hWnd, uMsg, wParam, lParam, pbNoFurtherProcessing,pUserContext);
  return 0;
}
//--------------------------------------------------------------------------------------
LRESULT TDXUT::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
                          void* pUserContext )
{
  if(pMDX)
    return pMDX->MsgProc(hWnd, uMsg, wParam, lParam, pbNoFurtherProcessing,pUserContext);
  return 0;
}
//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has 
// entered a lost state and before IDirect3DDevice9::Reset is called. Resources created
// in the OnResetDevice callback should be released here, which generally includes all 
// D3DPOOL_DEFAULT resources. See the "Lost Devices" section of the documentation for 
// information about lost devices.
//--------------------------------------------------------------------------------------
void CALLBACK OnLostDevice( void* pUserContext )
{
  if(pDXUT_Realize)
    pDXUT_Realize->OnLostDevice(pUserContext);
}
//--------------------------------------------------------------------------------------
void TDXUT::OnLostDevice( void* pUserContext )
{
  if(pMDX)
    pMDX->OnLostDevice(pUserContext);

  flgWasLostEvent = true;
}
//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has 
// been destroyed, which generally happens as a result of application termination or 
// windowed/full screen toggles. Resources created in the OnCreateDevice callback 
// should be released here, which generally includes all D3DPOOL_MANAGED resources. 
//--------------------------------------------------------------------------------------
void CALLBACK OnDestroyDevice( void* pUserContext )
{
  if(pDXUT_Realize)
    pDXUT_Realize->OnDestroyDevice(pUserContext);
}
//--------------------------------------------------------------------------------------
void TDXUT::OnDestroyDevice( void* pUserContext )
{
  if(pMDX)
    pMDX->OnDestroyDevice(pUserContext);

  flgWasDestroyEvent = true;
}
//--------------------------------------------------------------------------------------
TDXUT::TDXUT( TBigJack * _pMDX )
: IDirectX_Realize(_pMDX)
{
  pDXUT_Realize = this;
}
//--------------------------------------------------------------------------------------
TDXUT::~TDXUT()
{
  pDXUT_Realize = NULL;
}
//--------------------------------------------------------------------------------------
HRESULT TDXUT::Init(HWND hwnd )
{
  HRESULT hr;
  flgWasLostEvent    = false;
  flgWasDestroyEvent = false;

  DXUTSetCallbackMsgProc( ::MsgProc );
  //----------------------------------------------------
  DXUTSetCallbackD3D9DeviceAcceptable( ::IsDeviceAcceptable );
  DXUTSetCallbackDeviceChanging( ::ModifyDeviceSettings );
  DXUTSetCallbackD3D9DeviceCreated( ::OnCreateDevice );
  DXUTSetCallbackD3D9DeviceReset( ::OnResetDevice );

  //----------------------------------------------------
  DXUTSetCallbackFrameMove( ::OnFrameMove );
  DXUTSetCallbackD3D9FrameRender( ::OnFrameRender );
  //----------------------------------------------------
  DXUTSetCallbackD3D9DeviceLost( ::OnLostDevice );
  DXUTSetCallbackD3D9DeviceDestroyed( ::OnDestroyDevice );
  //----------------------------------------------------
  DXUTSetCursorSettings( true, true );

  if(FAILED(DXUTInit( true, true ))) return false; // Parse the command line and show msgboxes
  DXUTSetHotkeyHandling( true, true, true );  // handle the defaul hotkeys
  
  if(hwnd==NULL)
    V_RETURN(DXUTCreateWindow( L"TDXUT" ))
  else
  {
    // установить реакции
    // на "нашу" функцию
    LONG_PTR res = SetWindowLongPtr(hwnd,GWLP_WNDPROC,(LONG_PTR)DXUTStaticWndProc_Changed);
    V_RETURN(DXUTSetWindow( hwnd, hwnd, hwnd, false ))
  }
  DXUTCreateDevice( true, 1024, 768 );
  return S_OK;
}
//--------------------------------------------------------------------------------------
void TDXUT::Work()
{
  // вызов по таймеру
  DXUTRender3DEnvironment();
}
//--------------------------------------------------------------------------------------
int TDXUT::Done()
{
  int res = DXUTGetExitCode();
  DXUTShutdown();
  while(!flgWasLostEvent&&!flgWasDestroyEvent)
  {
    Sleep(eWaitDestroy);
  }
  pMDX = NULL;
  return res;
}
//--------------------------------------------------------------------------------------
std::string TDXUT::GetError()
{
  //DXUTGetErr
  return string();
}
//--------------------------------------------------------------------------------------
float TDXUT::GetFPS()
{
  return DXUTGetFPS();
}
//--------------------------------------------------------------------------------------
