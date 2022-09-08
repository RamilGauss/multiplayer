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


#ifndef ManagerDirectXH
#define ManagerDirectXH

#include "glib\gthread.h"
#include "SoundDX.h"
#include <list>
#include "Struct3D.h"
#include "LoaderMap.h"
#include "ManagerModel.h"
#include "ManagerObjectDX.h"
#include "DXUTcamera.h"

class TPrediction;
class TDX;
struct IDirect3DDevice9;
class TObjectDX;
class IMouseHandler;
class IKeyHandler;

#define WM_STOP_DXUT (WM_USER+1)

// BigJack - графический движок
class TManagerDirectX
{
  TPrediction* pPrediction;

  enum{
      eSizeBufferStream = 4000,
      eSizeBufferPacket = 4000,
  };

  char mBufferStream[eSizeBufferStream];
  char mBufferPacket[eSizeBufferPacket];

  int mSizeStream;
  int mSizePacket;
  guint32 mTimePacket_ms;
  guint32 mTimeStream_ms;

  //TA_In_Fight mPacket;
  // владеет потоком загрузки карты
  GThread* threadLoadMap;
  bool flgNeedLoadMap;
  bool flgLoadingMap;
  enum{
    eWaitLoadModel=100
  };
  //unsigned int 

  volatile bool flgNeedLoadObjectDX;
  volatile bool flgNeedSendCorrectPacket;

  // список на отрисовку
  std::list<TObjectDX*> mListReadyRender;

  enum{
    eLoadMap   = 0,// или клиентской или серверной
    eCountDown = 1,
    eFight     = 2,
  };
  int mState;
  // время последнего отсылки мышиной ориентации
  guint32 mLastTimeSendMouseStream;
  enum{
    eIntervalMouseStream = 100,// мс
  };

  TLoaderMap         mLoaderMap;
  CModelViewerCamera mCamera;                // A model viewing camera

  D3DXHANDLE         hmWorldViewProjection;
  D3DXHANDLE         hmWorld;
  D3DXHANDLE         hfTime;

  IDirect3DDevice9* mD3DDevice;
  
  TDX* pDX;// поставщик событий Windows, 
  // !!! НЕ править на объект, т.к. нельзя включать в этот файл #include "DX_main.h"
  TBufferizator2Thread mBufferizator2Thread;// буфер, отсюда берем задания для движка
  TManagerModel        mManagerModel;
  TManagerObjectDX     mManagerObjectDX;

  IKeyHandler*   mKeyHandler;
  IMouseHandler* mMouseHandler;

public:
  //----------------------------------------------------------------
  //                              INTERFACE
  //----------------------------------------------------------------
  TManagerDirectX();
  ~TManagerDirectX();

  //----------------------------------------------------------------
  //                             INTERFACE
  //----------------------------------------------------------------
  // управление группой объектов (картой) сцены
  void UnloadAll();
  void LoadMap(unsigned short id_map);// асинхронная загрузка карты
  // нельзя: 1. загружать одновременно карту и объект,
  //         2. загрузить объект, а потом карту (объект удалят со сцены, потеря объекта)
  bool SaveCurrentMap();// сохранит текущее состояние объектов на карте (кроме танков, тип MobileDamageObject ### - как-то так)
  // управление объектами сцены
  TObjectDX* LoadObjectDX(unsigned int id_model/*уникальный идентификатор модели*/,
                          unsigned int id_object/*уникальный идентификатор объекта на карте*/,
                          unsigned short type_object,/*тип объекта*/
                          bool flgNeedClear = false);// потом как правило идет настройка объекта
  TObjectDX* GetObject(unsigned int id_object);// для настройки
  unsigned int GetCountObject();// кол-во объектов на карте
  // клиентские эффекты движка, не влияют на физические параметры объектов
  void SetEffect(unsigned short id_effect/*уникальный эффект, см. таблицу эффектов*/,
                 D3DXMATRIXA16* loc/*местоположение*/,
                 double time_rest/*прошедшее время*/ = 0);
  // камера
  void SetCameraView(D3DXMATRIXA16* mView);// расположение камеры
  //----------------------------------------------------------------
  //                             ~INTERFACE
  //----------------------------------------------------------------
protected:
  // вызывается из DXUT либо на отрисовку либо для очистки буфера
  void Refresh();

protected:
  friend class TDX;
  friend void* ThreadLoadMap(void* p);
  friend HRESULT WINAPI DXUTMainLoop( TManagerDirectX* pObjRefresh, HACCEL hAccel);

  CModelViewerCamera* getCamera(){return &mCamera;}

  D3DXHANDLE* getWorldViewProjection(){return &hmWorldViewProjection;}
  D3DXHANDLE* getWorld(){return &hmWorld;}
  D3DXHANDLE* getTime(){return &hfTime;}

  void VisualEvent(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext);
  void KeyEvent(unsigned int nChar, bool bKeyDown, bool bAltDown, void* pUserContext);
  void MouseEvent(double fTime, float fElapsedTime, void* pUserContext);

  void CreateDeviceEvent(IDirect3DDevice9* pd3dDevice); 
    //const D3DSURFACE_DESC* pBackBufferSurfaceDesc,void* pUserContext );
  void ResetDevice(IDirect3DDevice9* pd3dDevice,
    const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
  void OnLostDevice();
  void OnDestroyDevice();

protected:

  void Calc();
  void Optimize();
  void Render(IDirect3DDevice9* pd3dDevice, double fTime = 0, float fElapsedTime = 0, void* pUserContext = NULL);
  void SetStateByTypeStream();

  void ThreadLoadMap();

  void ParserPacket();// обработка пакета

protected:
  TSoundDX  mSound;
};

#endif