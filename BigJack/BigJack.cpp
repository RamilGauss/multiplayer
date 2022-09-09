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
#include "BaseObjectDX.h"
#include "BL_Debug.h"
#include "SDKmisc.h"


#define LOG_DX
//#define LOG_DX_STREAM

using namespace std;
using namespace nsStruct3D;

#pragma warning(disable: 4355)

TBigJack::TBigJack(): mDXUT(this)
{
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
  int size = mSetCallbackMsg.size();
  BL_ASSERT(size==0);
  mSetCallbackMsg.clear();
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
    D3DXMATRIXA16 mView = *mCamera.GetViewMatrix();
    SetCommonShaderStack();// передать общие параметры в шейдер

    std::list<TBaseObjectDX*>::iterator it = mListReadyRender.begin();
    std::list<TBaseObjectDX*>::iterator eit = mListReadyRender.end();
    while(it!=eit)
    {
      (*it)->Draw(&mView);
      it++;
    }

    mViewerFPS.Render(mDXUT.GetFPS());
    V( pd3dDevice->EndScene() );
  }
}
//--------------------------------------------------------------------------------------------------------
void TBigJack::AddObject(TBaseObjectDX* pObject)
{
  TModelDX* pModel = mManagerModelDX.Find(pObject->GetID_Model());
  if(pModel)
  {
    if(pObject->GetModel()==NULL)
    {
      pObject->SetModel(pModel);
      mListAllObject.push_back(pObject);
    }
    else
    {
      GlobalLoggerDX.WriteF_time("Повторная инициализация объекта сцены, PTR_OBJECT=0x%X\n",pObject);
      BL_FIX_BUG();
    }
  }
}
//--------------------------------------------------------------------------------------------------------
void TBigJack::SetViewParams(D3DXVECTOR3* pvEyePt, D3DXVECTOR3* pvLookatPt)// расположение камеры
{
  mCamera.SetViewParams(pvEyePt, pvLookatPt);
}
//--------------------------------------------------------------------------------------------------------
void TBigJack::Clear()
{
  mListAllObject.clear();
}
//--------------------------------------------------------------------------------------------------------
void TBigJack::SetEffect(unsigned short id_effect/*уникальный эффект, см. таблицу эффектов*/,
               D3DVECTOR& coord,     // где
               D3DVECTOR& orient,    // ориентация эффекта
               guint32 time_past/*// прошло времени, мс*/)
{
/*
  TBaseObjectDX* pObjectDX = MakerEffect.New(id_effect);
  pObjectDX->SetTimeCreation(mTime_ms - time_past);
  pObjectDX->SetCoord(coord);
  pObjectDX->SetOrient(orient);
  mListAnimateObject.push_back(pObjectDX);
*/
}
//--------------------------------------------------------------------------------------------------------
void TBigJack::NotifyFrameMove(double fTime, float fElapsedTime, void* pUserContext)
{
  set<TCallBackFrameMove>::iterator bit = mSetCallbackFrameMove.begin();
  set<TCallBackFrameMove>::iterator eit = mSetCallbackFrameMove.end();
  while(bit!=eit)
  {
    TCallBackFrameMove pFunc = (*bit);
    pFunc(fTime, fElapsedTime, pUserContext);
    bit++;
  }
}
//--------------------------------------------------------------------------------------
void TBigJack::NotifyMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  set<TCallBackMsg>::iterator bit = mSetCallbackMsg.begin();
  set<TCallBackMsg>::iterator eit = mSetCallbackMsg.end();
  while(bit!=eit)
  {
    TCallBackMsg pFunc = (*bit);
    pFunc(hWnd, uMsg, wParam, lParam);
    bit++;
  }
}
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
  return S_OK;
}
//--------------------------------------------------------------------------------------
void TBigJack::OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
  // Update the camera's position based on user input 
  mCamera.FrameMove( fElapsedTime );

  NotifyFrameMove(fTime, fElapsedTime, pUserContext);
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
  mCamera.HandleMessages( hWnd, uMsg, wParam, lParam );

  NotifyMsg(hWnd, uMsg, wParam, lParam);
  return 0;
}
//--------------------------------------------------------------------------------------
void TBigJack::OnLostDevice( void* pUserContext )
{
  mManagerModelDX.LostDevice();
  mManagerResourceDX.Lost();
  mViewerFPS.Lost();
}
//--------------------------------------------------------------------------------------
void TBigJack::OnDestroyDevice( void* pUserContext )
{
  mManagerModelDX.DestroyModel();
  mManagerResourceDX.Destroy();
  mViewerFPS.Destroy();
}
//--------------------------------------------------------------------------------------
void TBigJack::Init(HWND hwnd )
{
  HRESULT hr = mDXUT.Init(hwnd);
  if(hr!=S_OK)
  {
    GlobalLoggerDX.WriteF_time("Init fail. hr=0x%X\n",hr);
  }
}
//--------------------------------------------------------------------------------------
void TBigJack::Done()
{
  mDXUT.Done();
  
  mManagerModelDX.DestroyModel();
}
//--------------------------------------------------------------------------------------
void TBigJack::Work(guint32 time_ms)
{
  mTime_ms = time_ms;
  mDXUT.Work();
}
//--------------------------------------------------------------------------------------
// на получение событий WinApi окна и DirectX
void TBigJack::Register(void* pFunc, int type)
{
  switch(type)
  {
    case eTypeMsg:
      RegisterSet((std::set<void*>*)&mSetCallbackMsg,(void*)pFunc);
      break;
    case eTypeFrameMove:
      RegisterSet((std::set<void*>*)&mSetCallbackFrameMove,pFunc);
      break;
    default:;
  }
}
//--------------------------------------------------------------------------------------
void TBigJack::Unregister(void* pFunc, int type)
{
  switch(type)
  {
    case eTypeMsg:
      UnregisterSet((std::set<void*>*)&mSetCallbackMsg,pFunc);
      break;
    case eTypeFrameMove:
      UnregisterSet((std::set<void*>*)&mSetCallbackFrameMove,pFunc);
      break;
    default:;
  }
}
//--------------------------------------------------------------------------------------
void TBigJack::UnregisterSet(std::set<void*>* setCallback, void* pFunc)
{
  set<void*>::iterator fit = setCallback->find(pFunc);
  set<void*>::iterator eit = setCallback->end();
  if(fit!=eit)
    setCallback->erase(fit);
  else
    BL_FIX_BUG();
}
//--------------------------------------------------------------------------------------
void TBigJack::RegisterSet(std::set<void*>* setCallback, void* pFunc)
{
  setCallback->insert(pFunc);
}
//--------------------------------------------------------------------------------------
void TBigJack::Animate()
{
  list<TBaseObjectDX*>::iterator bit = mListAnimateObject.begin();
  list<TBaseObjectDX*>::iterator eit = mListAnimateObject.end();
  while(bit!=eit)
  {
    if((*bit)->Animate(mTime_ms)==false)
    {
      // уничтожить эффект
      delete (*bit);
      list<TBaseObjectDX*>::iterator nit = bit;
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
  //guint32 start = ht_GetUSCount();

  mMainShaderStack.SetData(mIndexView,          (void*)mCamera.GetViewMatrix(), sizeof(D3DXMATRIXA16));
  mMainShaderStack.SetData(mIndexProj,          (void*)mCamera.GetProjMatrix(), sizeof(D3DXMATRIXA16));
  mMainShaderStack.SetData(mIndexCameraPosition,(void*)mCamera.GetEyePt(),      sizeof(D3DXMATRIXA16));

  mManagerResourceDX.Set(TManagerResourceDX::eTypeShader,&mMainShaderStack);

  //start = ht_GetUSCount() - start;
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
  list<TBaseObjectDX*>::iterator bit = mListAllObject.begin();
  list<TBaseObjectDX*>::iterator eit = mListAllObject.end();
  while(bit!=eit)
  {
    TBaseObjectDX* pObject = *(bit);
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
