#include "ManagerDirectX.h"
#include "HiTimer.h"
#include "Bufferizator2Thread.h"
#include "ClientTank.h"
#include "LogerDX.h"
#include <d3d9.h>
#include "ManagerObjectDX.h"
#include "ManagerModel.h"
#include "DXUT.h"
#include "SDKmisc.h"
#include "ObjectDX.h"

#define LOG_DX
//#define LOG_DX_STREAM

using namespace nsStruct3D;

TManagerDirectX GlobalManagerDirectX;

TManagerDirectX::TManagerDirectX():
mLoaderMap(&GlobalManagerObjectDX,&GlobalManagerModel)
{
  mLastTimeSendMouseStream = 0;
  flgNeedLoadModel         = true;
}
//--------------------------------------------------------------------------------------------------------
TManagerDirectX::~TManagerDirectX()
{

}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::VisualEvent(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext)
{
  Calc();
  Optimize();
  Render(pd3dDevice, fTime, fElapsedTime, pUserContext);
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::Refresh()
{
  // только когда загрузитс€ карта и перед тем как отошлетс€ запрос на корректирующий пакет.
  if(flgNeedSendCorrectPacket)
  {
    flgNeedSendCorrectPacket = false;
    GlobalBufferizator2Thread.RegisterToClientTank();
    GlobalClientTank.SendRequestCorrectPacket();
#ifdef LOG_DX
    GlobalLoggerDX.WriteF_time("ќтослан корректирующий пакет.\n");
#endif
  }

  mSizeStream = eSizeBufferStream;
  mSizePacket = eSizeBufferPacket;
  
  // достает только 1 пакет за раз
  bool getPacket = GlobalBufferizator2Thread.GetPacket(mBufferPacket,mSizePacket,mTimePacket_ms);
  bool getStream = GlobalBufferizator2Thread.GetStream(mBufferStream,mSizeStream,mTimeStream_ms);
  
  // изменить состо€ние в соответствии со стримом от сервера
  if(getStream)
    SetStateByTypeStream();

#ifdef LOG_DX_STREAM
  if(getPacket)
    GlobalLoggerDX.WriteF_time("ѕолучен пакет.\n");
  if(getStream)
    GlobalLoggerDX.WriteF_time("ѕолучен стрим.\n");
  else
    GlobalLoggerDX.WriteF_time("-----------------------------------------------\n");
#endif
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::Calc()
{
  // заполнить предсказатель данными
  mPrediction.SetState();
  // расчет
  mPrediction.Calc();
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::Optimize()
{

}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::Render(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext)
{
  HRESULT hr;
  D3DXMATRIXA16 mView;
  D3DXMATRIXA16 mProj;

  // Clear the render target and the zbuffer 
  V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 45, 50, 170 ), 1.0f, 0 ) );

  mProj = *mCamera.GetProjMatrix();
  mView = *mCamera.GetViewMatrix();

  // Render the scene
  if( SUCCEEDED( pd3dDevice->BeginScene() ) )
  {
    std::list<TObjectDX*>::iterator it = mListReadyRender.begin();
    std::list<TObjectDX*>::iterator eit = mListReadyRender.end();
    while(it!=eit)
    {
      (*it)->Draw(&mView,&mProj);
      it++;
    }
    V( pd3dDevice->EndScene() );
  }
}
//--------------------------------------------------------------------------------------------------------
void* ThreadLoadMap(void* p)
{
  ((TManagerDirectX*)(p))->ThreadLoadMap();
  return NULL;
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::ThreadLoadMap()
{
  // дождатьс€ загрузки моделей
  while(flgNeedLoadModel)
  {
    ht_msleep(eWaitLoadModel);
  }

  mLoaderMap.Load(mPacket.getCodeMap());
  flgLoadingMap = false;
  flgNeedSendCorrectPacket = true;
#ifdef LOG_DX
  GlobalLoggerDX.WriteF_time("«агрузка карты завершена.\n");
#endif
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::LoadMap(TA_In_Fight& packet)
{
  mState = eLoadMap;
#ifdef LOG_DX
  GlobalLoggerDX.WriteF_time("«агрузка карты началась.\n");
#endif

  flgLoadingMap = true;
  mPacket.setData(packet.getData(),packet.getSize());
  threadLoadMap = g_thread_create(::ThreadLoadMap, (gpointer)this, true, NULL);
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::KeyEvent(unsigned int nChar, bool bKeyDown, bool bAltDown, void* pUserContext)
{
  if(mState==eLoadMap) return;

  switch(nChar)
  {
    case 0x1B://VK_ESCAPE:
      GlobalClientTank.SendRequestExitFromFight();
      break;
    default:;
  }
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::MouseEvent(double fTime, float fElapsedTime, void* pUserContext)
{
  if(mState!=eFight) return;
  guint32 now_ms = ht_GetMSCount();
  if(now_ms>mLastTimeSendMouseStream+eIntervalMouseStream)
  {

    D3DXVECTOR3 view = *mCamera.GetLookAtPt();
    //view.y

    GlobalClientTank.SendOrientAim(0,0,1);//###
    mLastTimeSendMouseStream = now_ms;
  }
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::SetStateByTypeStream()
{
  if(mState!=eFight)
    if(flgLoadingMap==false)
    {
      unsigned short type = *((unsigned short*)mBufferStream);
      switch(type)
      {
        case APPL_TYPE_S_COUNT_DOWN:
          if(mState!=eCountDown)
          {
            mState = eCountDown;
#ifdef LOG_DX
            GlobalLoggerDX.WriteF_time("Ќачалс€ обратный отсчет.\n");
#endif
          }
          break;
        case APPL_TYPE_S_FIGHT_COORD_BULLET:
          if(mState!=eFight)
          {
            mState = eFight;
#ifdef LOG_DX
            GlobalLoggerDX.WriteF_time("Ќачалс€ бой.\n");
#endif
          }
          break;
        default:;
      }
    }
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::CreateDeviceEvent(IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                           void* pUserContext )
{
  HRESULT hr;

  // Read the D3DX effect file
  WCHAR str[MAX_PATH];
  DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;

#ifdef DEBUG_VS
  dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
  dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif
#ifdef D3DXFX_LARGEADDRESS_HANDLE
  dwShaderFlags |= D3DXFX_LARGEADDRESSAWARE;
#endif

#ifdef LOG_DX
  GlobalLoggerDX.WriteF_time("Ќачалась загрузка моделей.\n");
#endif

  // загрузка меша, текстур и шейдеров
  GlobalManagerModel.Load(pd3dDevice,pBackBufferSurfaceDesc,pUserContext);
  flgNeedLoadModel = false;

#ifdef LOG_DX
  GlobalLoggerDX.WriteF_time("«агрузка моделей завершена.\n");
#endif

  // Setup the camera's view parameters
  D3DXVECTOR3 vecEye( 0.0f, 0.0f, -5.0f );
  D3DXVECTOR3 vecAt ( 0.0f, 0.0f, -0.0f );

  GlobalManagerDirectX.getCamera()->SetViewParams( &vecEye, &vecAt );
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::ResetDevice(IDirect3DDevice9* pd3dDevice,
                                     const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
  HRESULT hr;
  GlobalManagerModel.ResetDevice();
  
  // Setup the camera's projection parameters
  float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;
  getCamera()->SetProjParams( D3DX_PI / 4, fAspectRatio, 0.1f, 1000.0f );
  getCamera()->SetWindow( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::OnLostDevice()
{
  GlobalManagerModel.OnLostDevice();
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::OnDestroyDevice()
{
  GlobalManagerModel.OnDestroyDevice();
}
//--------------------------------------------------------------------------------------------------------
