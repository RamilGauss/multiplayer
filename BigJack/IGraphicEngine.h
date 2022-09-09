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


#ifndef IGraphicEngineH
#define IGraphicEngineH

#include "DXUT.h"
#include "glibconfig.h"
#include "MakerDirectX_Realize.h"

class TBaseObjectDX;

class IGraphicEngine
{
protected:
  guint32 mTime_ms;// время для рендера, используется для анимации
  IDirectX_Realize* mDXUT;

public:
  typedef void (*TCallBackMsg)( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
  typedef void (*TCallBackFrameMove)( double fTime, float fElapsedTime, void* pUserContext );

  IGraphicEngine()
  {  
    TMakerDirectX_Realize makerDX;
    mDXUT = makerDX.New(this);
  }
  virtual ~IGraphicEngine()
  { 
    delete mDXUT;
    mDXUT = NULL;
  }

  enum{eTypeMsg     = 0,
    eTypeFrameMove,
  };

  virtual bool IsFullScreen(){return mDXUT->IsFullScreen();};
  virtual void* GetWndProc(){return mDXUT->GetWndProc();}
  // на получение событий WinApi окна и DirectX
  virtual void Register(void*   pFunc, int type) = 0;
  virtual void Unregister(void* pFunc, int type) = 0;
  // в частности события мыши и клавиатуры
  virtual void NotifyMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
  virtual void NotifyFrameMove(double fTime, float fElapsedTime, void* pUserContext) = 0;
  //------------------------------------------------------------------------
  virtual void Init(HWND hwnd = NULL) = 0;
  virtual void Work(guint32 time_ms) = 0;
  virtual void Done() = 0;
  //------------------------------------------------------------------------
  virtual void AddObject(TBaseObjectDX* pObject) = 0;
  virtual void Clear() = 0;
  // камера
  virtual void SetViewParams(D3DXVECTOR3* pvEyePt, D3DXVECTOR3* pvLookatPt) = 0;// расположение камеры

  // 21 декабря 2012 года реализую:
  // клиентские эффекты движка, не влияют на физические параметры объектов
  virtual void SetEffect(unsigned short id_effect/*уникальный эффект, см. таблицу эффектов*/,
    D3DVECTOR& coord,     // где
    D3DVECTOR& orient,    // ориентация эффекта
    guint32 time_past/* прошло времени, мс*/ = 0) = 0;
  virtual void SetViewFPS(bool val) = 0;
  // источники освещения
  // ###
  // ввод освещение накладывает условия на шейдер. он обязан содержать интерфейс
  virtual int GetCountLight() = 0;
  virtual const D3DVECTOR* GetCoordLight(int index) = 0;
  virtual const D3DVECTOR* GetCoordAtLight(int index) = 0;
  virtual unsigned int GetColorLight(int index) = 0;
  virtual void SetCoordLight(int index,D3DVECTOR* m) = 0;
  virtual void SetCoordAtLight(int index,D3DVECTOR* m) = 0;
  virtual void SetColorLight(int index, unsigned int color) = 0;
  virtual void AddLight() = 0;
  virtual void RemoveLight(int index) = 0;
  // ###
  //----------------------------------------------------------------
  //                             ~INTERFACE
  //----------------------------------------------------------------

public:
  //----------------------------------------------------------------
  // Для внутренних событий движка.
  //----------------------------------------------------------------

  virtual bool IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed,void* pUserContext ) = 0;
  virtual bool ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext ) = 0;
  virtual HRESULT OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,void* pUserContext ) = 0;
  virtual HRESULT OnResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,void* pUserContext ) = 0;
  virtual void OnFrameMove( double fTime, float fElapsedTime, void* pUserContext ) = 0;
  virtual void OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext ) = 0;
  virtual LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,void* pUserContext ) = 0;
  virtual void OnLostDevice( void* pUserContext ) = 0;
  virtual void OnDestroyDevice( void* pUserContext ) = 0;

};


#endif

