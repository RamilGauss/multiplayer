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


#include "DXUT.h"
#include "DXUTcamera.h"
#include "DXUTsettingsdlg.h"
#include "SDKmisc.h"
#pragma warning(disable: 4995)
#include "MeshLoader.h"
#pragma warning(default: 4995)
#include <list>
#include "Struct3D.h"
#include "ManagerModelDX.h"
#include <set>
#include "DXUT_Class.h"
#include "glibconfig.h"


class TBaseObjectDX;

// BigJack - графический движок
class TManagerDirectX
{
protected:

  guint32 mTime_ms;// время для рендера, используется для анимации

  TDXUT mDXUT;

  TManagerModelDX mManagerModel;

  // все объекты
  std::list<TBaseObjectDX*> mListAllObject;// движок не освобождает память из-под этих объектов
  // список на отрисовку
  std::list<TBaseObjectDX*> mListReadyRender;// временный список

  std::list<TBaseObjectDX*> mListAnimateObject;// только анимированные. создаются движком(эффект движка), движок сам должен освободить память

  CModelViewerCamera mCamera;                // A model viewing camera

public:
  //----------------------------------------------------------------
  //                              INTERFACE
  //----------------------------------------------------------------
  typedef void (*TCallBackMsg)( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
  typedef void (*TCallBackFrameMove)( double fTime, float fElapsedTime, void* pUserContext );

  TManagerDirectX();
  ~TManagerDirectX();

  enum{eTypeMsg     = 0,
       eTypeFrameMove,
  };

  // на получение событий WinApi окна и DirectX
  void Register(void*   pFunc, int type);
  void Unregister(void* pFunc, int type);
  // в частности события мыши и клавиатуры
  void NotifyMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
  void NotifyFrameMove(double fTime, float fElapsedTime, void* pUserContext);
  //------------------------------------------------------------------------
  void Init(HWND hwnd = NULL);
  void Work(guint32 time_ms);
  void Done();
  //------------------------------------------------------------------------
  void AddObject(TBaseObjectDX* pObject);
  void Clear();
  // камера
  void SetViewParams(D3DXVECTOR3* pvEyePt, D3DXVECTOR3* pvLookatPt);// расположение камеры
  
  // 21 декабря 2012 года реализую:
  // клиентские эффекты движка, не влияют на физические параметры объектов
  void SetEffect(unsigned short id_effect/*уникальный эффект, см. таблицу эффектов*/,
                 D3DVECTOR& coord,     // где
                 D3DVECTOR& orient,    // ориентация эффекта
                 guint32 time_past/* прошло времени, мс*/ = 0);
  //----------------------------------------------------------------
  //                             ~INTERFACE
  //----------------------------------------------------------------
public:
  //----------------------------------------------------------------
  // Для внутренних событий движка.
  //----------------------------------------------------------------

  bool IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed,void* pUserContext );
  bool ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );
  HRESULT OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,void* pUserContext );
  HRESULT OnResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,void* pUserContext );
  void OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );
  void OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
  LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,void* pUserContext );
  void OnLostDevice( void* pUserContext );
  void OnDestroyDevice( void* pUserContext );

protected:
  std::set<TCallBackMsg>       mSetCallbackMsg;
  std::set<TCallBackFrameMove> mSetCallbackFrameMove;

  void RegisterSet  (std::set<void*>* setCallback, void* pFunc);
  void UnregisterSet(std::set<void*>* setCallback, void* pFunc);

protected:

  void Optimize();
  void Render(IDirect3DDevice9* pd3dDevice);
  void Animate();// для анимации и подготовки стека шейдера

};

#endif