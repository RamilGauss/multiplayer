#include "ManagerDirectX.h"
#include "HiTimer.h"
#include "LoggerDX.h"
#include "ManagerObjectDX.h"
#include "ManagerModel.h"
#include "DXUT.h"
#include "ObjectDX.h"

#ifndef EDITOR_MODEL
  #include "ClientTank.h"
  #include "Bufferizator2Thread.h"
#endif

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
#ifndef EDITOR_MODEL
  // только когда загрузится карта и перед тем как отошлется запрос на корректирующий пакет.
  if(flgNeedSendCorrectPacket)
  {
    flgNeedSendCorrectPacket = false;
    GlobalBufferizator2Thread.RegisterToClientTank();
    GlobalClientTank.SendRequestCorrectPacket();
#ifdef LOG_DX
    GlobalLoggerDX.WriteF_time("Отослан корректирующий пакет.\n");
#endif
  }

  mSizeStream = eSizeBufferStream;
  mSizePacket = eSizeBufferPacket;
  
  // достает только 1 пакет за раз
  bool getPacket = GlobalBufferizator2Thread.GetPacket(mBufferPacket,mSizePacket,mTimePacket_ms);
  bool getStream = GlobalBufferizator2Thread.GetStream(mBufferStream,mSizeStream,mTimeStream_ms);
  
  // изменить состояние в соответствии со стримом от сервера
  if(getStream)
    SetStateByTypeStream();

#ifdef LOG_DX_STREAM
  if(getPacket)
    GlobalLoggerDX.WriteF_time("Получен пакет.\n");
  if(getStream)
    GlobalLoggerDX.WriteF_time("Получен стрим.\n");
  else
    GlobalLoggerDX.WriteF_time("-----------------------------------------------\n");
#endif
#endif
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::Calc()
{
  
#ifndef EDITOR_MODEL
  //------------------------------------------------------------------------
  // заполнить предсказатель данными
  mPrediction.SetState();
  // расчет
  mPrediction.Calc();
  //------------------------------------------------------------------------
#else
  TObjectDX* pObjectDX = GlobalManagerObjectDX.Get(0);
  if(false)//pObjectDX)
  {  
    nsStruct3D::TCoord3 coord;
    coord.y = 2*sin(float(ht_GetMSCount()/200.0f));
    coord.x = 2*cos(float(ht_GetMSCount()/200.0f));
    coord.z = 2*cos(float(ht_GetMSCount()/200.0f));
    pObjectDX->SetCoord(coord);// пускай объекты летают
  }
#endif
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::Optimize()
{
  while(mListReadyRender.size())
  {mListReadyRender.pop_back();}

  int cnt = GlobalManagerObjectDX.GetCnt();
  for(int i = 0; i < cnt ; i++)
  {
    TObjectDX* pObjectDX = GlobalManagerObjectDX.Get(i);
    if(pObjectDX)
    {  
      mListReadyRender.push_back(pObjectDX);
    }
  }
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
  // дождаться загрузки моделей
  while(flgNeedLoadModel)
  {
    ht_msleep(eWaitLoadModel);
  }

  mLoaderMap.LoadMap(mPacket.getCodeMap());
  flgLoadingMap = false;
  flgNeedSendCorrectPacket = true;
#ifdef LOG_DX
  GlobalLoggerDX.WriteF_time("Загрузка карты завершена.\n");
#endif
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::LoadMap(TA_In_Fight& packet)
{
  mState = eLoadMap;
#ifdef LOG_DX
  GlobalLoggerDX.WriteF_time("Загрузка карты началась.\n");
#endif

  flgLoadingMap = true;
  mPacket.setData(packet.getData(),packet.getSize());
  threadLoadMap = g_thread_create(::ThreadLoadMap, (gpointer)this, true, NULL);
}
//--------------------------------------------------------------------------------------------------------
#ifndef EDITOR_MODEL
void TManagerDirectX::KeyEvent(unsigned int nChar, bool bKeyDown, bool bAltDown, void* pUserContext)
{
  if(mState==eLoadMap) return;

  switch(nChar)
  {
    case 0x1B://VK_ESCAPE:

      GlobalClientTank.SendRequestExitFromFight();
      GlobalLoggerDX.WriteF_time("Escape.\n");
      break;
    default:;
  }
  GlobalLoggerDX.WriteF_time("Нажатие кнопки: %u.\n",nChar);
}
#else
void TManagerDirectX::KeyEvent(unsigned int nChar, bool bKeyDown, bool bAltDown, void* pUserContext)
{
  switch(nChar)
  {
    case 0x1B://VK_ESCAPE:
    {
      TLoaderMap loaderMap(&GlobalManagerObjectDX,&GlobalManagerModel);
      TLoaderMap::TDescObject oDesc;
      oDesc.id = 1;// cube_pro
      oDesc.state = 1;
      oDesc.coord.x = 0;
      oDesc.coord.y = 0;
      oDesc.coord.z = 0;
      oDesc.orient.vx = 0;
      oDesc.orient.vy = 0;
      oDesc.orient.vz = 0;
      loaderMap.LoadObjectDX(&oDesc,true);
      oDesc.id = 1;// cube_pro
      oDesc.state = 1;
      oDesc.coord.x = 0;
      oDesc.coord.y = 0;
      oDesc.coord.z = 0;
      oDesc.orient.vx = 0;
      oDesc.orient.vy = 0;
      oDesc.orient.vz = 0;
      loaderMap.LoadObjectDX(&oDesc,false);
      break;
    }
    default:;
  }
  GlobalLoggerDX.WriteF_time("Нажатие кнопки: %u.\n",nChar);
}
#endif
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::MouseEvent(double fTime, float fElapsedTime, void* pUserContext)
{
#ifndef EDITOR_MODEL
  if(mState!=eFight) return;
  guint32 now_ms = ht_GetMSCount();
  if(now_ms>mLastTimeSendMouseStream+eIntervalMouseStream)
  {

    D3DXVECTOR3 view = *mCamera.GetLookAtPt();
    //view.y

    GlobalClientTank.SendOrientAim(0,0,1);//###
    mLastTimeSendMouseStream = now_ms;
  }
#endif
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
            GlobalLoggerDX.WriteF_time("Начался обратный отсчет.\n");
#endif
          }
          break;
        case APPL_TYPE_S_FIGHT_COORD_BULLET:
          if(mState!=eFight)
          {
            mState = eFight;
#ifdef LOG_DX
            GlobalLoggerDX.WriteF_time("Начался бой.\n");
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
  GlobalLoggerDX.WriteF_time("Началась загрузка моделей.\n");
#endif

  // загрузка меша, текстур и шейдеров
  GlobalManagerModel.Load(pd3dDevice,pBackBufferSurfaceDesc,pUserContext);
  flgNeedLoadModel = false;

#ifdef LOG_DX
  GlobalLoggerDX.WriteF_time("Загрузка моделей завершена.\n");
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
