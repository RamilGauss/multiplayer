/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
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
aaaaaaaYou should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "TornadoEngine" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 
#define _USE_MATH_DEFINES

#include <cmath>

#include "BigJack.h"

#include "IDirectX_Realize.h"
#include "HiTimer.h"
#include "Logger.h"
#include "IBaseObjectGE.h"
#include "BL_Debug.h"
#include "SDKmisc.h"
#include "IGUI_Core.h"
#include "GUI_DescWindow.h"
#include "MyGUI.h"
#include "DescEvent.h"
#include "MakerDirectX_Realize.h"
#include "ICamera.h"
#include <xstring>
#include "StorePathResources.h"
#include "MapXML_Field.h"


#define LOG_DX
//#define LOG_DX_STREAM

#define USE_ICAMERA

using namespace std;
using namespace nsStruct3D;
using namespace nsEvent;

#pragma warning(disable: 4355)

TBigJack::TBigJack(ICamera* pCamera):IGraphicEngine(pCamera)
{
  mICamera->UpdateForRender();

#ifdef WIN32
  TMakerDirectX_Realize makerDXUT;
  mDXUT = makerDXUT.New(this);
#else
#endif
  
  SetNeedResizeGUI(true);

  mTime_ms = 0;

#ifdef WIN32
  mManagerModelDX.SetManagerResourceDX(&mManagerResourceDX);
#ifndef USE_ICAMERA
  mCamera.SetEnablePositionMovement(true);
  mCamera.SetEnableYAxisMovement(false);

  mIndexView           = 
    mMainShaderStack.Push("View",(void*)mCamera.GetViewMatrix(),     sizeof(D3DXMATRIX));
  mIndexProj           = 
    mMainShaderStack.Push("Proj",(void*)mCamera.GetProjMatrix(),     sizeof(D3DXMATRIX));
  mIndexCameraPosition = 
    mMainShaderStack.Push("CameraPosition",(void*)mCamera.GetEyePt(),sizeof(D3DXVECTOR3));
#else
  mIndexView           = 
    mMainShaderStack.Push("View",(void*)mICamera->GetView(),     sizeof(TMatrix16));
  mIndexProj           = 
    mMainShaderStack.Push("Proj",(void*)mICamera->GetProj(),     sizeof(TMatrix16));
  mIndexCameraPosition = 
    mMainShaderStack.Push("CameraPosition",(void*)mICamera->GetEyePt(),sizeof(TVector3));
#endif

#else
#endif
}
//--------------------------------------------------------------------------------------------------------
TBigJack::~TBigJack()
{
  Done();
#ifdef WIN32
  TMakerDirectX_Realize makerDXUT;
  makerDXUT.Delete(mDXUT);
  mDXUT = NULL;
#else
#endif
}
//--------------------------------------------------------------------------------------------------------
void TBigJack::Optimize()
{
  // собственно сам процесс оптимизации (подсказка: процесс оптимизации заключается в выкидывании лишних
  // не рисуемых и не задействованных объектов, используются BB моделей объектов)
  /*...*/

}
//--------------------------------------------------------------------------------------------------------
void TBigJack::Render(IDirect3DDevice9* pd3dDevice)
{
#ifdef WIN32
  if(IsNeedResizeGUI())// когда произошло событие Reset, только после этого можно делать Resize (не было подстройки размера окна, сырость DXUT)
  {
    ResizeGUI();
    SetNeedResizeGUI(false);
  }
  //-----------------------------------------------------------------------
  mICamera->UpdateForRender();
  HRESULT hr;
  // Clear the render target and the zbuffer 
  V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 141, 153, 191 ), 1.0f, 0 ) );

  // Render the scene
  if( SUCCEEDED( pd3dDevice->BeginScene() ) )
  {
    TMatrix16 mView;
#ifndef USE_ICAMERA
    D3DXMATRIXA16* mDXView = (D3DXMATRIXA16*)mCamera.GetViewMatrix();// только для совместимости с DirectX (временно)
    MATRIX16_EQUAL_M_P(mView,mDXView)
#else
    mView = *mICamera->GetView();// только для совместимости с DirectX (временно)
#endif

    SetCommonShaderStack();// передать общие параметры в шейдер

    std::list<IBaseObjectGE*>::iterator it = mListReadyRender.begin();
    std::list<IBaseObjectGE*>::iterator eit = mListReadyRender.end();
    while(it!=eit)
    {
      (*it)->Draw(&mView);
      it++;
    }

    DispFPS();

    SaveRenderState();
      RenderGUI();
    RestoreRenderState();

    V( pd3dDevice->EndScene() );
  }
#else
#endif
}
//--------------------------------------------------------------------------------------------------------
void TBigJack::AddObject(IBaseObjectGE* pObject)
{
  TModelDX* pModel = mManagerModelDX.Find(pObject->GetID_Model());
  if(pModel)
  {
    if(pObject->GetModel()==NULL)
    {
      pObject->SetModel((IModelGE*)pModel);
      mListAllObject.push_back(pObject);
    }
    else
    {
      GetLogger()->Get("GE")->WriteF_time("Повторная инициализация объекта сцены, PTR_OBJECT=0x%X\n",pObject);
      BL_FIX_BUG();
    }
  }
}
//--------------------------------------------------------------------------------------------------------
void TBigJack::Clear()
{
  mListAllObject.clear();
}
//--------------------------------------------------------------------------------------------------------
void TBigJack::SetEffect(unsigned short id_effect/*уникальный эффект, см. таблицу эффектов*/,
                       TVector3* coord3,     // где
                       TVector3* orient3,    // ориентация эффекта
                       guint32 time_past/* прошло времени, мс*/)
{
/*
  IBaseObjectGE* pObjectDX = MakerEffect.New(id_effect);
  pObjectDX->SetTimeCreation(mTime_ms - time_past);
  pObjectDX->SetCoord(coord);
  pObjectDX->SetOrient(orient);
  mListAnimateObject.push_back(pObjectDX);
*/
}
//--------------------------------------------------------------------------------------------------------
bool TBigJack::IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat,
                               D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
  // No fallback defined by this app, so reject any device that 
  // doesn't support at least ps2.0
  if( pCaps->PixelShaderVersion < D3DPS_VERSION( 2, 0 ) )
    return false;

  // Skip backbuffer formats that don't support alpha blending
  IDirect3D9* pD3D = DXUTGetD3D9Object();
  if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
    AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
    D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
    return false;

  return true;
}
//--------------------------------------------------------------------------------------
bool TBigJack::ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
  assert( DXUT_D3D9_DEVICE == pDeviceSettings->ver );

  HRESULT hr;
  IDirect3D9* pD3D = DXUTGetD3D9Object();
  D3DCAPS9 caps;

  V( pD3D->GetDeviceCaps( pDeviceSettings->d3d9.AdapterOrdinal,
    pDeviceSettings->d3d9.DeviceType,
    &caps ) );

  // If device doesn't support HW T&L or doesn't support 1.1 vertex shaders in HW 
  // then switch to SWVP.
  if( ( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT ) == 0 ||
    caps.VertexShaderVersion < D3DVS_VERSION( 1, 1 ) )
  {
    pDeviceSettings->d3d9.BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
  }

  // Enable anti-aliasing for HAL devices which support it
  CD3D9Enumeration* pEnum = DXUTGetD3D9Enumeration();
  CD3D9EnumDeviceSettingsCombo* pCombo = pEnum->GetDeviceSettingsCombo( &pDeviceSettings->d3d9 );

  if( pDeviceSettings->d3d9.DeviceType == D3DDEVTYPE_HAL &&
    pCombo->multiSampleTypeList.Contains( D3DMULTISAMPLE_4_SAMPLES ) )
  {
    pDeviceSettings->d3d9.pp.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
    pDeviceSettings->d3d9.pp.MultiSampleQuality = 0;
  }
  // For the first device created if its a REF device, optionally display a warning dialog box
  static bool s_bFirstTime = true;
  if( s_bFirstTime )
  {
    s_bFirstTime = false;
    if( pDeviceSettings->d3d9.DeviceType == D3DDEVTYPE_REF )
      DXUTDisplaySwitchingToREFWarning( pDeviceSettings->ver );
  }
  return true;
}
//--------------------------------------------------------------------------------------
HRESULT TBigJack::OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                              void* pUserContext )
{
  //HRESULT hr;
  //V_RETURN(mDialogResourceManager.OnD3D9CreateDevice( pd3dDevice ))

  mManagerModelDX.Setup(pd3dDevice);
  mManagerResourceDX.Setup(pd3dDevice);
  mViewerFPS.CreateDevice(pd3dDevice);
#ifndef USE_ICAMERA
  // Setup the camera's view parameters
  D3DXVECTOR3 vecEye( 0.0f, -10.0f, 0.0001f );
  D3DXVECTOR3 vecAt ( 0.0f, 0.0f, 0.0f );

  mCamera.SetViewParams( &vecEye, &vecAt );
  // повернуть на 180 градусов
  const D3DXMATRIX* mView = mCamera.GetViewMatrix();
  D3DXMATRIX matrix;
  D3DXMATRIX newView;
  D3DXMatrixIdentity(&matrix);
  D3DXMatrixRotationZ(&matrix, float(M_PI));
  mCamera.SetViewMatrix(&newView);
#else
  TVector3 eye(0,0,0);
  float angleDown  = 0;//float(M_PI_2);
  float angleRight = 0;
  float angleRoll  = 0;//M_PI_2;
  mICamera->SetPosition(&eye);
  mICamera->RotateDown(angleDown);
  mICamera->RotateRight(angleRight);
  mICamera->Roll(angleRoll);
  mICamera->MoveForward(-30.0f);
  //mICamera->MoveUp(30.0f);
#endif
  return S_OK;
}
//--------------------------------------------------------------------------------------
HRESULT TBigJack::OnResetDevice( IDirect3DDevice9* pd3dDevice,
                             const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
  //HRESULT hr;
  //V_RETURN( mDialogResourceManager.OnD3D9ResetDevice() );

  // для прозрачности текстур
  pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
  pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
  pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
  pd3dDevice->SetRenderState( D3DRS_LIGHTING,  true);
  //---------------------------------------------------------------
  mManagerModelDX.ResetDevice();
  mManagerResourceDX.Reset();
  mViewerFPS.Reset();
  // Setup the camera's projection parameters
  float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;

#ifndef USE_ICAMERA
  mCamera.SetProjParams( D3DX_PI / 4, fAspectRatio, 0.1f, 1000.0f );
  mCamera.SetWindow( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );
#else
  mICamera->SetProjParams(D3DX_PI / 4, fAspectRatio, 0.1f, 1000.0f);
#endif

  SetNeedResizeGUI(true);
  return S_OK;
}
//--------------------------------------------------------------------------------------
void TBigJack::OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
#ifndef USE_ICAMERA
  // Update the camera's position based on user input 
  mCamera.FrameMove( fElapsedTime );
#endif
}
//--------------------------------------------------------------------------------------
void TBigJack::OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
  Animate();
  MakeVectorOnRender();// создать список на оптимизацию, прозрачные объекты идут самыми последними
  Optimize();
  Render(pd3dDevice);
}
//--------------------------------------------------------------------------------------
LRESULT TBigJack::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
                       void* pUserContext )
{
  //*pbNoFurtherProcessing = mDialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
  //if( *pbNoFurtherProcessing )
    //return 0;
#ifndef USE_ICAMERA
  mCamera.HandleMessages( hWnd, uMsg, wParam, lParam );
#endif
  return 0;
}
//--------------------------------------------------------------------------------------
void TBigJack::OnLostDevice( void* pUserContext )
{
  mManagerModelDX.LostDevice();
  mManagerResourceDX.Lost();
  mViewerFPS.Lost();

  LostDeviceGUI();
}
//--------------------------------------------------------------------------------------
void TBigJack::OnDestroyDevice( void* pUserContext )
{
  mManagerModelDX.DestroyModel();
  mManagerResourceDX.Destroy();
  mViewerFPS.Destroy();
}
//--------------------------------------------------------------------------------------
void TBigJack::Init()
{
  HRESULT hr = mDXUT->Init();
  if(hr!=S_OK)
  {
    GetLogger()->Get("GE")->WriteF_time("Init fail. hr=0x%X\n",hr);
  }
  SetIsCreateWindow(true);
  if(mGUI)
    InitGUI();
}
//--------------------------------------------------------------------------------------
void TBigJack::Done()
{
  mDXUT->Done();
  
  mManagerModelDX.DestroyModel();
  SetIsCreateWindow(false);
}
//--------------------------------------------------------------------------------------
void TBigJack::Work(guint32 time_ms)
{
  mTime_ms = time_ms;
  mDXUT->Work();
}
//--------------------------------------------------------------------------------------
void TBigJack::Animate()
{
  list<IBaseObjectGE*>::iterator bit = mListAnimateObject.begin();
  list<IBaseObjectGE*>::iterator eit = mListAnimateObject.end();
  while(bit!=eit)
  {
    if((*bit)->Animate(mTime_ms)==false)
    {
      // уничтожить эффект
      delete (*bit);
      list<IBaseObjectGE*>::iterator nit = bit;
      nit++;
      mListAnimateObject.erase(bit);
      bit = nit;
    }
    else
      bit++;
  }
  //---------------------------------------------------------------
  bit = mListAllObject.begin();
  eit = mListAllObject.end();
  while(bit!=eit)
  {
    bool res = (*bit)->Animate(mTime_ms);
    BL_ASSERT(res);
    bit++;
  }
}
//--------------------------------------------------------------------------------------
void TBigJack::SetCommonShaderStack()
{
#ifndef USE_ICAMERA
  mMainShaderStack.SetData(mIndexView,          (void*)mCamera.GetViewMatrix(), sizeof(D3DXMATRIX));
  mMainShaderStack.SetData(mIndexProj,          (void*)mCamera.GetProjMatrix(), sizeof(D3DXMATRIX));
  mMainShaderStack.SetData(mIndexCameraPosition,(void*)mCamera.GetEyePt(),      sizeof(D3DXVECTOR3));
#else
  mMainShaderStack.SetData(mIndexView,          (void*)mICamera->GetView(), sizeof(TMatrix16));
  mMainShaderStack.SetData(mIndexProj,          (void*)mICamera->GetProj(), sizeof(TMatrix16));
  mMainShaderStack.SetData(mIndexCameraPosition,(void*)mICamera->GetEyePt(),sizeof(TVector3));
#endif

  mManagerResourceDX.Set(TManagerResourceDX::eTypeShader,&mMainShaderStack);
}
//--------------------------------------------------------------------------------------
void TBigJack::SetViewFPS(bool val)
{
  if(val)
    mViewerFPS.Show();
  else
    mViewerFPS.Hide();
}
//--------------------------------------------------------------------------------------
void TBigJack::MakeVectorOnRender()
{
  mListReadyRender.clear();
  mListTransparencyObject.clear();
  //--------------------------------------------------------------
  list<IBaseObjectGE*>::iterator bit = mListAllObject.begin();
  list<IBaseObjectGE*>::iterator eit = mListAllObject.end();
  while(bit!=eit)
  {
    IBaseObjectGE* pObject = *(bit);
    if(pObject->GetAlphaTransparency()==1.0f)
      mListReadyRender.push_back(pObject);
    else
      mListTransparencyObject.push_back(pObject);

    bit++;
  }
  mListReadyRender.insert(mListReadyRender.end(),
                          mListTransparencyObject.begin(),
                          mListTransparencyObject.end());
}
//--------------------------------------------------------------------------------------
void TBigJack::GetResolutionFrame(int& h, int& w )// формат X8R8G8B8
{

}
//--------------------------------------------------------------------------------------
void TBigJack::SetResolutionFrame(int  h, int  w )// формат X8R8G8B8
{

}
//--------------------------------------------------------------------------------------
bool TBigJack::IsFullScreen()
{
  return mDXUT->IsFullScreen();
}
//--------------------------------------------------------------------------------------
void TBigJack::ToggleFullScreen()
{
  return mDXUT->ToggleFullScreen();
}
//--------------------------------------------------------------------------------------
bool TBigJack::HandleInternalEvent()
{
#ifdef WIN32
  HWND hWnd = mDXUT->GetHWND();

  bool bGotMsg;
  MSG msg;
  msg.message = WM_NULL;
  PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );

  while( WM_QUIT != msg.message )
  {
    bGotMsg = ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) != 0 );

    if( bGotMsg )
    {
      if(0 == TranslateAccelerator( hWnd, NULL, &msg ) )
      {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
      }
    }
    else
      return true;
  }
#endif
  return false;
}
//--------------------------------------------------------------------------------------
void TBigJack::OnKeyEvent( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
  // конвертируем в стандартный вид для клавиатуры
  TKeyEvent keyEvent;
  keyEvent.key      = mMapSysytemKeys.Get(nChar);
  keyEvent.modifier = bAltDown ? eAlt : eKeyEmpty;
  keyEvent.pressed  = bKeyDown;
  AddEvent(&keyEvent,sizeof(TKeyEvent));

  //GetLogger()->Get("GE")->WriteF("%d,%d\n",nChar,keyEvent.key);
}
//--------------------------------------------------------------------------------------
void TBigJack::OnMouseEvent( int state, int nMouseWheelDelta, 
                             int xPos, int yPos, void* pUserContext )
{
  // конвертируем в стандартный вид для мыши
  TMouseEvent mouseEvent;
#ifdef WIN32
  switch(state)
  {
    case WM_XBUTTONDOWN:
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
      mouseEvent.state = eButtonDown;
      break;
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
    case WM_XBUTTONUP:
      mouseEvent.state = eButtonUp;
      break;
    case WM_LBUTTONDBLCLK:
    case WM_MBUTTONDBLCLK:
    case WM_RBUTTONDBLCLK:
    case WM_XBUTTONDBLCLK:
      mouseEvent.state = eButtonDblClick;
      break;
    case WM_MOUSEWHEEL:
      mouseEvent.state = eWheel;
      break;
    case WM_MOUSEMOVE:
      mouseEvent.state = eMove;
      break;
    default:;
  }

  if( mouseEvent.state == eButtonDown || 
      mouseEvent.state == eButtonUp   )
  {
    if(state==WM_LBUTTONDOWN||state==WM_LBUTTONUP||state==WM_LBUTTONDBLCLK)
      mouseEvent.button = eLClick;
    else if(state==WM_MBUTTONDOWN||state==WM_MBUTTONUP||state==WM_MBUTTONDBLCLK)
      mouseEvent.button = eMClick;
    else if(state==WM_RBUTTONDOWN||state==WM_RBUTTONUP||state==WM_RBUTTONDBLCLK)
      mouseEvent.button = eRClick;
  }
  //------------------------------
  mouseEvent.delta_wheel = nMouseWheelDelta;
  mouseEvent.x = xPos;
  mouseEvent.y = yPos;
#endif
  AddEvent(&mouseEvent,sizeof(TMouseEvent));
}
//--------------------------------------------------------------------------------------
void TBigJack::SetTitleWindow(const char* sTitle)
{
#ifdef WIN32
  mDXUT->SetTitleWindow(sTitle);
#else
#endif
}
//--------------------------------------------------------------------------------------
bool TBigJack::InitGUI()
{
#ifdef WIN32
  TGUIDescWindow descWindow;
  descWindow.hwnd       = mDXUT->GetHWND();
  descWindow.pD3D       = mDXUT->GetD3D9Object();
  descWindow.pD3DDevice = mDXUT->GetD3D9Device(); 
  descWindow.pD3DPP     = mDXUT->GetDevicePresentParameters9();
  descWindow.windowed   = !IsFullScreen();
  mGUI->AddWindow(&descWindow);
#else
#endif
  bool res = false;
  TMapXML_Field* pMap = GetStorePathResources();
  int cntGUI = pMap->GetCount("GUI");
  for(int i = 0 ; i < cntGUI ; i++ )
  {
    mGUI->AddResourceLocation(pMap->GetSecond("GUI",i));
  }
  int cntSkin = pMap->GetCount("GUI_skin");
  for(int i = 0 ; i < cntSkin ; i++ )
  {
    res = 
      MyGUI::ResourceManager::getInstance().load(pMap->GetSecond("GUI_skin",i));// загрузка скина
    BL_ASSERT(res);
  }
  return res;
}
//--------------------------------------------------------------------------------------
void TBigJack::SaveRenderState()
{
  mDXUT->CaptureState9();  
}
//--------------------------------------------------------------------------------------
void TBigJack::RestoreRenderState()
{
  mDXUT->ApplyState9();
}
//--------------------------------------------------------------------------------------
void TBigJack::DispFPS()
{
  float FPS = mDXUT->GetFPS();
  wchar_t sFPS[100];
  swprintf_s(sFPS,L"FPS:%0.2f",FPS);
  mViewerFPS.SetText(wstring(sFPS));
  mViewerFPS.Render();
}
//--------------------------------------------------------------------------------------
