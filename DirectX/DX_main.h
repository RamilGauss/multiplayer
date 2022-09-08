#ifndef DX_mainH
#define DX_mainH

#include "DXUT.h"
#include "DXUTgui.h"
#include "DXUTmisc.h"
#include "DXUTCamera.h"
#include "DXUTSettingsDlg.h"
#include "SDKmisc.h"
#include "SDKmesh.h"
#include "VisualDX.h"
#include "SoundDX.h"
#include "MouseDX.h"
#include "KeyDX.h"


class TDX
{
protected:
	typedef void(*TCallBackExit)();

  TCallBackExit mCallBackExit;

  CModelViewerCamera          g_Camera;                // A model viewing camera
  CDXUTDialogResourceManager  g_DialogResourceManager; // manager for shared resources of dialogs
  CD3DSettingsDlg             g_SettingsDlg;           // Device settings dialog
  CDXUTTextHelper*            g_pTxtHelper;
  CDXUTDialog                 g_HUD;                   // dialog for standard controls
  CDXUTDialog                 g_SampleUI;              // dialog for sample specific controls

  // Direct3D 9 resources
  ID3DXFont*                  g_pFont9;
  ID3DXSprite*                g_pSprite9;
  ID3DXEffect*                g_pEffect9;
  D3DXHANDLE                  g_hmWorldViewProjection;
  D3DXHANDLE                  g_hmWorld;
  D3DXHANDLE                  g_hfTime;
  
  HWND hWnd;// хэндл окна

  TVisualDX mVisual;
  TSoundDX  mSound;
  TMouseDX  mMouse;
  TKeyDX    mKey;

  bool flgWasStop;
public:
  TDX();
  ~TDX();

  void Work(void* pFuncCallExit);
  bool Stop();

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