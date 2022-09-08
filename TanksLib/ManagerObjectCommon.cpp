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
#include "TankTower.h"

TManagerObjectCommon::TManagerObjectCommon()
{
  thread = NULL;
  flgActiveLoadThread = false;
  mLastTimeFreshData  = 0;
}
//--------------------------------------------------------------------
TManagerObjectCommon::~TManagerObjectCommon()
{
  Done();
}
//--------------------------------------------------------------------
void TManagerObjectCommon::Setup(IDirect3DDevice9* pDevice)
{
  mProgressBar.CreateDeviceEvent(pDevice);
  mMDX_Scene.CreateDeviceEvent(pDevice);
}
//--------------------------------------------------------------------
void TManagerObjectCommon::VisualEvent(guint32 iTime, float fElapsedTime)
{
  if(flgActiveLoadThread)
  {
    mProgressBar.VisualEvent(mProcentLoadMap);
  }
  else
  {
    if(iTime>mLastTimeFreshData+eTimeoutFreshData)
    {
      mPrediction.Calc();
      mLastTimeFreshData = ht_GetMSCount();
    }

    mMDX_Scene.VisualEvent(iTime, fElapsedTime);
  }
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
  mProcentLoadMap = 0;
  LoadMap(mPacketInFight.getCodeMap());
  mProcentLoadMap = 20;
  AddFromLoaderObjectInCommonList();
  mProcentLoadMap = 50;
  AddFromLoaderObjectInMDX();
  mProcentLoadMap = 70;
  AddFromLoaderObjectInPrediction();
  mProcentLoadMap = 90;
  AddTankInCommonList();
  mProcentLoadMap = 100;
  //---------------------------------------
  SendCorrectPacket();
  flgActiveLoadThread = false;
}
//--------------------------------------------------------------------
void TManagerObjectCommon::LoadMap(unsigned int id_map)
{
  mLoaderObject.LoadMap(id_map);
}
//--------------------------------------------------------------------
bool TManagerObjectCommon::IsLoadMap(unsigned char& procent)
{
  if(flgActiveLoadThread)
  {
    procent = mProcentLoadMap;
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
void TManagerObjectCommon::SetPacketA_In_Fight(char* pData, int size)
{
  // к сожалению поток загрузки карты прервать нельзя,
  // ждите
  mPacketInFight.setData(pData,size);
  flgActiveLoadThread = true;
  thread = g_thread_create(Thread, this, true, NULL);
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
void TManagerObjectCommon::AddTankInCommonList()
{
  TMakerBehavior mMaker;
  int cntTank = mPacketInFight.getCountTank();
  int cntObject = mVectorObject.size();
  int cntAll = cntTank+cntObject;

  mVectorObject.reserve(cntAll);
  for(int i = cntObject ; i < cntAll ; i++ )
  {
    // 
    TBaseObjectCommon* pObject = mMaker.New(nsID_BEHAVIOR::ID_TANK_TOWER);
    PrepareTank((TTankTower*)pObject,i);

    mVectorObject.push_back(pObject);
    mMDX_Scene.AddObject(pObject);
    mPrediction.AddObject(pObject);
  }
}
//--------------------------------------------------------------------
void TManagerObjectCommon::Done()
{
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
void TManagerObjectCommon::RefreshFromServer()
{
  mLastTimeFreshData = ht_GetMSCount();
}
//--------------------------------------------------------------------
void TManagerObjectCommon::PrepareTank(TTankTower* pTank, int i)
{
  pTank->SetTypeTank(mPacketInFight.getID_Tank(i));
  pTank->mTower =  mPacketInFight.getTowerType(i);
  pTank->mGun = mPacketInFight.getGunType(i);
}
//--------------------------------------------------------------------