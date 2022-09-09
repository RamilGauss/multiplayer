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

#include "DXUT.h"
#include "ManagerDirectX.h"
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

TManagerDirectX::TManagerDirectX(): mDXUT(this)
{
  mTime_ms = 0.0f;
}
//--------------------------------------------------------------------------------------------------------
TManagerDirectX::~TManagerDirectX()
{
  int size = mSetCallbackMsg.size();
  BL_ASSERT(size==0);
  mSetCallbackMsg.clear();
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::Optimize()
{
  mListReadyRender = mListAllObject;

  // собственно сам процесс оптимизации (подсказка: процесс оптимизации заключается в выкидывании лишних
  // не рисуемых и не задействованных объектов, используются BB моделей объектов)
  /*...*/

}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::Render(IDirect3DDevice9* pd3dDevice)
{
  HRESULT hr;
  D3DXMATRIXA16 mView;
  D3DXMATRIXA16 mProj;

  // Clear the render target and the zbuffer 
  V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 141, 153, 191 ), 1.0f, 0 ) );

  // Render the scene
  if( SUCCEEDED( pd3dDevice->BeginScene() ) )
  {
    mProj = *mCamera.GetProjMatrix();
    mView = *mCamera.GetViewMatrix();

    std::list<TBaseObjectDX*>::iterator it = mListReadyRender.begin();
    std::list<TBaseObjectDX*>::iterator eit = mListReadyRender.end();
    while(it!=eit)
    {
      (*it)->Draw(&mView,&mProj,mCamera.GetEyePt());
      it++;
    }
    V( pd3dDevice->EndScene() );
  }
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::AddObject(TBaseObjectDX* pObject)
{
  TModelDX* pModel = mManagerModel.Find(pObject->GetID_Model());
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
void TManagerDirectX::SetViewParams(D3DXVECTOR3* pvEyePt, D3DXVECTOR3* pvLookatPt)// расположение камеры
{
  mCamera.SetViewParams(pvEyePt, pvLookatPt);
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::Clear()
{
  mListAllObject.clear();
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::SetEffect(unsigned short id_effect/*уникальный эффект, см. таблицу эффектов*/,
               D3DVECTOR& coord,     // где
               D3DVECTOR& orient,    // ориентация эффекта
               float time_past/*// прошло времени, мс*/)
{

}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::NotifyFrameMove(double fTime, float fElapsedTime, void* pUserContext)
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
void TManagerDirectX::NotifyMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
bool TManagerDirectX::IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat,
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
bool TManagerDirectX::ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
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
HRESULT TManagerDirectX::OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                              void* pUserContext )
{
  mManagerModel.Setup(pd3dDevice);
  // Setup the camera's view parameters
  D3DXVECTOR3 vecEye( 0.0f, -10.0f, 0.0001f );
  D3DXVECTOR3 vecAt ( 0.0f, 0.0f, 0.0f );
  //###
  //// Setup the camera's view parameters
  //D3DXVECTOR3 vecEye( 0.0f, 10.0f, 0.0001f );
  //D3DXVECTOR3 vecAt ( 0.0f, 0.0f, 0.0f );
  //D3DXVECTOR3 vUp( 0,1,0 );
  ////D3DXMatrixLookAtLH( &mView, &vecEye, &vecAt, &vUp );
  ////###
  //D3DXVECTOR3 dvec = vecAt - vecEye;
  //D3DXVECTOR3 zaxis; 
  //D3DXVec3Normalize(&zaxis,&dvec);
  //D3DXVECTOR3 xaxis; 
  //D3DXVec3Cross( &xaxis, &vUp, &zaxis);
  //D3DXVec3Normalize(&xaxis,&xaxis);

  //D3DXVECTOR3 yaxis; 
  //D3DXVec3Cross(&yaxis,&zaxis, &xaxis);// = cross(zaxis, xaxis);
  //mView = D3DXMATRIXA16(xaxis.x,                       yaxis.x,                      zaxis.x,                      0.0f,
  //  xaxis.y,                       yaxis.y,                      zaxis.y,                      0.0f,
  //  xaxis.z,                       yaxis.z,                      zaxis.z,                      0.0f,
  //  -D3DXVec3Dot(&xaxis, &vecEye), -D3DXVec3Dot(&yaxis,&vecEye), -D3DXVec3Dot(&zaxis,&vecEye), 1.0f);
  ////###

  mCamera.SetViewParams( &vecEye, &vecAt );
  return S_OK;
}
//--------------------------------------------------------------------------------------
HRESULT TManagerDirectX::OnResetDevice( IDirect3DDevice9* pd3dDevice,
                             const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
  mManagerModel.ResetDevice();
  // Setup the camera's projection parameters
  float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;
  mCamera.SetProjParams( D3DX_PI / 4, fAspectRatio, 0.1f, 1000.0f );

  mCamera.SetWindow( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );
  return S_OK;
}
//--------------------------------------------------------------------------------------
void TManagerDirectX::OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
  // Update the camera's position based on user input 
  mCamera.FrameMove( fElapsedTime );

  NotifyFrameMove(fTime, fElapsedTime, pUserContext);
}
//--------------------------------------------------------------------------------------
void TManagerDirectX::OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
  Optimize();
  Render(pd3dDevice);
}
//--------------------------------------------------------------------------------------
LRESULT TManagerDirectX::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
                       void* pUserContext )
{
  mCamera.HandleMessages( hWnd, uMsg, wParam, lParam );

  NotifyMsg(hWnd, uMsg, wParam, lParam);
  return 0;
}
//--------------------------------------------------------------------------------------
void TManagerDirectX::OnLostDevice( void* pUserContext )
{
  mManagerModel.LostDevice();
}
//--------------------------------------------------------------------------------------
void TManagerDirectX::OnDestroyDevice( void* pUserContext )
{
  mManagerModel.DestroyModel();
}
//--------------------------------------------------------------------------------------
void TManagerDirectX::Init(HWND hwnd )
{
  mDXUT.Init(hwnd);
}
//--------------------------------------------------------------------------------------
void TManagerDirectX::Done()
{
  mDXUT.Done();
  
  mManagerModel.DestroyModel();
}
//--------------------------------------------------------------------------------------
void TManagerDirectX::Work(float time_ms)
{
  mTime_ms = time_ms;
  mDXUT.Work();
}
//--------------------------------------------------------------------------------------
// на получение событий WinApi окна и DirectX
void TManagerDirectX::Register(void* pFunc, int type)
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
void TManagerDirectX::Unregister(void* pFunc, int type)
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
void TManagerDirectX::UnregisterSet(std::set<void*>* setCallback, void* pFunc)
{
  set<void*>::iterator fit = setCallback->find(pFunc);
  set<void*>::iterator eit = setCallback->end();
  if(fit!=eit)
    setCallback->erase(fit);
  else
    BL_FIX_BUG();
}
//--------------------------------------------------------------------------------------
void TManagerDirectX::RegisterSet(std::set<void*>* setCallback, void* pFunc)
{
  setCallback->insert(pFunc);
}
//--------------------------------------------------------------------------------------
