#include "Direct3DWidget.h"

#include <QMessageBox>
#define WIN32_LEAN_AND_MEAN 
#include <d3d9.h> 
#include <d3dx9.h> 
#include <d3d9.h>

// Constructor 
QD3DWidget::QD3DWidget( QWidget* pParent) 
: QWidget( pParent) 
{ 
  //ui.setupUi(this);

  mD3D = NULL; 
  mDevice = NULL; 

  setMinimumSize( 400, 400); 
  setAttribute( Qt::WA_OpaquePaintEvent, true);  
  setAttribute( Qt::WA_PaintOnScreen, true); 

  Setup(); 
} 

// Destructor 
QD3DWidget::~QD3DWidget() 
{ 
  Close(); 
} 

// Initialized the D3D environment 
void QD3DWidget::Setup() 
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
  // pack hier deine eigenen PresentationParams rein 
  D3DPRESENT_PARAMETERS d3dpp;    // create a struct to hold various device information

  ZeroMemory(&d3dpp, sizeof(d3dpp));    // clear out the struct for use
  d3dpp.Windowed = TRUE;    // program windowed, not fullscreen
  d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
  d3dpp.hDeviceWindow = windowHandle;    // set the window to be used by Direct3D


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

// Destroys the D3D environment 
void QD3DWidget::Close() 
{ 
  if( mDevice) 
    mDevice->Release(); 
  if( mD3D) 
    mD3D->Release(); 
} 

// paints the scene 
void QD3DWidget::paintEvent( QPaintEvent* pEvent) 
{ 
  // clear render buffer 
  HRESULT hr = mDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(250, 40, 40), 1.0f, 0);
  if( FAILED( hr)) 
    QMessageBox::critical(this,
    "ERROR",
    "Failed to clear backbuffer.",
    QMessageBox::Ok);

  mDevice->BeginScene();    // begins the 3D scene



  mDevice->EndScene();    // ends the 3D scene

  // and show the result 
  hr = mDevice->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen
  if( FAILED( hr)) 
    QMessageBox::critical(this,
    "ERROR",
    "Failed to Present().",
    QMessageBox::Ok);

  // trigger another update as soon as possible 
  update(); 
} 