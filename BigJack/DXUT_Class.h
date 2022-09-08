#ifndef DXUT_ClassH
#define DXUT_ClassH

#include "DXUT.h"
#include "DXUTcamera.h"
#include "DXUTsettingsdlg.h"
#include "SDKmisc.h"
#include "IDirectX_Realize.h"

class TDXUT : public IDirectX_Realize
{
  enum{eWaitDestroy=40};

  bool flgWasLostEvent;
  bool flgWasDestroyEvent;

public:
  TDXUT(TManagerDirectX * pMDX );
  virtual ~TDXUT();

  virtual void Init(HWND hwnd = NULL);
  virtual void Work();
  virtual int Done();

protected:

  friend bool CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed,
    void* pUserContext );
  friend bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );
  friend HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
    void* pUserContext );
  friend HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
    void* pUserContext );
  friend void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );
  friend void CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
  friend LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
    void* pUserContext );
  friend void CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
  friend void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
  friend void CALLBACK OnLostDevice( void* pUserContext );
  friend void CALLBACK OnDestroyDevice( void* pUserContext );

  bool IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed,
                                   void* pUserContext );
  bool ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );
  HRESULT OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                  void* pUserContext );
  HRESULT OnResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                 void* pUserContext );
  void OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );
  void OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
  LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
                           void* pUserContext );
  void OnLostDevice( void* pUserContext );
  void OnDestroyDevice( void* pUserContext );

};




#endif