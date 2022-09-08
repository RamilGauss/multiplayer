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

#include "ManagerObjectCommon.h"
#include <stddef.h>
#include "Logger.h"
#include "ApplicationProtocolPacketAnswer.h"
#include "namespace_ID_BEHAVIOR.h"
#include "HiTimer.h"
#include "BaseObjectCommon.h"

TManagerObjectCommon* pMOC = NULL;
//-----------------------------------------------------------------------------
void CallBackMsg( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
  if(pMOC==NULL) return;
  
  pMOC->OnMsg( hWnd, uMsg, wParam, lParam );
}
//-----------------------------------------------------------------------------
void CallBackFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
  if(pMOC==NULL) return;
  
  pMOC->OnFrameMove( fTime, fElapsedTime, pUserContext );
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#define CHECK_END if(flgNeedStopThreadLoadMap==false) \
{ \
  flgActiveLoadThread = false; \
  EndLoadMap(); \
  return; \
}

//-----------------------------------------------------------------------------
TManagerObjectCommon::TManagerObjectCommon()
{
  thread                   = NULL;
  flgLoadMap               = false;
  flgActiveLoadThread      = false;
  mLastTimeFreshData       = 0;

  pMOC = this;
  mMDX_Scene.Register(CallBackMsg,TManagerDirectX::eTypeMsg);
  mMDX_Scene.Register(CallBackFrameMove,TManagerDirectX::eTypeFrameMove);
}
//--------------------------------------------------------------------
TManagerObjectCommon::~TManagerObjectCommon()
{
  pMOC = this;
  mMDX_Scene.Unregister(CallBackMsg,TManagerDirectX::eTypeMsg);
  mMDX_Scene.Unregister(CallBackFrameMove,TManagerDirectX::eTypeFrameMove);
  Done();
}
//--------------------------------------------------------------------
void* Thread(void* p)
{
  ((TManagerObjectCommon*)p)->ThreadLoadMap();
  return NULL;
}
//--------------------------------------------------------------------
void TManagerObjectCommon::ThreadLoadMap()
{
  GlobalLoggerMOC.WriteF_time("Началась загрузка карты.\n");
  flgActiveLoadThread = true;

  mProcentLoadMap = 0;
  CHECK_END
  mLoaderObject.LoadMap(mID_map);
  mProcentLoadMap = 15;
  CHECK_END
  AddFromLoaderObjectInCommonList();
  mProcentLoadMap = 55;
  CHECK_END
  AddFromLoaderObjectInMDX();
  mProcentLoadMap = 70;
  CHECK_END
  AddFromLoaderObjectInPrediction();
  mProcentLoadMap = 80;

  CHECK_END
  //---------------------------------------
  NotifyLoadMapEndEvent();
  //---------------------------------------
  flgActiveLoadThread = false;
  
  GlobalLoggerMOC.WriteF_time("Загрузка карты завершена (поток MOC).\n");
  GlobalLoggerMOC.WriteF_time("Ожидание корректирующего пакета.\n");
}
//--------------------------------------------------------------------
void TManagerObjectCommon::LoadMap(unsigned int id_map)
{
  flgNeedStopThreadLoadMap = false;
  flgLoadMap               = true;
  mID_map = id_map;
  thread = g_thread_create(Thread, this, true, NULL);
}
//--------------------------------------------------------------------
bool TManagerObjectCommon::IsLoadMap(unsigned char* procent)
{
  if(flgLoadMap)
  {
    if(procent)
      *procent = mProcentLoadMap;
    return true;
  }
  return false;
}
//--------------------------------------------------------------------
void TManagerObjectCommon::AddObject(TBaseObjectCommon* pObject)
{
  mVectorObject.push_back(pObject);
  mMDX_Scene.AddObject(pObject);
  mPrediction.AddObject(pObject);
}
//--------------------------------------------------------------------
void TManagerObjectCommon::AddFromLoaderObjectInCommonList()
{
  int cnt = mLoaderObject.GetCountObject();
  mVectorObject.reserve(cnt);
  for(int i = 0 ; i < cnt ; i++ )
  {
    mVectorObject.push_back(mLoaderObject.Get(i));
  }
}
//--------------------------------------------------------------------
void TManagerObjectCommon::AddFromLoaderObjectInMDX()
{
  int cnt = mVectorObject.size();
  for(int i = 0 ; i < cnt ; i++ )
  {
    mMDX_Scene.AddObject((TBaseObjectDX*)mVectorObject[i]);
  }
}
//--------------------------------------------------------------------
void TManagerObjectCommon::AddFromLoaderObjectInPrediction()
{
  int cnt = mVectorObject.size();
  for(int i = 0 ; i < cnt ; i++ )
  {
    mPrediction.AddObject((TBaseObjectPrediction*)mVectorObject[i]);
  }
}
//--------------------------------------------------------------------
void TManagerObjectCommon::Done()
{
  mMDX_Scene.Done();// отцепиться от WinApi и DirectX
  ClearSceneVectorObject();
  ClearProgressBarVectorObject();
}
//--------------------------------------------------------------------
void TManagerObjectCommon::ClearSceneVectorObject()
{
  mPrediction.Clear();
  mMDX_Scene.Clear();
  int cnt = mVectorObject.size();
  for(int i = 0 ; i < cnt ; i++ )
  {
    delete mVectorObject[i];
  }
  mVectorObject.clear();
}
//--------------------------------------------------------------------
void TManagerObjectCommon::ClearProgressBarVectorObject()
{
  mProgressBar.Clear();
}
//--------------------------------------------------------------------
void TManagerObjectCommon::Fresh()
{
  mLastTimeFreshData = ht_GetMSCount();
}
//--------------------------------------------------------------------
void TManagerObjectCommon::SetCameraDelta(int x, int y)
{

  //mMDX_Scene.SetViewParams();
}
//--------------------------------------------------------------------
TBaseObjectCommon* TManagerObjectCommon::Get(int index)
{
  TBaseObjectCommon* pObject = mVectorObject.at(index);
  return pObject;
}
//--------------------------------------------------------------
void TManagerObjectCommon::NotifyLoadMapEndEvent()
{
  mCallbackLoadMapEndEvent.Notify(NULL,0);
}
//--------------------------------------------------------------
void TManagerObjectCommon::SetEffect(unsigned int id_effect, // номер эффекта
               D3DVECTOR& coord,     // где
               D3DVECTOR& orient,    // ориентация эффекта
               float time_past ) // прошло времени, мс
{
  mMDX_Scene.SetEffect(id_effect,coord,orient,time_past);
}
//--------------------------------------------------------------
void TManagerObjectCommon::EndLoadMap()
{
  GlobalLoggerMOC.WriteF_time("Загрузка карты завершена (поток Form).\n");
  mProcentLoadMap          = 100;
  flgLoadMap               = false;
}
//--------------------------------------------------------------
void TManagerObjectCommon::StopLoadMap()// синхронно, придется подождать маленько
{
  GlobalLoggerMOC.WriteF_time("Приказ остановить загрузку карты.\n");
  flgNeedStopThreadLoadMap = true;
}
//--------------------------------------------------------------  
void TManagerObjectCommon::Init(HWND hwnd)
{
  mMDX_Scene.Init(hwnd);
}
//--------------------------------------------------------------  
//void TGameForm::keyPressEvent( QKeyEvent * event )
//{
//  switch(event->key())
//  {
//    case Qt::Key_Escape:
//    {
//      pClient->SendRequestExitFromFight();
//      break;
//    }
//    default:;
//  }
//}
//--------------------------------------------------------------------------------------------------------
void TManagerObjectCommon::OnMsg( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

}
//--------------------------------------------------------------------------------------------------------
void TManagerObjectCommon::OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{

}
//--------------------------------------------------------------------------------------------------------