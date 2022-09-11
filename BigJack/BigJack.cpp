/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012, 2013
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
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 
#ifdef WIN32

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
#include "MakerCamera.h"
#include "file_operation.h"
#include <atlconv.h>
#include <set>

#define LOG_DX
//#define LOG_DX_STREAM

using namespace std;
using namespace nsStruct3D;
using namespace nsEvent;

#pragma warning(disable: 4355)

namespace nsBigJack
{
	const char* strView 					= "View";
	const char* strProj 					=	"Proj";
	const char* strCameraPosition = "CameraPosition";
	const char* strLightPosition  =	"LightPosition";
	const char* strLightColor     = "LightColor";

	DWORD ColorForClear = D3DCOLOR_ARGB( 0, 141, 153, 191 );
};

// This is the vertex format used with the quad during post-process.
struct PPVERT
{
	float x, y, z, rhw;
	float tu, tv;       // Texcoord for post-process source
	float tu2, tv2;     // Texcoord for the original scene

	const static D3DVERTEXELEMENT9 Decl[4];
};

// Vertex declaration for post-processing
const D3DVERTEXELEMENT9 PPVERT::Decl[4] =
{
	{ 0, 0,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0 },
	{ 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  0 },
	{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  1 },
	D3DDECL_END()
};
//---------------------------------------------------------------------------------------------------------

using namespace nsBigJack;

TBigJack::TBigJack(ICamera* pCamera):IGraphicEngine(pCamera)
{
  TMakerCamera maker;
  pCameraInnerNeed = maker.New();

	mSurfaceDepthStencilCube = NULL;

  SetLightEnable(0,true);
  InitLightArr();

  mICamera->Update();

  TMakerDirectX_Realize makerDXUT;
  mDXUT = makerDXUT.New(this);
  
  SetNeedResizeGUI(true);

  mTime_ms = 0;

  mManagerModelDX.SetManagerResourceDX(&mManagerResourceDX);

	mShHandleView           = 
    mMainShaderStack.Push("View",(void*)mICamera->GetView(),     sizeof(TMatrix16));
  mShHandleProj           = 
    mMainShaderStack.Push("Proj",(void*)mICamera->GetProj(),     sizeof(TMatrix16));
  mShHandleCameraPosition = 
    mMainShaderStack.Push("CameraPosition",(void*)mICamera->GetEyePt(),sizeof(TVector3));

	mShHandleLightPosition = 
		mMainShaderStack.Push("LightPosition",(void*)&mArrLightPosition[0],sizeof(TVector3)*eCountLight);
	mShHandleLightColor = 
  	mMainShaderStack.Push("LightColor",(void*)&mArrLightColor[0],sizeof(TVector3)*eCountLight);

	mVertDeclPP = NULL;

	flgViewPostEffectInMirror = false;
}
//--------------------------------------------------------------------------------------------------------
TBigJack::~TBigJack()
{
  Done();

	TMakerDirectX_Realize makerDXUT;
  makerDXUT.Delete(mDXUT);
  mDXUT = NULL;

	TMakerCamera maker;
  maker.Delete(pCameraInnerNeed);
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
	LPDIRECT3DSURFACE9 pRTOld = NULL;
	V( pd3dDevice->GetRenderTarget( 0, &pRTOld ) );// запомнить старое полотно
	//-----------------------------------------------------------------------------------------------
	IDirect3DSurface9* pSurf;
	V( mTextureMainRender->GetSurfaceLevel(0, &pSurf) );
	V( pd3dDevice->SetRenderTarget( 0, pSurf ) );
	SAFE_RELEASE(pSurf);
	//-----------------------------------------------------------------------------------------------
	ZBufferOn();
  // Clear the render target and the zbuffer 
  V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, ColorForClear, 1.0f, 0 ) );

  // Render the scene
  if( SUCCEEDED( pd3dDevice->BeginScene() ) )
  {
    SetCommonShaderStack(mICamera->GetView(), mICamera->GetProj(), mICamera->GetEyePt());// передать общие параметры в шейдер

    TListPtr::iterator bit = mListReadyRender.begin();
    TListPtr::iterator eit = mListReadyRender.end();
    while(bit!=eit)
    {
      RenderObject(*bit,mICamera->GetView());
      bit++;
    }
    V( pd3dDevice->EndScene() );
	}
	//ZBufferOff();
	//-----------------------------------------------------------------------------------------------
	// старое полотно
	V( pd3dDevice->SetRenderTarget( 0, pRTOld ) );
	SAFE_RELEASE( pRTOld );
	//-------------------------------------------------------
	// Clear the render target and the zbuffer 
	V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, ColorForClear, 1.0f, 0 ) );
	if( SUCCEEDED( pd3dDevice->BeginScene() ) )
	{
		BlendTextureFromPostEffect(mTextureMainRender,true);

		DispFPS();
		HandleGUI();

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
      PrepareForPostEffect(pObject);
    }
    else
    {
      GetLogger()->Get(STR_NAME_GE)->WriteF_time("Повторная инициализация объекта сцены, PTR_OBJECT=0x%X\n",pObject);
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
                       unsigned int time_past/* прошло времени, мс*/)
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
	HRESULT hr;
  mManagerModelDX.Setup(pd3dDevice);
  mManagerResourceDX.Setup(pd3dDevice);
  mViewerFPS.CreateDevice(pd3dDevice);

	TVector3 eye(0,0,0);
  float angleDown  = 0;
  float angleRight = 0;
  float angleRoll  = 0;
  mICamera->SetPosition(&eye);
  mICamera->RotateDown(angleDown);
  mICamera->RotateRight(angleRight);
  mICamera->Roll(angleRoll);
  mICamera->MoveForward(-30.0f);
#if 1 // задел на будущее
  InitEffectForGlow();
	// Create vertex declaration for post-process
	V( pd3dDevice->CreateVertexDeclaration( PPVERT::Decl, &mVertDeclPP ) );
#endif
  return S_OK;
}
//--------------------------------------------------------------------------------------
HRESULT TBigJack::OnResetDevice( IDirect3DDevice9* pd3dDevice,
                                 const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	HRESULT hr;
  // для прозрачности текстур
  pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
  pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
  pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
  pd3dDevice->SetRenderState( D3DRS_LIGHTING,  true);
  pd3dDevice->SetRenderState( D3DRS_ZENABLE,  true);
	pd3dDevice->SetRenderState( D3DRS_ZFUNC,    D3DCMP_LESSEQUAL );
  //---------------------------------------------------------------
  mManagerModelDX.ResetDevice();
  mManagerResourceDX.Reset();
  mViewerFPS.Reset();
  // Setup the camera's projection parameters
  float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;

  mICamera->SetProjParams(D3DX_PI / 4, fAspectRatio, 0.1f, 1000.0f);

  SetNeedResizeGUI(true);

  //--------------------------------------------------------------
	DXUTDeviceSettings d3dSettings = mDXUT->GetDeviceSettings();
  //--------------------------------------------------------------
  V_RETURN( pd3dDevice->CreateDepthStencilSurface( eEnvMapSizeCubemap,
		                                               eEnvMapSizeCubemap,
		                                               d3dSettings.d3d9.pp.AutoDepthStencilFormat,
		                                               D3DMULTISAMPLE_NONE,
		                                               0,
		                                               TRUE,
		                                               &mSurfaceDepthStencilCube,
		                                               NULL ) );
  //--------------------------------------------------------------
  ResetEventForSurfacePestEffect();
  //--------------------------------------------------------------
	// создание текстуры под главный рендер
	V( mDXUT->GetD3D9Device()->CreateTexture( pBackBufferSurfaceDesc->Width,
																						pBackBufferSurfaceDesc->Height,
																						1,
																						D3DUSAGE_RENDERTARGET,
																						pBackBufferSurfaceDesc->Format,
																						D3DPOOL_DEFAULT,
																						&mTextureMainRender,
																						NULL ) );
	//--------------------------------------------------------------
	V( mDXUT->GetD3D9Device()->CreateTexture( pBackBufferSurfaceDesc->Width,
																						pBackBufferSurfaceDesc->Height,
																						1,
																						D3DUSAGE_RENDERTARGET,
																						pBackBufferSurfaceDesc->Format,
																						D3DPOOL_DEFAULT,
																						&mTextureSecondRender,
																						NULL ) );
	//--------------------------------------------------------------
	TListPtrEffect::iterator bit = mListPtrEffect.begin();
	TListPtrEffect::iterator eit = mListPtrEffect.end();
	while(bit!=eit)
	{
		ResetShader((*bit)->GetEffect(),
								pBackBufferSurfaceDesc->Width,
								pBackBufferSurfaceDesc->Height);
		bit++;
	}
  return S_OK;
}
//--------------------------------------------------------------------------------------
void TBigJack::OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
}
//--------------------------------------------------------------------------------------
void TBigJack::OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
  Animate();
  //------------------
	mICamera->Update();
	//------------------
  MakeVectorOnRender();// создать список, прозрачные объекты идут самыми последними
  Optimize();

	SetParamShaderForCubeMap();
	//------------------------------------------------------------------------------------
  RenderCubeMap(pd3dDevice);//подготовить текстуры-поверхности отражения для объектов
	//------------------------------------------------------------------------------------
	SetParamShaderForMainRender();
  RenderBlendTexturePostEffect(mICamera->GetView(),mICamera->GetProj(),mICamera->GetEyePt());
  Render(pd3dDevice);
}
//--------------------------------------------------------------------------------------
LRESULT TBigJack::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext )
{
	return 0;
}
//--------------------------------------------------------------------------------------
void TBigJack::OnLostDevice( void* pUserContext )
{
  mManagerModelDX.LostDevice();
  mManagerResourceDX.Lost();
  mViewerFPS.Lost();

  LostDeviceGUI();

	SAFE_RELEASE( mSurfaceDepthStencilCube );
  LostEventForSurfacePestEffect();
	SAFE_RELEASE(mTextureMainRender);
	SAFE_RELEASE(mTextureSecondRender);
}
//--------------------------------------------------------------------------------------
void TBigJack::OnDestroyDevice( void* pUserContext )
{
  mManagerModelDX.DestroyModel();
  mManagerResourceDX.Destroy();
  mViewerFPS.Destroy();
#if 1 // задел на будущее
	DestroyEffectForGlow();
  SAFE_RELEASE( mVertDeclPP );
#endif
}
//--------------------------------------------------------------------------------------
void TBigJack::Init()
{
  HRESULT hr = mDXUT->Init();
  if(hr!=S_OK)
  {
    GetLogger()->Get(STR_NAME_GE)->WriteF_time("Init fail. hr=0x%X\n",hr);
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
void TBigJack::Work(unsigned int time_ms)
{
  mTime_ms = time_ms;
  mDXUT->Work();
}
//--------------------------------------------------------------------------------------
void TBigJack::Animate()
{
  TListPtr::iterator bit = mListAnimateObject.begin();
  TListPtr::iterator eit = mListAnimateObject.end();
  while(bit!=eit)
  {
    if((*bit)->Animate(mTime_ms)==false)
    {
      // уничтожить эффект
      delete (*bit);
      TListPtr::iterator nit = bit;
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
void TBigJack::SetCommonShaderStack(const TMatrix16* pView, const TMatrix16* pProj, const TVector3* pCameraPos)
{
  mMainShaderStack.SetData(mShHandleView,          (void*)pView, sizeof(TMatrix16));
  mMainShaderStack.SetData(mShHandleProj,          (void*)pProj, sizeof(TMatrix16));
  mMainShaderStack.SetData(mShHandleCameraPosition,(void*)pCameraPos,sizeof(TVector3));

	mMainShaderStack.SetData(mShHandleLightColor,   (void*)&mArrLightColor[0],   sizeof(TVector3)*eCountLight);
	mMainShaderStack.SetData(mShHandleLightPosition,(void*)&mArrLightPosition[0],sizeof(TVector3)*eCountLight);

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
  TListPtr::iterator bit = mListAllObject.begin();
  TListPtr::iterator eit = mListAllObject.end();
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
  AddEventCopy(&keyEvent,sizeof(TKeyEvent));

  //GetLogger()->Get("GE")->WriteF("%d,%d\n",nChar,keyEvent.key);
}
//--------------------------------------------------------------------------------------
void TBigJack::OnMouseEvent( int state, int nMouseWheelDelta, 
                             int xPos, int yPos, void* pUserContext )
{
  // конвертируем в стандартный вид для мыши
  TMouseEvent mouseEvent;
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

  AddEventCopy(&mouseEvent,sizeof(TMouseEvent));
}
//--------------------------------------------------------------------------------------
void TBigJack::SetTitleWindow(const char* sTitle)
{
  mDXUT->SetTitleWindow(sTitle);
}
//--------------------------------------------------------------------------------------
bool TBigJack::InitGUI()
{
  TGUIDescWindow descWindow;
  descWindow.hwnd       = mDXUT->GetHWND();
  descWindow.pD3D       = mDXUT->GetD3D9Object();
  descWindow.pD3DDevice = mDXUT->GetD3D9Device(); 
  descWindow.pD3DPP     = mDXUT->GetDevicePresentParameters9();
  descWindow.windowed   = !IsFullScreen();
  mGUI->AddWindow(&descWindow);

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
//--------------------------------------------------------------------------------------
void TBigJack::InitLightArr()
{
  // сформировать массив для освещения
  for(int i = 0 ; i < eCountLight ; i++)
  {
    if(mArrLight[i].active)
      mArrLightColor[i] = mArrLight[i].color;
    else
      mArrLightColor[i] = TVector3(0,0,0);
    mArrLightPosition[i] = mArrLight[i].position;
  }
}
//--------------------------------------------------------------------------------------
void TBigJack::SetLightEnable(int index, bool v)
{
  mArrLight[index].active = v;

  if(mArrLight[index].active)
    mArrLightColor[index] = mArrLight[index].color;
  else
    mArrLightColor[index] = TVector3(0,0,0);
}
//--------------------------------------------------------------------------------------
bool TBigJack::GetLightEnable(int index)
{
  return mArrLight[index].active;
}
//--------------------------------------------------------------------------------------
int TBigJack::GetCountAllLight()
{
	return eCountLight;
}
//--------------------------------------------------------------------------------------
const TVector3* TBigJack::GetLightPosition(int index)
{
  return &(mArrLight[index].position);
}
//--------------------------------------------------------------------------------------
const TVector3* TBigJack::GetLightColor(int index)
{
  return &(mArrLight[index].color);
}
//--------------------------------------------------------------------------------------
void TBigJack::SetLightColor(int index, TVector3* color)
{
  mArrLight[index].color = *color;

  if(mArrLight[index].active)
    mArrLightColor[index] = mArrLight[index].color;
}
//--------------------------------------------------------------------------------------
void TBigJack::SetLightPosition(int index, TVector3* m3)
{	
  mArrLight[index].position = *m3;

  mArrLightPosition[index] = mArrLight[index].position;
}
//--------------------------------------------------------------------------------------
void TBigJack::RenderCubeMap(IDirect3DDevice9* pd3dDevice)
{
	HRESULT hr;
	// The projection matrix has a FOV of 90 degrees and asp ratio of 1
	TMatrix16 mat_proj;
	TMatrix16 mat_view;
	TVector3  vec_camera_pos;

	SetMatrixPerspectiveFovLH( &mat_proj, D3DX_PI * 0.5f, 1.0f, 0.01f, 1000.0f );

	mat_view = *mICamera->GetView();

	// перечисляем все объекты и ищем те которым нужно заполнить cube map
	TListPtr::iterator bit = mListReadyRender.begin();
	TListPtr::iterator eit = mListReadyRender.end();
	while(bit!=eit)
	{
		const TMatrix16* pMatObj = NULL;
		IBaseObjectGE* pObj = *bit;
		int nPart = pObj->GetCountPartForCubeMap();
		if(nPart>0)
		{
			pMatObj = pObj->GetWorld();
			mat_view._41 = 0;// вращаться камера будет вокруг центра координат
			mat_view._42 = 0;// затем умножится на матрицу, которая рассчитана для одной из сторон Cube map
			mat_view._43 = 0;
			vec_camera_pos.x = pMatObj->_41;
			vec_camera_pos.y = pMatObj->_42;
			vec_camera_pos.z = pMatObj->_43;
		}
		for(int iPart = 0; iPart < nPart ; iPart++)// перечисление всех частей объекта, которые нуждаются в построении текстуры CubeMap
		{
			IDirect3DCubeTexture9* pCubeMap = *((IDirect3DCubeTexture9**)pObj->GetTextureForCubeMap(iPart));
			for( int nFace = 0; nFace < 6; nFace++ )// стороны куба
			{
				TMatrix16 view;
				GetMatrixForCubeMap(nFace, view);
				SetMatrixMultiply( &view, &mat_view, &view );

				pCameraInnerNeed->SetView(&view);// создать камеру на основе матрицы
				pCameraInnerNeed->SetPosition(&vec_camera_pos);// установить позицию
				pCameraInnerNeed->Update();// расчет
				view = *pCameraInnerNeed->GetView();

				if(GetPostEffectMirror())
					RenderBlendTexturePostEffect(&view,&mat_proj,&vec_camera_pos);
				// установка стенсила должна быть после сбора текстуры для пост-эффекта
				//------------------------------------------------------------------------------
				LPDIRECT3DSURFACE9 pRTOld = NULL;
				V( pd3dDevice->GetRenderTarget( 0, &pRTOld ) );// запомнить старое полотно

				IDirect3DSurface9* pSurf;
				V( mTextureMainRender->GetSurfaceLevel(0, &pSurf) );
				V( pd3dDevice->SetRenderTarget( 0, pSurf ) );
				SAFE_RELEASE(pSurf);
				//-----------------------------------------------------------------------------------------------
				ZBufferOn();
				//-----------------------------------------------------------------------------------------------
				V( pd3dDevice->Clear( 0L, NULL, D3DCLEAR_ZBUFFER|D3DCLEAR_TARGET,ColorForClear, 1.0f, 0L ) );
				if( SUCCEEDED( pd3dDevice->BeginScene() ) )
				{
					SetCommonShaderStack(&view,&mat_proj,&vec_camera_pos);// передать общие параметры в шейдер
					TListPtr::iterator bbit = mListReadyRender.begin();
					TListPtr::iterator eeit = mListReadyRender.end();
					while(bbit!=eeit)
					{
						if(bit!=bbit)
							RenderObject(*bbit,&view);// отрисовка объектов сцены
						bbit++;
					}
					pd3dDevice->EndScene();
				}
				//---------------------------------------------------
				//ZBufferOff();
				//---------------------------------------------------
				V( mTextureSecondRender->GetSurfaceLevel(0, &pSurf) );
				V( pd3dDevice->SetRenderTarget( 0, pSurf ) );
				SAFE_RELEASE(pSurf);
				//-----------------------------------------------------------------------------------------------
				V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, ColorForClear, 1.0f, 0 ) );
				if( SUCCEEDED( pd3dDevice->BeginScene() ) )
				{
					BlendTextureFromPostEffect(mTextureMainRender,GetPostEffectMirror());
					V( pd3dDevice->EndScene() );
				}
				IDirect3DSurface9* pSurfDst = NULL;
				IDirect3DSurface9* pSurfSrc = NULL;
				V( mTextureSecondRender->GetSurfaceLevel(0, &pSurfSrc) );
				V( pCubeMap->GetCubeMapSurface( ( D3DCUBEMAP_FACES )nFace, 0, &pSurfDst ) );
				V( pd3dDevice->StretchRect(pSurfSrc,NULL,pSurfDst,NULL,D3DTEXF_NONE) );
				SAFE_RELEASE( pSurfSrc );
				SAFE_RELEASE( pSurfDst );
				//-----------------------------------------------------------------------------------------------
				V( pd3dDevice->SetRenderTarget( 0, pRTOld ) );
				SAFE_RELEASE( pRTOld );
			}
		}
		bit++;
	}
}
//--------------------------------------------------------------------------------------
void TBigJack::GetMatrixForCubeMap(int nFace, TMatrix16& view)
{
  TVector3 vEyePt = TVector3( 0.0f, 0.0f, 0.0f );
  TVector3 vLookDir;
  TVector3 vUpDir;
  TVector3 vRight;

  switch( nFace )
  {
    case D3DCUBEMAP_FACE_POSITIVE_X:
      vLookDir = TVector3( 1.0f, 0.0f, 0.0f );
      vUpDir   = TVector3( 0.0f, 1.0f, 0.0f );
      vRight   = TVector3( 0.0f, 0.0f, -1.0f);
      break;
    case D3DCUBEMAP_FACE_NEGATIVE_X:
      vLookDir = TVector3( -1.0f, 0.0f, 0.0f );
      vUpDir   = TVector3( 0.0f,  1.0f, 0.0f );
      vRight   = TVector3( 0.0f,  0.0f, 1.0f);
      break;
    case D3DCUBEMAP_FACE_POSITIVE_Y:
      vLookDir = TVector3( 0.0f, 1.0f, 0.0f );
      vUpDir   = TVector3( 0.0f, 0.0f, -1.0f );
      vRight   = TVector3( 1.0f, 0.0f, 0.0f);
      break;
    case D3DCUBEMAP_FACE_NEGATIVE_Y:
      vLookDir = TVector3( 0.0f, -1.0f, 0.0f );
      vUpDir   = TVector3( 0.0f, 0.0f,  1.0f );
      vRight   = TVector3( 1.0f, 0.0f,  0.0f);
      break;
    case D3DCUBEMAP_FACE_POSITIVE_Z:
      vLookDir = TVector3( 0.0f, 0.0f, 1.0f );
      vUpDir   = TVector3( 0.0f, 1.0f, 0.0f );
      vRight   = TVector3( 1.0f, 0.0f, 0.0f);
      break;
    case D3DCUBEMAP_FACE_NEGATIVE_Z:
      vLookDir = TVector3( 0.0f,  0.0f, -1.0f );
      vUpDir   = TVector3( 0.0f,  1.0f, 0.0f );
      vRight   = TVector3( -1.0f, 0.0f, 0.0f);
      break;
  }

  pCameraInnerNeed->SetPosition(&vEyePt);
  pCameraInnerNeed->SetDir(&vRight, &vUpDir, &vLookDir);
  pCameraInnerNeed->Update();
  view = *pCameraInnerNeed->GetView();
}
//--------------------------------------------------------------------------------------
void TBigJack::HandleGUI()
{
  if(IsNeedResizeGUI())// когда произошло событие Reset, только после этого можно делать Resize (не было подстройки размера окна, сырость DXUT)
  {
    ResizeGUI();
    SetNeedResizeGUI(false);
  }
  SaveRenderState();
  RenderGUI();
  RestoreRenderState();
}
//--------------------------------------------------------------------------------------
void TBigJack::RenderBlendTexturePostEffect(const TMatrix16* pView, 
                                            const TMatrix16* pProj,
                                            const TVector3* pPosCamera)// для данной точки обзора
{
	TListPtr::iterator bit = mListReadyRender.begin();
	TListPtr::iterator eit = mListReadyRender.end();
	// 1. Перебор все объектов - поиск тех кому нужен эффект
  while(bit!=eit)
  {
    IBaseObjectGE* pObject = *bit;
    if(pObject->IsShow())
	    if(pObject->IsGlowEffect())
		    RenderSrcTextureForPostEffect(pObject,pView,pProj,pPosCamera);
    bit++;
  }
  // 2. Процесс bluring-а (только для Glow)
	bit = mListReadyRender.begin();
	while(bit!=eit)
	{
		IBaseObjectGE* pObject = *bit;
		if(pObject->IsShow())
			if(pObject->IsGlowEffect())
				RenderGlow(pObject);
		bit++;
	}
}
//--------------------------------------------------------------------------------------
void TBigJack::RenderSrcTextureForPostEffect(IBaseObjectGE* pObject, 
                                             const TMatrix16* pView, 
                                             const TMatrix16* pProj,
                                             const TVector3* pPosCamera)
{
  // для оптимизации: рисовать только объекты, которые находятся между камерой и объектом pObj
  float distObjCamera = CalcDist(pObject->GetWorld(),pView);
  // сохранить предыдущее состояние полотна
  HRESULT hr;
  IDirect3DDevice9* pd3dDevice = mDXUT->GetD3D9Device();
  LPDIRECT3DSURFACE9 pRTOld = NULL;
  V( pd3dDevice->GetRenderTarget( 0, &pRTOld ) );// запомнить старое полотно
  //--------------------------------------------------------------------------------------
  // подготовить полотно - текстуру, связанную с объектом
  TMapPtrDescTPE::iterator fit = mMapObjTexturePostEffect.find(pObject);
  if(fit==mMapObjTexturePostEffect.end()) BL_FIX_BUG();
  IDirect3DTexture9* pTexture = fit->second.pSrc;
  IDirect3DSurface9* pSurface = NULL;
  pTexture->GetSurfaceLevel(0, &pSurface);
  V( pd3dDevice->SetRenderTarget( 0, pSurface ) );
  SAFE_RELEASE( pSurface );
  // черный фон
  V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 0, 0, 0 ), 1.0f, 0 ) );

  if(SUCCEEDED(pd3dDevice->BeginScene()))
  {
    SetCommonShaderStack(pView,pProj,pPosCamera);// передать общие параметры в шейдер

    TListPtr::iterator bit = mListReadyRender.begin();
    TListPtr::iterator eit = mListReadyRender.end();
    while(bit!=eit)
    {
      IBaseObjectGE* pIObj = *bit;
      float distIObjCamera = CalcDist(pIObj->GetWorld(),pView);
      //if(distIObjCamera<distObjCamera)//???
      if(pObject!=pIObj)
        pIObj->Draw(pView,&mEffectPreGlow_black);

      bit++;
    }
    pObject->Draw(pView);// что бы выделяться
    pd3dDevice->EndScene();
  }
  //--------------------------------------------------------------------------------------
  // вернуть полотно обратно
  V( pd3dDevice->SetRenderTarget( 0, pRTOld ) );
  SAFE_RELEASE( pRTOld );
}
//--------------------------------------------------------------------------------------
void TBigJack::BlendTextureFromPostEffect(IDirect3DTexture9* pTextureScreen, bool UsePostEffectTexture)
{
	// сцена используется всегда
	mEffectBlend.SetBool("use0",true);// сцена
	mEffectBlend.SetTexture("g_Texture0", pTextureScreen);//текстура сцены
	//-----------------------------------------------------------------
	// сбросить использование текстур
	mEffectBlend.SetBool("use1",false);
	mEffectBlend.SetBool("use2",false);
	mEffectBlend.SetBool("use3",false);
	mEffectBlend.SetBool("use4",false);
	int i = 1;
	bool isBlend = false;
	// смешать все текстуры
	// вывод результата на экран
	TMapPtrDescTPE::iterator bit = mMapObjTexturePostEffect.begin();
	TMapPtrDescTPE::iterator eit = mMapObjTexturePostEffect.end();
	while(bit!=eit)
	{
		if(bit->first->IsShow())
		if(bit->first->IsGlowEffect())
		{
			char s[50];
			sprintf(s,"use%d",i);
			mEffectBlend.SetBool(s,true);
			sprintf(s,"g_Texture%d",i);
			mEffectBlend.SetTexture(s, bit->second.pDst);  //текстура пост-эффекта
			isBlend = true;
			i++;
		}
		bit++;
	}
	// нет объектов с glow эффектом
	if(!UsePostEffectTexture||(isBlend==false))
	{
		// натянуть на экран все текстуры
		mEffectScene.SetTexture(pTextureScreen);//текстура сцены
		mEffectScene.SetInnerShaderParam();
		RenderByEffect(mEffectScene.GetEffect());
	}
	else
	{
		// натянуть на экран все текстуры
		mEffectBlend.SetInnerShaderParam();
		RenderByEffect(mEffectBlend.GetEffect());
	}
}
//--------------------------------------------------------------------------------------
void TBigJack::PrepareForPostEffect(IBaseObjectGE* pObject)
{
  if(pObject->IsGlowable())
  {
    const D3DSURFACE_DESC* pBackBufferSurfaceDesc = mDXUT->GetD3D9BackBufferSurfaceDesc();
    // создать 
    HRESULT hr;
		TDescTexturePostEffect desc;
    // src
		V( mDXUT->GetD3D9Device()->CreateTexture( pBackBufferSurfaceDesc->Width,
                                              pBackBufferSurfaceDesc->Height,
                                              1,
                                              D3DUSAGE_RENDERTARGET,
                                              pBackBufferSurfaceDesc->Format,
                                              D3DPOOL_DEFAULT,
                                              &desc.pSrc,
                                              NULL ) );
		// dst
		V( mDXUT->GetD3D9Device()->CreateTexture( pBackBufferSurfaceDesc->Width,
																							pBackBufferSurfaceDesc->Height,
																							1,
																							D3DUSAGE_RENDERTARGET,
																							pBackBufferSurfaceDesc->Format,
																							D3DPOOL_DEFAULT,
																							&desc.pDst,
																							NULL ) );

    TMapPtrDescTPE::value_type val(pObject, desc);
    mMapObjTexturePostEffect.insert(val);
  }
}
//--------------------------------------------------------------------------------------
void TBigJack::RenderObject(IBaseObjectGE* pObject, const TMatrix16* view)
{
  pObject->Draw(view);
}
//--------------------------------------------------------------------------------------
void TBigJack::ResetEventForSurfacePestEffect()
{
  const D3DSURFACE_DESC* pBackBufferSurfaceDesc = mDXUT->GetD3D9BackBufferSurfaceDesc();

  TMapPtrDescTPE::iterator bit = mMapObjTexturePostEffect.begin();
  TMapPtrDescTPE::iterator eit = mMapObjTexturePostEffect.end();
  while(bit!=eit)
  {
    HRESULT hr;
		TDescTexturePostEffect desc = bit->second;
		// src
    V( mDXUT->GetD3D9Device()->CreateTexture( pBackBufferSurfaceDesc->Width,
                                              pBackBufferSurfaceDesc->Height,
                                              1,
                                              D3DUSAGE_RENDERTARGET,
                                              pBackBufferSurfaceDesc->Format,
                                              D3DPOOL_DEFAULT,
                                              &desc.pSrc,
                                              NULL ) );
		// dst
		V( mDXUT->GetD3D9Device()->CreateTexture( pBackBufferSurfaceDesc->Width,
																							pBackBufferSurfaceDesc->Height,
																							1,
																							D3DUSAGE_RENDERTARGET,
																							pBackBufferSurfaceDesc->Format,
																							D3DPOOL_DEFAULT,
																							&desc.pDst,
																							NULL ) );
    bit->second = desc;
    bit++;
  }
}
//--------------------------------------------------------------------------------------
void TBigJack::LostEventForSurfacePestEffect()
{
  TMapPtrDescTPE::iterator bit = mMapObjTexturePostEffect.begin();
  TMapPtrDescTPE::iterator eit = mMapObjTexturePostEffect.end();
  while(bit!=eit)
  {
    TDescTexturePostEffect desc = bit->second;
    SAFE_RELEASE( desc.pSrc );
    SAFE_RELEASE( desc.pDst );
    bit->second = desc;
    bit++;
  }
}
//--------------------------------------------------------------------------------------
#define INIT_SHADER(name) \
strcpy(sPath,sAbsPath); \
strcat(sPath,"\\shader"#name".fx"); \
mEffect##name.SetEffect( (ID3DXEffect*)mManagerResourceDX.Load(A2W(sPath)) ); \
mEffect##name.Init(); \
mListPtrEffect.push_back(&mEffect##name);
//--------------------------------------------------------------------------------------
void TBigJack::InitEffectForGlow()
{
  USES_CONVERSION;
  // загрузка и настройка эффекта Glow
  char sAbsPath[300];
  FindAbsPath((char*)GetStorePathResources()->GetSecond("shader"),sAbsPath,sizeof(sAbsPath));
  // регистрация в ManagerEffect
  char sPath[300];
	// черный
	INIT_SHADER(PreGlow_black)
  // белый
	INIT_SHADER(PreGlow_white)
	// post
	INIT_SHADER(EdgeDetect)
	INIT_SHADER(DownFilter4x)
	INIT_SHADER(UpFilter4x)
	INIT_SHADER(BloomH)
	INIT_SHADER(BloomV)
	INIT_SHADER(Combine4x)
	// создание и вывод
	INIT_SHADER(Blend)
	INIT_SHADER(Scene)
}
//--------------------------------------------------------------------------------------
void TBigJack::RenderGlow(IBaseObjectGE* pObject)
{
	// находим текстуру
	TMapPtrDescTPE::iterator fit = mMapObjTexturePostEffect.find(pObject);
	BL_ASSERT(mMapObjTexturePostEffect.end()!=fit);
	TDescTexturePostEffect desc = fit->second;
	// сохранить цель рендера
	HRESULT hr;
	IDirect3DDevice9* pd3dDevice = mDXUT->GetD3D9Device();
	LPDIRECT3DSURFACE9 pRTOld = NULL;
	V( pd3dDevice->GetRenderTarget( 0, &pRTOld ) );// запомнить старое полотно
	//--------------------------------------------------------------------------------------
	IDirect3DTexture9* pTexture = NULL;
	IDirect3DSurface9* pSurface = NULL;
	//--------------------------------------------------------------------------------------
	pTexture = desc.pDst;
	pSurface = NULL;
	pTexture->GetSurfaceLevel(0, &pSurface);
	V( pd3dDevice->SetRenderTarget( 0, pSurface ) );
	SAFE_RELEASE( pSurface );
	// черный фон
	V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 0, 0, 0 ), 1.0f, 0 ) );

	if(SUCCEEDED(pd3dDevice->BeginScene()))
	{
		mEffectDownFilter4x.SetTexture(desc.pSrc);
		mEffectDownFilter4x.SetInnerShaderParam();
		RenderByEffect(mEffectDownFilter4x.GetEffect());

		pd3dDevice->EndScene();
	}
	swap(desc.pSrc,desc.pDst);
	//--------------------------------------------------------------------------------------
	pTexture = desc.pDst;
	pSurface = NULL;
	pTexture->GetSurfaceLevel(0, &pSurface);
	V( pd3dDevice->SetRenderTarget( 0, pSurface ) );
	SAFE_RELEASE( pSurface );
	// черный фон
	V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 0, 0, 0 ), 1.0f, 0 ) );

	if(SUCCEEDED(pd3dDevice->BeginScene()))
	{
		mEffectBloomH.SetTexture(desc.pSrc);
		mEffectBloomH.SetInnerShaderParam();
		RenderByEffect(mEffectBloomH.GetEffect());

		pd3dDevice->EndScene();
	}
	swap(desc.pSrc,desc.pDst);
	//--------------------------------------------------------------------------------------
	pTexture = desc.pDst;
	pSurface = NULL;
	pTexture->GetSurfaceLevel(0, &pSurface);
	V( pd3dDevice->SetRenderTarget( 0, pSurface ) );
	SAFE_RELEASE( pSurface );
	// черный фон
	V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 0, 0, 0 ), 1.0f, 0 ) );

	if(SUCCEEDED(pd3dDevice->BeginScene()))
	{
		mEffectBloomV.SetTexture(desc.pSrc);
		mEffectBloomV.SetInnerShaderParam();
		RenderByEffect(mEffectBloomV.GetEffect());

		pd3dDevice->EndScene();
	}
	//--------------------------------------------------------------------------------------
  fit->second = desc;
	//--------------------------------------------------------------------------------------
	// вернуть полотно обратно
	V( pd3dDevice->SetRenderTarget( 0, pRTOld ) );
	SAFE_RELEASE( pRTOld );
}
//--------------------------------------------------------------------------------------
void TBigJack::SaveTexture(const char* name, IDirect3DTexture9* pTexture)
{
	USES_CONVERSION;
	wchar_t sFile[300];
	wsprintf(sFile,L"D:\\%s.jpg",A2W(name));
	DXUTSnapD3D9Screenshot_Texture((LPCTSTR)sFile, pTexture);
}
//--------------------------------------------------------------------------------------
void TBigJack::SaveSurface(const char* name, IDirect3DSurface9* pSurfaсe)
{
	USES_CONVERSION;
	wchar_t sFile[300];
	wsprintf(sFile,L"D:\\%s.jpg",A2W(name));
	DXUTSnapD3D9Screenshot_Surface((LPCTSTR)sFile, pSurfaсe);
}
//--------------------------------------------------------------------------------------
void TBigJack::DestroyEffectForGlow()
{
	TListPtrEffect::iterator bit = mListPtrEffect.begin();
	TListPtrEffect::iterator eit = mListPtrEffect.end();
	while(bit!=eit)
	{
		(*bit)->Destroy();
		bit++;
	}
	mListPtrEffect.clear();
}
//-------------------------------------------------------------
void TBigJack::ResetShader( ID3DXEffect* m_pEffect, DWORD dwWidth, DWORD dwHeight )
{
	D3DXHANDLE hParamToConvert;
	D3DXHANDLE hAnnotation;
	UINT uParamIndex = 0;
	// If a top-level parameter has the "ConvertPixelsToTexels" annotation,
	// do the conversion.
	while( NULL != ( hParamToConvert = m_pEffect->GetParameter( NULL, uParamIndex++ ) ) )
	{
		if( NULL != ( hAnnotation = m_pEffect->GetAnnotationByName( hParamToConvert, "ConvertPixelsToTexels" ) ) )
		{
			LPCSTR szSource;
			m_pEffect->GetString( hAnnotation, &szSource );
			D3DXHANDLE hConvertSource = m_pEffect->GetParameterByName( NULL, szSource );

			if( hConvertSource )
			{
				// Kernel source exists. Proceed.
				// Retrieve the kernel size
				D3DXPARAMETER_DESC desc;
				m_pEffect->GetParameterDesc( hConvertSource, &desc );
				// Each element has 2 floats
				DWORD cKernel = desc.Bytes / ( 2 * sizeof( float ) );
				D3DXVECTOR4* pvKernel = new D3DXVECTOR4[cKernel];
				if( !pvKernel )
				{
					BL_FIX_BUG();
					return;
				}
				m_pEffect->GetVectorArray( hConvertSource, pvKernel, cKernel );
				// Convert
				for( DWORD i = 0; i < cKernel; ++i )
				{
					pvKernel[i].x = pvKernel[i].x / dwWidth;
					pvKernel[i].y = pvKernel[i].y / dwHeight;
				}
				// Copy back
				m_pEffect->SetVectorArray( hParamToConvert, pvKernel, cKernel );

				delete[] pvKernel;
			}
		}
	}
}
//---------------------------------------------------------------------------------------------------------
void TBigJack::RenderByEffect(ID3DXEffect* g_pEffect)
{
	HRESULT hr;
	const D3DSURFACE_DESC* pDescBB = mDXUT->GetD3D9BackBufferSurfaceDesc();
	DWORD Width =  pDescBB->Width;
	DWORD Height = pDescBB->Height;

	IDirect3DDevice9* pd3dDevice = mDXUT->GetD3D9Device();
	// Render a screen-sized quad
	PPVERT quad[4] =
	{
		{ -0.5f,        -0.5f,         0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
		{ Width - 0.5f, -0.5f,         0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f },
		{ -0.5f,        Height - 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f },
		{ Width - 0.5f, Height - 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f }
	};
	V( pd3dDevice->SetVertexDeclaration( mVertDeclPP ) );
	UINT cPasses,p;
	V( g_pEffect->Begin( &cPasses, 0 ) );
	for( p = 0; p < cPasses; ++p )
	{
		V( g_pEffect->BeginPass( p ) );
		V( pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, quad, sizeof( PPVERT ) ) );
		V( g_pEffect->EndPass() );
	}
	V( g_pEffect->End() );
}
//---------------------------------------------------------------------------------------------------------
void TBigJack::SetParamShaderForCubeMap()
{
	// достаточно для Horizontal, т.к. параметр шейдера shared
	mEffectBloomH.SetFloat("g_BloomScale",3.0f);//усиление света
	mEffectBloomH.SetFloat("g_BloomShiftScale",6.0f);// длина короны
}
//---------------------------------------------------------------------------------------------------------
void TBigJack::SetParamShaderForMainRender()
{
	// достаточно для Horizontal, т.к. параметр шейдера shared
	mEffectBloomH.SetFloat("g_BloomScale",1.4f);
	mEffectBloomH.SetFloat("g_BloomShiftScale",4.0f);
}
//---------------------------------------------------------------------------------------------------------
bool TBigJack::GetPostEffectMirror()
{
	return flgViewPostEffectInMirror;
}
//---------------------------------------------------------------------------------------------------------
void TBigJack::SetPostEffectMirror(bool v)
{
	flgViewPostEffectInMirror = v;
}
//---------------------------------------------------------------------------------------------------------
void TBigJack::ZBufferOn()
{
	mDXUT->GetD3D9Device()->SetRenderState(D3DRS_ZENABLE, true);
}
//---------------------------------------------------------------------------------------------------------
void TBigJack::ZBufferOff()
{
	mDXUT->GetD3D9Device()->SetRenderState(D3DRS_ZENABLE, false);
}
//---------------------------------------------------------------------------------------------------------

#endif