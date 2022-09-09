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
#define _USE_MATH_DEFINES

#include <cmath>

#include "DXUT.h"
#include "BigJack.h"
#include "HiTimer.h"
#include "Logger.h"
#include "IBaseObjectGE.h"
#include "BL_Debug.h"
#include "SDKmisc.h"
#include <DXGI.h>
#include "IGUI_Core.h"
#include "..\GUI\GUI_DescWindow.h"
#include "..\Common\Base\DirectX\BaseManager.h"


#define LOG_DX
//#define LOG_DX_STREAM

using namespace std;
using namespace nsStruct3D;

#pragma warning(disable: 4355)

TBigJack::TBigJack()
{
#ifdef WIN32
  mDXUT = new TDXUT(this);
#else
#endif

  mTime_ms = 0;

  mManagerModelDX.SetManagerResourceDX(&mManagerResourceDX);

  mIndexView           = 
    mMainShaderStack.Push("View",(void*)mCamera.GetViewMatrix(),     sizeof(D3DXMATRIXA16));
  mIndexProj           = 
    mMainShaderStack.Push("Proj",(void*)mCamera.GetProjMatrix(),     sizeof(D3DXMATRIXA16));
  mIndexCameraPosition = 
    mMainShaderStack.Push("CameraPosition",(void*)mCamera.GetEyePt(),sizeof(D3DXMATRIXA16));
}
//--------------------------------------------------------------------------------------------------------
TBigJack::~TBigJack()
{
  Done();
#ifdef WIN32
  delete mDXUT;
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
  HRESULT hr;
  // Clear the render target and the zbuffer 
  V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 141, 153, 191 ), 1.0f, 0 ) );

  // Render the scene
  if( SUCCEEDED( pd3dDevice->BeginScene() ) )
  {
    D3DXMATRIXA16 mDXView = *mCamera.GetViewMatrix();// только дял совместимости с DirectX (временно)
    TMatrix16 mView;
    MATRIX16_EQUAL_M_M(mView,mDXView)

    SetCommonShaderStack();// передать общие параметры в шейдер

    std::list<IBaseObjectGE*>::iterator it = mListReadyRender.begin();
    std::list<IBaseObjectGE*>::iterator eit = mListReadyRender.end();
    while(it!=eit)
    {
      (*it)->Draw(&mView);
      it++;
    }

    mViewerFPS.Render(mDXUT->GetFPS());

    RenderGUI();

    V( pd3dDevice->EndScene() );
  }
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
      GlobalLoggerGE.WriteF_time("Повторная инициализация объекта сцены, PTR_OBJECT=0x%X\n",pObject);
      BL_FIX_BUG();
    }
  }
}
//--------------------------------------------------------------------------------------------------------
void TBigJack::SetViewParams(nsStruct3D::TVector3* pvEyePt3, nsStruct3D::TVector3* pvLookatPt3)
{
  //mCamera.SetViewParams(pvEyePt, pvLookatPt);
}
//--------------------------------------------------------------------------------------------------------
void TBigJack::Clear()
{
  mListAllObject.clear();
}
//--------------------------------------------------------------------------------------------------------
void TBigJack::SetEffect(unsigned short id_effect/*уникальный эффект, см. таблицу эффектов*/,
                       nsStruct3D::TVector3* coord3,     // где
                       nsStruct3D::TVector3* orient3,    // ориентация эффекта
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
//void TBigJack::NotifyFrameMove(double fTime, float fElapsedTime, void* pUserContext)
//{
  //set<TCallBackFrameMove>::iterator bit = mSetCallbackFrameMove.begin();
  //set<TCallBackFrameMove>::iterator eit = mSetCallbackFrameMove.end();
  //while(bit!=eit)
  //{
  //  TCallBackFrameMove pFunc = (*bit);
  //  pFunc(fTime, fElapsedTime, pUserContext);
  //  bit++;
  //}
//}
//--------------------------------------------------------------------------------------
//void TBigJack::NotifyMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
  //set<TCallBackMsg>::iterator bit = mSetCallbackMsg.begin();
  //set<TCallBackMsg>::iterator eit = mSetCallbackMsg.end();
  //while(bit!=eit)
  //{
  //  TCallBackMsg pFunc = (*bit);
  //  pFunc(hWnd, uMsg, wParam, lParam);
  //  bit++;
  //}
//}
//--------------------------------------------------------------------------------------
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
  HRESULT hr;
  V_RETURN(mDialogResourceManager.OnD3D9CreateDevice( pd3dDevice ))

  mManagerModelDX.Setup(pd3dDevice);
  mManagerResourceDX.Setup(pd3dDevice);
  mViewerFPS.CreateDevice(pd3dDevice);
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

  return S_OK;
}
//--------------------------------------------------------------------------------------
HRESULT TBigJack::OnResetDevice( IDirect3DDevice9* pd3dDevice,
                             const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
  HRESULT hr;
  V_RETURN( mDialogResourceManager.OnD3D9ResetDevice() );

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
  mCamera.SetProjParams( D3DX_PI / 4, fAspectRatio, 0.1f, 1000.0f );

  mCamera.SetWindow( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );

  //ResetGUI(pBackBufferSurfaceDesc->Width,pBackBufferSurfaceDesc->Height);

  return S_OK;
}
//--------------------------------------------------------------------------------------
void TBigJack::OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
  // Update the camera's position based on user input 
  mCamera.FrameMove( fElapsedTime );
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
  *pbNoFurtherProcessing = mDialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
  if( *pbNoFurtherProcessing )
    return 0;

  //MsgProcGUI((unsigned int)hWnd, (unsigned int)uMsg, (unsigned int)wParam, (unsigned int)lParam);

  mCamera.HandleMessages( hWnd, uMsg, wParam, lParam );
  return 0;
}
//--------------------------------------------------------------------------------------
void TBigJack::OnLostDevice( void* pUserContext )
{
  mDialogResourceManager.OnD3D9LostDevice();
  mManagerModelDX.LostDevice();
  mManagerResourceDX.Lost();
  mViewerFPS.Lost();
}
//--------------------------------------------------------------------------------------
void TBigJack::OnDestroyDevice( void* pUserContext )
{
  mDialogResourceManager.OnD3D9DestroyDevice();
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
    GlobalLoggerGE.WriteF_time("Init fail. hr=0x%X\n",hr);
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
// на получение событий WinApi окна и DirectX
//void TBigJack::Register(void* pFunc, int type)
//{
//  switch(type)
//  {
//    case eTypeMsg:
//      RegisterSet((std::set<void*>*)&mSetCallbackMsg,(void*)pFunc);
//      break;
//    case eTypeFrameMove:
//      RegisterSet((std::set<void*>*)&mSetCallbackFrameMove,pFunc);
//      break;
//    default:;
//  }
//}
////--------------------------------------------------------------------------------------
//void TBigJack::Unregister(void* pFunc, int type)
//{
//  switch(type)
//  {
//    case eTypeMsg:
//      UnregisterSet((std::set<void*>*)&mSetCallbackMsg,pFunc);
//      break;
//    case eTypeFrameMove:
//      UnregisterSet((std::set<void*>*)&mSetCallbackFrameMove,pFunc);
//      break;
//    default:;
//  }
//}
//--------------------------------------------------------------------------------------
//void TBigJack::UnregisterSet(std::set<void*>* setCallback, void* pFunc)
//{
//  set<void*>::iterator fit = setCallback->find(pFunc);
//  set<void*>::iterator eit = setCallback->end();
//  if(fit!=eit)
//    setCallback->erase(fit);
//  else
//    BL_FIX_BUG();
//}
////--------------------------------------------------------------------------------------
//void TBigJack::RegisterSet(std::set<void*>* setCallback, void* pFunc)
//{
//  setCallback->insert(pFunc);
//}
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
  mMainShaderStack.SetData(mIndexView,          (void*)mCamera.GetViewMatrix(), sizeof(D3DXMATRIXA16));
  mMainShaderStack.SetData(mIndexProj,          (void*)mCamera.GetProjMatrix(), sizeof(D3DXMATRIXA16));
  mMainShaderStack.SetData(mIndexCameraPosition,(void*)mCamera.GetEyePt(),      sizeof(D3DXMATRIXA16));

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
//void* TBigJack::GetFuncEventGUI()
//{
//  void* funcGUI = NULL;
//#ifdef WIN32
//  funcGUI = mDXUT->GetFuncEventGUI();
//#else
//#endif
//  return funcGUI;
//}
//--------------------------------------------------------------------------------------
//void TBigJack::OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
//{
//  GUIEvent(nEvent, nControlID, pUserContext);
//}
//--------------------------------------------------------------------------------------
void TBigJack::OnKeyEvent( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
  // конвертируем в стандартный вид для клавиатуры
  TKeyEvent keyEvent;
  keyEvent.key      = nChar;
  keyEvent.modifier = bAltDown ? eAlt : eKeyEmpty;
  keyEvent.pressed  = bKeyDown;
  AddEvent(&keyEvent,sizeof(TKeyEvent));
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
//void* TBigJack::GetObjectForInitGUI()
//{
//  return 
//#ifdef WIN32
//  &mDialogResourceManager;
//#else
//#endif
//}
////--------------------------------------------------------------------------------------
//void TBigJack::ForceResizeEventGUI()
//{
//  int w,h;
//#ifdef WIN32
//  mDXUT->GetSizeWindow(w,h);
//#else
//#endif
//  ResetGUI(w,h);
//}
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

  //### Gauss
  mGUI->AddResourceLocation("/Demos/Demo_Themes");
  mGUI->AddResourceLocation("/Common/Demos");
  mGUI->AddResourceLocation("/Common/Themes");
  bool res = MyGUI::ResourceManager::getInstance().load("MyGUI_BlackBlueTheme.xml");// загрузка скина
  //### Gauss

  return res;
}
//--------------------------------------------------------------------------------------
