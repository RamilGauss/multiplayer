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


#include "DX_main.h"

#include "DefineDX.h"
#include "GlobalParamsTank.h"

#include "ManagerDirectX.h"
#ifndef EDITOR_MODEL
  #include "Bufferizator2Thread.h"
#endif
#include <atlconv.h>
#include "LoggerDX.h"

#define LOG_DX

//--------------------------------------------------------------------------------------
// UI control IDs
//--------------------------------------------------------------------------------------
#define IDC_TOGGLEFULLSCREEN    1
#define IDC_TOGGLEREF           2
#define IDC_CHANGEDEVICE        3

//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
                         void* pUserContext );
void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );

bool CALLBACK IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat,
                                     bool bWindowed, void* pUserContext );
HRESULT CALLBACK OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                    void* pUserContext );
HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                   void* pUserContext );
void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
void CALLBACK OnD3D9LostDevice( void* pUserContext );
void CALLBACK OnD3D9DestroyDevice( void* pUserContext );


TDX *g_pDX = NULL;

TDX::TDX()
{
  g_pDX = this;

  g_pTxtHelper = NULL;
  g_pFont9 = NULL;
  g_pSprite9 = NULL;
  mCallBackExit = NULL;
  pManagerDirectX = NULL;
}
//-------------------------------------------------------------------------------
TDX::~TDX()
{
  g_pDX = NULL;
}
//-------------------------------------------------------------------------------
DWORD WINAPI ThreadDirectX(LPVOID pDX)
{
  // ������ ������������ �����, ���������� �����, ����������� ������� Key � Mouse
  ((TDX*)pDX)->Work();
  return 0;
}
//---------------------------------------------
bool TDX::Start(TManagerDirectX* pMDX, void* pFuncCallExit)
{
  hWnd = NULL;
  flgWasStop = false;
  mCallBackExit = (TCallBackExit)pFuncCallExit;
  pManagerDirectX = pMDX;
  DWORD ThreadId;
  HANDLE handle = CreateThread(NULL, // default security attributes
    0,                               // use default stack size  
    ThreadDirectX,                   // thread function 
    this,                            // argument to thread function 
    0,                               // use default creation flags 
    &ThreadId);                      // returns the thread identifier 
  if(handle==NULL)
    return false;
  while(flgActive==false||hWnd==NULL)
  {
    ht_msleep(eTimeWaitStart);// ���� ������� 
  }
  return (handle!=NULL);
}
//-------------------------------------------------------------------------------
void TDX::Work()
{
  flgActive  = true;
  MainLoop();
  if(mCallBackExit&&!flgWasStop)
    mCallBackExit();
  flgActive = false;
}
//-------------------------------------------------------------------------------
void TDX::Show()
{
  ShowWindow(hWnd,SW_SHOW);
} 
//--------------------------------------------------------------------------------------------------------
void TDX::Hide()
{
  ShowWindow(hWnd,SW_HIDE);
}
//--------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int TDX::MainLoop()
{
#ifndef EDITOR_MODEL
  pManagerDirectX->GetPipeLine()->ClearWasRegisterCallback();
#endif
  pManagerDirectX->flgNeedSendCorrectPacket = false;//������
  // DXUT will create and use the best device (either D3D9 or D3D10) 
  // that is available on the system depending on which D3D callbacks are set below

  DXUTSetCallbackMsgProc( ::MsgProc );

  DXUTSetCallbackKeyboard( ::OnKeyboard );
  DXUTSetCallbackFrameMove( ::OnFrameMove );
  DXUTSetCallbackDeviceChanging( ::ModifyDeviceSettings );

  DXUTSetCallbackD3D9DeviceAcceptable( ::IsD3D9DeviceAcceptable );
  DXUTSetCallbackD3D9DeviceCreated( ::OnD3D9CreateDevice );
  DXUTSetCallbackD3D9DeviceReset( ::OnD3D9ResetDevice );
  DXUTSetCallbackD3D9DeviceLost( ::OnD3D9LostDevice );
  DXUTSetCallbackD3D9DeviceDestroyed( ::OnD3D9DestroyDevice );
  DXUTSetCallbackD3D9FrameRender( ::OnD3D9FrameRender );

  InitApp();
  DXUTInit( true, true, NULL ); // Parse the command line, show msgboxes on error, no extra command line params
  DXUTSetCursorSettings( true, true );
  USES_CONVERSION;
#ifndef EDITOR_MODEL    
  DXUTCreateWindow( &hWnd, A2W(STR_VERSION_CLIENT) );
#else
  DXUTCreateWindow( &hWnd, A2W(STR_VERSION_EDITOR) );
#endif
  DXUTCreateDevice( true, 640, 480 );
  DXUTMainLoop(pManagerDirectX); // Enter into the DXUT render loop
  DXUTDestroyState();// ���������� DXUT
#ifndef EDITOR_MODEL    
  pManagerDirectX->GetPipeLine()->UnregisterFromClientTank();
#endif
  //--------------------------------------------------
  return 0;
}
//--------------------------------------------------------------------------------------
bool TDX::Stop()
{
  flgWasStop = true;

  if(flgActive==false) return true;
#ifdef LOG_DX
  GlobalLoggerDX.WriteF_time("TDX::Stop(): PostMessage().\n");
#endif
  bool res = PostMessage( hWnd,WM_STOP_DXUT/*WM_QUIT*/,0,NULL);
  hWnd = NULL;
  if(res==false) return false;

  while(flgActive)
  {
    ht_msleep(eTimeWaitStop);
  }
  return true;
}
//--------------------------------------------------------------------------------------
// Initialize the app 
//--------------------------------------------------------------------------------------
void TDX::InitApp()
{
  g_SettingsDlg.Init( &g_DialogResourceManager );
  g_HUD.Init( &g_DialogResourceManager );
  g_SampleUI.Init( &g_DialogResourceManager );

  g_HUD.SetCallback( ::OnGUIEvent ); int iY = 10;
  g_HUD.AddButton( IDC_TOGGLEFULLSCREEN, L"Toggle full screen", 35, iY, 125, 22 );
  g_HUD.AddButton( IDC_TOGGLEREF, L"Toggle REF (F3)", 35, iY += 24, 125, 22, VK_F3 );
  g_HUD.AddButton( IDC_CHANGEDEVICE, L"Change device (F2)", 35, iY += 24, 125, 22, VK_F2 );

  g_SampleUI.SetCallback( ::OnGUIEvent ); iY = 10;
}
//--------------------------------------------------------------------------------------
// Render the help and statistics text. This function uses the ID3DXFont interface for 
// efficient text rendering.
//--------------------------------------------------------------------------------------
void TDX::RenderText()
{
  g_pTxtHelper->Begin();
  g_pTxtHelper->SetInsertionPos( 5, 5 );
  g_pTxtHelper->SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
  g_pTxtHelper->DrawTextLine( DXUTGetFrameStats( DXUTIsVsyncEnabled() ) );
  g_pTxtHelper->DrawTextLine( DXUTGetDeviceStats() );
  g_pTxtHelper->End();
}
//--------------------------------------------------------------------------------------
// Rejects any D3D9 devices that aren't acceptable to the app by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat,
                                      D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
  if(g_pDX)
    return g_pDX->IsD3D9DeviceAcceptable( pCaps, AdapterFormat, BackBufferFormat, bWindowed, pUserContext );
  return false;
}
//--------------------------------------------------------------------------------------
bool TDX::IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat,
                                     D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
  // Skip backbuffer formats that don't support alpha blending
  IDirect3D9* pD3D = DXUTGetD3D9Object();
  if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
    AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
    D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
    return false;

  // No fallback defined by this app, so reject any device that 
  // doesn't support at least ps2.0
  if( pCaps->PixelShaderVersion < D3DPS_VERSION( 2, 0 ) )
    return false;

  return true;
}
//--------------------------------------------------------------------------------------
// Called right before creating a D3D9 or D3D10 device, allowing the app to modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
  if(g_pDX)
    return g_pDX->ModifyDeviceSettings( pDeviceSettings, pUserContext );
  return false;
}
//--------------------------------------------------------------------------------------
bool TDX::ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
  if( pDeviceSettings->ver == DXUT_D3D9_DEVICE )
  {
    IDirect3D9* pD3D = DXUTGetD3D9Object();
    D3DCAPS9 Caps;
    pD3D->GetDeviceCaps( pDeviceSettings->d3d9.AdapterOrdinal, pDeviceSettings->d3d9.DeviceType, &Caps );

    // If device doesn't support HW T&L or doesn't support 1.1 vertex shaders in HW 
    // then switch to SWVP.
    if( ( Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT ) == 0 ||
      Caps.VertexShaderVersion < D3DVS_VERSION( 1, 1 ) )
    {
      pDeviceSettings->d3d9.BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }

    // Debugging vertex shaders requires either REF or software vertex processing 
    // and debugging pixel shaders requires REF.  
#ifdef DEBUG_VS
    if( pDeviceSettings->d3d9.DeviceType != D3DDEVTYPE_REF )
    {
      pDeviceSettings->d3d9.BehaviorFlags &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
      pDeviceSettings->d3d9.BehaviorFlags &= ~D3DCREATE_PUREDEVICE;
      pDeviceSettings->d3d9.BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }
#endif
#ifdef DEBUG_PS
    pDeviceSettings->d3d9.DeviceType = D3DDEVTYPE_REF;
#endif
  }

  // For the first device created if its a REF device, optionally display a warning dialog box
  static bool s_bFirstTime = true;
  if( s_bFirstTime )
  {
    s_bFirstTime = false;
    if( ( DXUT_D3D9_DEVICE == pDeviceSettings->ver && pDeviceSettings->d3d9.DeviceType == D3DDEVTYPE_REF ) ||
      ( DXUT_D3D10_DEVICE == pDeviceSettings->ver &&
      pDeviceSettings->d3d10.DriverType == D3D10_DRIVER_TYPE_REFERENCE ) )
      DXUTDisplaySwitchingToREFWarning( pDeviceSettings->ver );
  }

  return true;
}
//--------------------------------------------------------------------------------------
// Create any D3D9 resources that will live through a device reset (D3DPOOL_MANAGED)
// and aren't tied to the back buffer size
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                         void* pUserContext )
{
  if(g_pDX)
    return g_pDX->OnD3D9CreateDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );
  return 0;
}
//--------------------------------------------------------------------------------------
HRESULT TDX::OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                    void* pUserContext )
{
  HRESULT hr;
  V_RETURN( g_DialogResourceManager.OnD3D9CreateDevice( pd3dDevice ) );
  V_RETURN( g_SettingsDlg.OnD3D9CreateDevice( pd3dDevice ) );

  V_RETURN( D3DXCreateFont( pd3dDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
    L"Arial", &g_pFont9 ) );

  pManagerDirectX->CreateDeviceEvent(pd3dDevice, pBackBufferSurfaceDesc, pUserContext);
  return S_OK;
}
//--------------------------------------------------------------------------------------
// Create any D3D9 resources that won't live through a device reset (D3DPOOL_DEFAULT) 
// or that are tied to the back buffer size 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice,
                                        const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
  if(g_pDX)
    g_pDX->OnD3D9ResetDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );

  return 0;
}
//--------------------------------------------------------------------------------------
HRESULT TDX::OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice,
                                   const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
  HRESULT hr;

  V_RETURN( g_DialogResourceManager.OnD3D9ResetDevice() );
  V_RETURN( g_SettingsDlg.OnD3D9ResetDevice() );

  if( g_pFont9 ) V_RETURN( g_pFont9->OnResetDevice() );
  
  V_RETURN( D3DXCreateSprite( pd3dDevice, &g_pSprite9 ) );
  g_pTxtHelper = new CDXUTTextHelper( g_pFont9, g_pSprite9, NULL, NULL, 15 );

  pManagerDirectX->ResetDevice(pd3dDevice,pBackBufferSurfaceDesc, pUserContext);

  g_HUD.SetLocation( pBackBufferSurfaceDesc->Width - 170, 0 );
  g_HUD.SetSize( 170, 170 );
  g_SampleUI.SetLocation( pBackBufferSurfaceDesc->Width - 170, pBackBufferSurfaceDesc->Height - 350 );
  g_SampleUI.SetSize( 170, 300 );

  return S_OK;
}
//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
  if(g_pDX)
    g_pDX->OnFrameMove( fTime, fElapsedTime, pUserContext );
}
//--------------------------------------------------------------------------------------
void TDX::OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
  // Update the camera's position based on user input 
  pManagerDirectX->getCamera()->FrameMove( fElapsedTime );
  pManagerDirectX->MouseEvent(fTime, fElapsedTime, pUserContext);
}
//--------------------------------------------------------------------------------------
// Render the scene using the D3D9 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
  if(g_pDX)
    g_pDX->OnD3D9FrameRender( pd3dDevice, fTime, fElapsedTime, pUserContext );
}
//--------------------------------------------------------------------------------------
void TDX::OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
#if 0
  HRESULT hr;
  D3DXMATRIXA16 mWorld;
  D3DXMATRIXA16 mView;
  D3DXMATRIXA16 mProj;
  D3DXMATRIXA16 mWorldViewProjection;

  // If the settings dialog is being shown, then render it instead of rendering the app's scene
  if( g_SettingsDlg.IsActive() )
  {
    g_SettingsDlg.OnRender( fElapsedTime );
    return;
  }

  // Clear the render target and the zbuffer 
  V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 45, 50, 170 ), 1.0f, 0 ) );

  // Render the scene
  if( SUCCEEDED( pd3dDevice->BeginScene() ) )
  {
    // Get the projection & view matrix from the camera class
    mWorld = *g_Camera.GetWorldMatrix();
    mProj = *g_Camera.GetProjMatrix();
    mView = *g_Camera.GetViewMatrix();

    mWorldViewProjection = mWorld * mView * mProj;

    // Update the effect's variables.  Instead of using strings, it would 
    // be more efficient to cache a handle to the parameter by calling 
    // ID3DXEffect::GetParameterByName
    V( g_pEffect9->SetMatrix( g_hmWorldViewProjection, &mWorldViewProjection ) );
    V( g_pEffect9->SetMatrix( g_hmWorld, &mWorld ) );
    V( g_pEffect9->SetFloat( g_hfTime, ( float )fTime ) );

    DXUT_BeginPerfEvent( DXUT_PERFEVENTCOLOR, L"HUD / Stats" ); // These events are to help PIX identify what the code is doing
    RenderText();
    V( g_HUD.OnRender( fElapsedTime ) );
    V( g_SampleUI.OnRender( fElapsedTime ) );
    DXUT_EndPerfEvent();

    V( pd3dDevice->EndScene() );
  }
#endif
  pManagerDirectX->VisualEvent(pd3dDevice, fTime, fElapsedTime, pUserContext);
}
//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
                              void* pUserContext )
{
  if(g_pDX)
    return g_pDX->MsgProc( hWnd, uMsg, wParam, lParam, pbNoFurtherProcessing, pUserContext );
  return 0;
}
//--------------------------------------------------------------------------------------
LRESULT TDX::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
                         void* pUserContext )
{
  // Pass messages to dialog resource manager calls so GUI state is updated correctly
  *pbNoFurtherProcessing = g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
  if( *pbNoFurtherProcessing )
    return 0;

  // Pass messages to settings dialog if its active
  if( g_SettingsDlg.IsActive() )
  {
    g_SettingsDlg.MsgProc( hWnd, uMsg, wParam, lParam );
    return 0;
  }

  // Give the dialogs a chance to handle the message first
  *pbNoFurtherProcessing = g_HUD.MsgProc( hWnd, uMsg, wParam, lParam );
  if( *pbNoFurtherProcessing )
    return 0;
  *pbNoFurtherProcessing = g_SampleUI.MsgProc( hWnd, uMsg, wParam, lParam );
  if( *pbNoFurtherProcessing )
    return 0;

  // Pass all remaining windows messages to camera so it can respond to user input
  pManagerDirectX->getCamera()->HandleMessages( hWnd, uMsg, wParam, lParam );

  return 0;
}
//--------------------------------------------------------------------------------------
// Handle key presses
//--------------------------------------------------------------------------------------
void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
  if(g_pDX)
    g_pDX->OnKeyboard( nChar, bKeyDown, bAltDown, pUserContext );
}
//--------------------------------------------------------------------------------------
void TDX::OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
  pManagerDirectX->KeyEvent(nChar, bKeyDown, bAltDown, pUserContext);
}
//--------------------------------------------------------------------------------------
// Handles the GUI events
//--------------------------------------------------------------------------------------
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
  if(g_pDX)
    g_pDX->OnGUIEvent( nEvent, nControlID, pControl, pUserContext );
}
//--------------------------------------------------------------------------------------
void TDX::OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
  switch( nControlID )
  {
    case IDC_TOGGLEFULLSCREEN:
      DXUTToggleFullScreen(); break;
    case IDC_TOGGLEREF:
      DXUTToggleREF(); break;
    case IDC_CHANGEDEVICE:
      g_SettingsDlg.SetActive( !g_SettingsDlg.IsActive() ); break;
  }
}
//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9ResetDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9LostDevice( void* pUserContext )
{
  if(g_pDX)
    g_pDX->OnD3D9LostDevice( pUserContext );
}
//--------------------------------------------------------------------------------------
void TDX::OnD3D9LostDevice( void* pUserContext )
{
  g_DialogResourceManager.OnD3D9LostDevice();
  g_SettingsDlg.OnD3D9LostDevice();
  if( g_pFont9 ) g_pFont9->OnLostDevice();
  
  pManagerDirectX->OnLostDevice();

  SAFE_RELEASE( g_pSprite9 );
  SAFE_DELETE( g_pTxtHelper );
}
//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9CreateDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9DestroyDevice( void* pUserContext )
{
  if(g_pDX)
    g_pDX->OnD3D9DestroyDevice( pUserContext );
}
//--------------------------------------------------------------------------------------
void TDX::OnD3D9DestroyDevice( void* pUserContext )
{
  g_DialogResourceManager.OnD3D9DestroyDevice();
  g_SettingsDlg.OnD3D9DestroyDevice();

  pManagerDirectX->OnDestroyDevice();
  SAFE_RELEASE( g_pFont9 );
}
//--------------------------------------------------------------------------------------