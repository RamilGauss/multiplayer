/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
√удаков –амиль —ергеевич 
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

#ifndef ManagerObjectCommonH
#define ManagerObjectCommonH

#include "BaseObjectCommon.h"
#include "LoaderObjectCommon.h"
#include "ManagerDirectX.h"
#include "InterpretatorPredictionTank.h"
#include "ApplicationProtocolPacketAnswer.h"
#include "glib/gthread.h"
#include <vector>
#include "ProgressBar.h"

class TTankTower;

class TManagerObjectCommon
{
protected:
  GThread* thread;

  TA_In_Fight mPacketInFight;

  volatile unsigned char mProcentLoadMap;

  volatile bool flgActiveLoadThread;

  guint32 mLastTimeFreshData;
  enum{eTimeoutFreshData=10, // мс
  };

protected:
  
  std::vector< TBaseObjectCommon* > mVectorObject;
  
  TLoaderObjectCommon mLoaderObject;

  TInterpretatorPredictionTank mPrediction;// физика
  TManagerDirectX mMDX_Scene; // отрисовка сцены
  
  // прогресс загрузки карты
  TProgressBar mProgressBar;

public:

  TManagerObjectCommon();
  virtual ~TManagerObjectCommon();

  void Setup(IDirect3DDevice9* pDevice);

  void VisualEvent(guint32 iTime, float fElapsedTime);

  // в бою
  void SetPacketA_In_Fight(char* pData, int size);

  void AddObject(TBaseObjectCommon* pObject);
  // прогресс загрузки карты
  bool IsLoadMap(unsigned char& procent);// от 0 до 100 

  void RefreshFromServer();

protected:

  void Done();

  friend void* Thread(void*p);

  void ThreadLoadMap();
  // загрузить карту
  void LoadMap(unsigned int id_map);

  void AddFromLoaderObjectInCommonList();
  void AddFromLoaderObjectInMDX();
  void AddFromLoaderObjectInPrediction();
  void AddTankInCommonList();

  void ClearSceneVectorObject();
  void ClearProgressBarVectorObject();

  void PrepareTank(TTankTower* pTank, int i);

  // отослать через какой-то транспорт запрос на получение корректирующего пакет
  // отсылаетс€ после загрузки карты
  virtual void SendCorrectPacket(){};


};


#endif