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

#include "ManagerObjectCommon.h"
#include <stddef.h>
#include "Logger.h"
#include "HiTimer.h"


//-----------------------------------------------------------------------------
TManagerObjectCommon::TManagerObjectCommon()
{
  thread                   = NULL;
  flgLoadMap               = false;
  flgActiveLoadThread      = false;
}
//--------------------------------------------------------------------
TManagerObjectCommon::~TManagerObjectCommon()
{
  Done();
}
//--------------------------------------------------------------------
//void* Thread(void* p)
//{
  //((TManagerObjectCommon*)p)->ThreadLoadMap();
  //return NULL;
//}
//--------------------------------------------------------------------
//void TManagerObjectCommon::ThreadLoadMap()
//{
  /*GlobalLoggerMOC.WriteF_time("Началась загрузка карты.\n");
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
  GlobalLoggerMOC.WriteF_time("Ожидание корректирующего пакета.\n");*/
//}
//--------------------------------------------------------------------
void TManagerObjectCommon::LoadMap(unsigned int id_map)
{
  //flgNeedStopThreadLoadMap = false;
  //flgLoadMap               = true;
  //mID_map = id_map;
  //thread = g_thread_create(Thread, this, true, NULL);
}
//--------------------------------------------------------------------
int TManagerObjectCommon::GetProgressLoadMap()
{
  return 100;
}
//--------------------------------------------------------------------
void TManagerObjectCommon::Done()
{
  Clear();
}
//--------------------------------------------------------------------
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
//-------------------------------------------------------------------------------------------------------------  
void TManagerObjectCommon::Init(IMakerObjectCommon* pMakerObjectCommon)
{
  mLoaderObject.SetMakerObjectCommon(pMakerObjectCommon);
}
//--------------------------------------------------------------------------------------------------------
void TManagerObjectCommon::Clear()
{
  int cnt = mVectorObject.size();
  for(int i = 0 ; i < cnt ; i++ )
    mLoaderObject.Delete(mVectorObject[i]);

  mVectorObject.clear();
}
//--------------------------------------------------------------------------------------------------------
IBaseObjectCommon* TManagerObjectCommon::CreateObject(unsigned int id_model)
{
  IBaseObjectCommon* pObject = mLoaderObject.LoadObject(id_model);
  return pObject;
}
//--------------------------------------------------------------------------------------------------------
