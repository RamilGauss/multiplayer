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


#include "ManagerDirectX.h"
#include "HiTimer.h"
#include "LoggerDX.h"
#include "BaseObjectDX.h"


#define LOG_DX
//#define LOG_DX_STREAM

using namespace nsStruct3D;

TManagerDirectX::TManagerDirectX()
{
  mD3DDevice = NULL;
}
//--------------------------------------------------------------------------------------------------------
TManagerDirectX::~TManagerDirectX()
{

}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::VisualEvent(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext)
{
  //Calc();
  //Optimize();
  //Render(pd3dDevice, fTime, fElapsedTime, pUserContext);
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::Refresh()
{
//#ifndef EDITOR_MODEL
//  // только когда загрузится карта и перед тем как отошлется запрос на корректирующий пакет.
//  if(flgNeedSendCorrectPacket)
//  {
//    flgNeedSendCorrectPacket = false;
//    mBufferizator2Thread.RegisterToClientTank();
//    //GlobalClientTank.SendRequestCorrectPacket();
//#ifdef LOG_DX
//    //GlobalLoggerDX.WriteF_time("Отослан корректирующий пакет.\n");
//#endif // LOG_DX
//  }
//
//  mSizeStream = eSizeBufferStream;
//  mSizePacket = eSizeBufferPacket;
//
//  // цикл обработки заданий для движка
//  bool getPacket = true;
//  while(getPacket)
//  {
//    getPacket = mBufferizator2Thread.GetPacket(mBufferPacket,mSizePacket,mTimePacket_ms);
//    if(getPacket)
//      ParserPacket();// обработка пакета
//  }
//
//  // достает только 1 пакет за раз
//  bool getStream = mBufferizator2Thread.GetStream(mBufferStream,mSizeStream,mTimeStream_ms);
//  
//  // изменить состояние в соответствии со стримом от сервера
//  if(getStream)
//    SetStateByTypeStream();
//
//#ifdef LOG_DX_STREAM
//  if(getPacket)
//    GlobalLoggerDX.WriteF_time("Получен пакет.\n");
//  if(getStream)
//    GlobalLoggerDX.WriteF_time("Получен стрим.\n");
//  else
//    GlobalLoggerDX.WriteF_time("-----------------------------------------------\n");
//#endif // LOG_DX_STREAM
//#endif
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::Calc()
{
//#ifndef EDITOR_MODEL
  //------------------------------------------------------------------------
  // заполнить предсказатель данными
  //if(pPrediction)
    //pPrediction->SetState();
  // расчет
  //if(pPrediction)
    //pPrediction->Calc();
  //------------------------------------------------------------------------
//#else
//  TObjectDX* pObjectDX = mManagerObjectDX.Get(0);
//  if(pObjectDX)
//  {  
//    nsStruct3D::TCoord3 coord;
//    coord.y = 2*sin(float(ht_GetMSCount()/200.0f));
//    coord.x = 2*cos(float(ht_GetMSCount()/200.0f));
//    coord.z = 2*cos(float(ht_GetMSCount()/200.0f));
//    pObjectDX->SetCoord(coord);// пускай объекты летают
//  }
//#endif
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::Optimize()
{
  //while(mListReadyRender.size())
  //{mListReadyRender.pop_back();}

  //int cnt = mManagerObjectDX.GetCnt();
  //for(int i = 0; i < cnt ; i++)
  //{
  //  TObjectDX* pObjectDX = mManagerObjectDX.Get(i);
  //  if(pObjectDX)
  //  {  
  //    mListReadyRender.push_back(pObjectDX);
  //  }
  //}
  // собственно сам процесс оптимизации (подсказка: процесс оптимизации заключается в выкидывании лишних
  // не рисуемых и не задействованных объектов)
  /*...*/

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
    std::list<TBaseObjectDX*>::iterator it = mListReadyRender.begin();
    std::list<TBaseObjectDX*>::iterator eit = mListReadyRender.end();
    while(it!=eit)
    {
      (*it)->Draw(&mView,&mProj);
      it++;
    }
    V( pd3dDevice->EndScene() );
  }
}
//--------------------------------------------------------------------------------------------------------
//void* ThreadLoadMap(void* p)
//{
//  ((TManagerDirectX*)(p))->ThreadLoadMap();
//  return NULL;
//}
//--------------------------------------------------------------------------------------------------------
//void TManagerDirectX::ThreadLoadMap()
//{
//  mLoaderMap.LoadMap(mPacket.getCodeMap());
//  flgLoadingMap = false;
//  flgNeedSendCorrectPacket = true;
//#ifdef LOG_DX
//  GlobalLoggerDX.WriteF_time("Загрузка карты завершена.\n");
//#endif
//}
//--------------------------------------------------------------------------------------------------------
//void TManagerDirectX::LoadMap(TA_In_Fight& packet)
//{
//  mState = eLoadMap;
//#ifdef LOG_DX
//  GlobalLoggerDX.WriteF_time("Загрузка карты началась.\n");
//#endif
//
//  flgLoadingMap = true;
//  mPacket.setData(packet.getData(),packet.getSize());
//  threadLoadMap = g_thread_create(::ThreadLoadMap, (gpointer)this, true, NULL);
//}
//--------------------------------------------------------------------------------------------------------
//void TManagerDirectX::KeyEvent(unsigned int nChar, bool bKeyDown, bool bAltDown, void* pUserContext)
//{
//#ifndef EDITOR_MODEL
//  if(mState==eLoadMap) return;
//#endif
//  if(mKeyHandler)
//    mKeyHandler->Work(nChar, bKeyDown, bAltDown, pUserContext);
//  GlobalLoggerDX.WriteF_time("Нажатие кнопки: %u.\n",nChar);
//}
//--------------------------------------------------------------------------------------------------------
//void TManagerDirectX::MouseEvent(double fTime, float fElapsedTime, void* pUserContext)
//{
//#ifndef EDITOR_MODEL
//  if(mState!=eFight) return;
//  guint32 now_ms = ht_GetMSCount();
//  if(now_ms>mLastTimeSendMouseStream+eIntervalMouseStream)
//  {
//
//    D3DXVECTOR3 view = *mCamera.GetLookAtPt();
//    //view.y
//    if(mMouseHandler)
//      mMouseHandler->Work(fTime, fElapsedTime, pUserContext);
//
//    mLastTimeSendMouseStream = now_ms;
//  }
//#endif
//}
//--------------------------------------------------------------------------------------------------------
//void TManagerDirectX::SetStateByTypeStream()
//{
//  if(mState!=eFight)
//    if(flgLoadingMap==false)
//    {
//      unsigned short type = *((unsigned short*)mBufferStream);
//      switch(type)
//      {
//        case APPL_TYPE_G_S_COUNT_DOWN:
//          if(mState!=eCountDown)
//          {
//            mState = eCountDown;
//#ifdef LOG_DX
//            GlobalLoggerDX.WriteF_time("Начался обратный отсчет.\n");
//#endif
//          }
//          break;
//        case APPL_TYPE_G_S_FIGHT_COORD_BULLET:
//          if(mState!=eFight)
//          {
//            mState = eFight;
//#ifdef LOG_DX
//            GlobalLoggerDX.WriteF_time("Начался бой.\n");
//#endif
//          }
//          break;
//        default:;
//      }
//    }
//}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::CreateDeviceEvent(IDirect3DDevice9* pd3dDevice)//, 
                                        //const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                        //void* pUserContext )
{
  //HRESULT hr;

  // Read the D3DX effect file
  //WCHAR str[MAX_PATH];
//  DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;
//
//#ifdef DEBUG_VS
//  dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
//#endif
//#ifdef DEBUG_PS
//  dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
//#endif
//#ifdef D3DXFX_LARGEADDRESS_HANDLE
//  dwShaderFlags |= D3DXFX_LARGEADDRESSAWARE;
//#endif

  mD3DDevice = pd3dDevice;

//#ifdef LOG_DX
  //GlobalLoggerDX.WriteF_time("Началась загрузка моделей.\n");
//#endif

  // загрузка меша, текстур и шейдеров
  //mManagerModel.Load(pd3dDevice,pBackBufferSurfaceDesc,pUserContext);

//#ifdef LOG_DX
  //GlobalLoggerDX.WriteF_time("Загрузка моделей завершена.\n");
//#endif

  // Setup the camera's view parameters
  //D3DXVECTOR3 vecEye( 0.0f, 0.0f, -5.0f );
  //D3DXVECTOR3 vecAt ( 0.0f, 0.0f, -0.0f );

  //mCamera.SetViewParams( &vecEye, &vecAt );
}
//--------------------------------------------------------------------------------------------------------
//void TManagerDirectX::ResetDevice(IDirect3DDevice9* pd3dDevice,
//                                     const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
//{
//  HRESULT hr;
//  mManagerModel.ResetDevice();
//  
//  // Setup the camera's projection parameters
//  float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;
//  mCamera.SetProjParams( D3DX_PI / 4, fAspectRatio, 0.1f, 1000.0f );
//  mCamera.SetWindow( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );
//  mD3DDevice = pd3dDevice;
//}
//--------------------------------------------------------------------------------------------------------
//void TManagerDirectX::OnLostDevice()
//{
//  mManagerModel.OnLostDevice();
//  mD3DDevice = NULL;
//}
//--------------------------------------------------------------------------------------------------------
//void TManagerDirectX::OnDestroyDevice()
//{
//  mManagerModel.OnDestroyDevice();
//  mManagerObjectDX.Clean();
//
//  mD3DDevice = NULL;
//}
//--------------------------------------------------------------------------------------------------------
//bool TManagerDirectX::Start(void *pFuncCallExit)
//{
//  return pDX->Start(this,pFuncCallExit);
//}
////--------------------------------------------------------------------------------------------------------
//void TManagerDirectX::Stop()
//{
//  if(pDX->Stop()==false)
//    BL_FIX_BUG();
//}
////--------------------------------------------------------------------------------------------------------
//void TManagerDirectX::Show()
//{
//  pDX->Show();
//} 
////--------------------------------------------------------------------------------------------------------
//void TManagerDirectX::Hide()
//{
//  pDX->Hide();
//}
////--------------------------------------------------------------------------------------------------------
//void TManagerDirectX::ParserPacket()// обработка пакета
//{
//
//}
//--------------------------------------------------------------------------------------------------------
//TObjectDX* TManagerDirectX::LoadObjectDX(unsigned int id/*уникальный идентификатор, не карта*/,bool flgNeedClear)// потом как правило идет настройка объекта
//{
//  //TLoaderMap loaderMap(&mManagerObjectDX,&mManagerModel);
//  TLoaderMap::TDescObject oDesc;
//  oDesc.id = id;// cube_pro
//  oDesc.state = 1;
//  oDesc.coord.x = 0;
//  oDesc.coord.y = 0;
//  oDesc.coord.z = 0;
//  oDesc.orient.vx = 0;
//  oDesc.orient.vy = 0;
//  oDesc.orient.vz = 0;
//  return mLoaderMap.LoadObjectDX(&oDesc,flgNeedClear);
//}
//--------------------------------------------------------------------------------------------------------
//void TManagerDirectX::SetKeyHandler(IKeyHandler* pKeyHandler)
//{
//  mKeyHandler = pKeyHandler;
//}
////--------------------------------------------------------------------------------------------------------
//// контроль над событиями мышки
//void TManagerDirectX::SetMouseHandler(IMouseHandler* pMouseHandler)
//{
//  mMouseHandler = pMouseHandler;
//}
////--------------------------------------------------------------------------------------------------------

