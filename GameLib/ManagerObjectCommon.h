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

#ifndef ManagerObjectCommonH
#define ManagerObjectCommonH

#include "IGraphicEngine.h"
#include "Camera.h"
#include "LoaderObjectCommon.h"
#include "InterpretatorPredictionTank.h"
#include "ApplicationProtocolPacketAnswer.h"
#include "glib/gthread.h"
#include <vector>
#include "ProgressBar.h"
#include "CallBackRegistrator.h"
#include "FilterWinApi.h"
#include "ControlCamera.h"
#include "ManagerEventWinApi.h"
#include "ControlEventWinApiGE.h"
#include "ControlEventWinApiNET.h"
#include "ControlEventWinApiGUI.h"

class TBaseObjectCommon;

class TManagerObjectCommon
{
protected:
  enum{eLoadMapEnd=0,
  };
  TCallBackRegistrator mCallbackLoadMapEndEvent;

  unsigned int mID_map;

  GThread* thread;

  volatile unsigned char mProcentLoadMap;
  
  volatile bool flgActiveLoadThread;// активность потока
  volatile bool flgLoadMap;         // процесс загрузки идет
  volatile bool flgNeedStopThreadLoadMap;    // необходимо остановить загрузку

  guint32 mLastTimeFreshData;

protected:
  
  std::vector< TBaseObjectCommon* > mVectorObject;
  
  TLoaderObjectCommon mLoaderObject;

  TInterpretatorPredictionTank mPrediction;// физика
  IGraphicEngine* mMDX_Scene; // отрисовка сцены
  TControlCamera mControlCamera;
  
  // прогресс загрузки карты/данных
  TProgressBar mProgressBar;

public:

  TManagerObjectCommon();
  virtual ~TManagerObjectCommon();

  void Init(HWND hwnd = NULL);
  virtual void Work() = 0;
  void Done();

  // какие-то данные
  virtual void Translate(unsigned short type, char*pData, int size){};
  // в наследуемом классе добавятся методы для управления - например через сеть или чтение реплея и воспроизведение боя
  /*...*/

protected:

  friend void* Thread(void*p);

  void ThreadLoadMap();
  void NotifyLoadMapEndEvent();

  void AddFromLoaderObjectInCommonList();
  void AddFromLoaderObjectInMDX();
  void AddFromLoaderObjectInPrediction();

  void ClearSceneVectorObject();
  void ClearProgressBarVectorObject();


  // прогресс загрузки карты
  bool IsLoadMap(unsigned char* procent = NULL );// от 0 до 100 
  void SetCameraDelta(int x, int y);
  // загрузить карту
  void LoadMap(unsigned int id_map);
  void EndLoadMap();// очень криво и отвратительно, что есть такой метод! но пока только так.
  void StopLoadMap();// синхронно, придется подождать маленько

  TBaseObjectCommon* Get(int index);// отдать объект на изменение свойств объекта
  void AddObject(TBaseObjectCommon* pObject);
  void Fresh();

  void SetEffect(unsigned int id_effect, // номер эффекта
    D3DVECTOR& coord,     // где
    D3DVECTOR& orient,    // ориентация эффекта
    guint32 time_past = 0); // прошло времени, мс


  friend void CallBackMsg( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
  friend void CallBackFrameMove( double fTime, float fElapsedTime, void* pUserContext );

  virtual void OnMsg( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
  virtual void OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );


  virtual float GetTimeWork() = 0;

  void HandleOnMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  // примочка для транцсляции событий в Qt
  void SetWinApiEvent(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


  //----------------------------------------------------
  // фильтры на обработку событий WinApi
  TFilterWinApi mFilter;
  TManagerEventWinApi       mManagerEventWinApi;
  
  TControlEventWinApiGE  mControlEventGE;
  TControlEventWinApiNET mControlEventNET;
  TControlEventWinApiGUI mControlEventGUI;
};

#endif
