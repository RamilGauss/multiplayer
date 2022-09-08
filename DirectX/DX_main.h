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


#ifndef DX_mainH
#define DX_mainH

#include "DXUT.h"
#include "DXUTgui.h"
#include "DXUTmisc.h"
#include "DXUTCamera.h"
#include "DXUTSettingsDlg.h"
#include "SDKmisc.h"
#include "SDKmesh.h"


class TDX
{
protected:
	typedef void(*TCallBackExit)();

  TCallBackExit mCallBackExit;

  CDXUTDialogResourceManager  g_DialogResourceManager; // manager for shared resources of dialogs
  CD3DSettingsDlg             g_SettingsDlg;           // Device settings dialog
  CDXUTTextHelper*            g_pTxtHelper;
  CDXUTDialog                 g_HUD;                   // dialog for standard controls
  CDXUTDialog                 g_SampleUI;              // dialog for sample specific controls

  // Direct3D 9 resources
  ID3DXFont*                  g_pFont9;
  ID3DXSprite*                g_pSprite9;
  HWND hWnd;// хэндл окна

  bool flgWasStop;
  bool flgActive;
  TManagerDirectX* pManagerDirectX;

  enum{eTimeWaitStop=50,// мс
       eTimeWaitStart=50,
  };

public:
  TDX();
  ~TDX();

  bool Start(TManagerDirectX* pMDX, void* pFuncCallExit);
  bool Stop();
  void Show();
  void Hide();


protected:
  friend DWORD WINAPI ThreadDirectX(LPVOID pDX);
  void Work();
protected:
  //----------------------------------------------------------------------------
  // friend functions
  //----------------------------------------------------------------------------
  friend LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext );
  friend void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
  friend void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
  friend void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );
  friend bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );

  friend bool CALLBACK IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
  friend HRESULT CALLBACK OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
  friend HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
  friend void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
  friend void CALLBACK OnD3D9LostDevice( void* pUserContext );
  friend void CALLBACK OnD3D9DestroyDevice( void* pUserContext );



  LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext );
  void OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
  void OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
  void OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );
  bool ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );

  bool IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
  HRESULT OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
  HRESULT OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
  void OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
  void OnD3D9LostDevice( void* pUserContext );
  void OnD3D9DestroyDevice( void* pUserContext );

protected:

  void InitApp();
  void RenderText();

  int MainLoop();

private:

  
};

#endif