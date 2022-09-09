/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012
===========================================================================
                        Common Information
"TornadoEngine" GPL Source Code

This file is part of the "TornadoEngine" GPL Source Code.

"TornadoEngine" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"TornadoEngine" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "TornadoEngine" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "TornadoEngine" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "TornadoEngine" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef BigJackH
#define BigJackH

#ifdef WIN32
  #include "DXUT.h"
  #include "DXUTcamera.h"
  #include "DispTextDX.h"
  #include "ManagerModelDX.h"
  #include "ManagerResourceDX.h"
#else
#endif

#include <list>
#include <set>
#include "glibconfig.h"
#include "Struct3D.h"
#include "ShaderStack.h"
#include "IGraphicEngine.h"
#include "StateMachine.h"
#include "MapSystemKey.h"


class IBaseObjectGE;
class IDirectX_Realize;

// BigJack - графический движок
class TBigJack : public IGraphicEngine
{
protected:

#ifdef WIN32
  IDirectX_Realize* mDXUT;
  TManagerResourceDX mManagerResourceDX;
  TManagerModelDX mManagerModelDX;

  TDispTextDX mViewerFPS;

  CModelViewerCamera mCamera;                       // A model viewing camera
#else
#endif

  TShaderStack mMainShaderStack;

  int mIndexView;
  int mIndexProj;
  int mIndexCameraPosition;

  typedef std::list<IBaseObjectGE*> TListPtr;
  // все объекты
  TListPtr mListAllObject;// движок не освобождает память из-под этих объектов
  // список на отрисовку
  TListPtr mListReadyRender;// временный список

  TListPtr mListAnimateObject;// только анимированные. создаются движком(эффект движка), движок сам должен освободить память

  TListPtr mListTransparencyObject;// прозрачные объекты, временный список, только на этапе создания списка на отображение

  bool flgNeedResizeGUI;

  // для независимости от платформы, мэппинг клавиатурных событий
  TMapSystemKey mMapSysytemKeys;

public:
  //----------------------------------------------------------------
  //                              INTERFACE
  //----------------------------------------------------------------
  TBigJack(ICamera* pCamera);
  virtual ~TBigJack();

  //------------------------------------------------------------------------
  virtual void Init();
  virtual void Work(guint32 time_ms);
  virtual bool HandleInternalEvent();
  virtual void Done();

  virtual bool IsFullScreen();
  virtual void ToggleFullScreen();
  virtual void SetTitleWindow(const char* sTitle);

  virtual void  GetResolutionFrame(int& h, int& w );// формат X8R8G8B8
  virtual void  SetResolutionFrame(int  h, int  w );// формат X8R8G8B8
  //------------------------------------------------------------------------
  virtual void AddObject(IBaseObjectGE* pObject);
  virtual void Clear();

  // клиентские эффекты движка, не влияют на физические параметры объектов
  virtual void SetEffect(unsigned short id_effect/*уникальный эффект, см. таблицу эффектов*/,
    nsStruct3D::TVector3* coord3,     // где
    nsStruct3D::TVector3* orient3,    // ориентация эффекта
    guint32 time_past/* прошло времени, мс*/ = 0);
  virtual void SetViewFPS(bool val);
  // источники освещения
  // ###
  // ввод освещение накладывает условия на шейдер. он обязан содержать интерфейс
  virtual int GetCountLight(){return 1;}
  virtual const float* GetCoordLight(int index){return NULL;}
  virtual const float* GetCoordAtLight(int index){return NULL;}
  virtual unsigned int GetColorLight(int index){return 0;}
  virtual void SetCoordLight(int index,nsStruct3D::TVector3* m3){}
  virtual void SetCoordAtLight(int index,nsStruct3D::TVector3* m3){}
  virtual void SetColorLight(int index, unsigned int color){}
  virtual void AddLight(){}
  virtual void RemoveLight(int index){}
  // ###
  //----------------------------------------------------------------
  //                             ~INTERFACE
  //----------------------------------------------------------------
protected:
  //----------------------------------------------------------------
  // Для внутренних событий движка.
  //----------------------------------------------------------------
  friend class TDXUT;
  bool IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed,void* pUserContext );
  bool ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );
  HRESULT OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,void* pUserContext );
  HRESULT OnResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,void* pUserContext );
  void OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );
  void OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
  LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,void* pUserContext );
  void OnLostDevice( void* pUserContext );
  void OnDestroyDevice( void* pUserContext );
  
  void OnKeyEvent( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
  void OnMouseEvent( int state, 
                     int nMouseWheelDelta, 
                     int xPos, int yPos, void* pUserContext );

protected:

  void RegisterSet  (std::set<void*>* setCallback, void* pFunc);
  void UnregisterSet(std::set<void*>* setCallback, void* pFunc);

protected:
  
  void MakeVectorOnRender();
  void Optimize();
  void Render(IDirect3DDevice9* pd3dDevice);
  void Animate();// для анимации и подготовки стека шейдера


  void SetCommonShaderStack();

  virtual bool InitGUI();

  void SaveRenderState();
  void RestoreRenderState();


  // подстрйока под глюк DXUT 21.12.2012 Gauss
  bool IsNeedResizeGUI(){return flgNeedResizeGUI;}
  void SetNeedResizeGUI(bool val){flgNeedResizeGUI=val;}

  void DispFPS();
};

#endif