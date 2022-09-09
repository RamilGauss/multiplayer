/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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


#ifndef BigJackH
#define BigJackH


#include "DXUT.h"
#include "IGraphicEngine.h"

#include "DXUTcamera.h"
#include "DXUTsettingsdlg.h"
#include "SDKmisc.h"
#pragma warning(disable: 4995)
#include "MeshLoader.h"
#pragma warning(default: 4995)
#include <list>
#include <set>
#include "Struct3D.h"
#include "ManagerModelDX.h"
#include "DXUT_Class.h"
#include "glibconfig.h"
#include "ManagerResourceDX.h"
#include "ShaderStack.h"
#include "ViewerFPS.h"



class IBaseObjectDX;

// BigJack - ����������� ������
class TBigJack : public IGraphicEngine
{
protected:

  TDXUT mDXUT;
  //IDirect3DSurface9* mSurfaceBackBuffer;
  //IDirect3DSurface9* mSurfaceRender;


  TManagerResourceDX mManagerResourceDX;
  TManagerModelDX mManagerModelDX;

  TShaderStack mMainShaderStack;

  TViewerFPS mViewerFPS;

  int mIndexView;
  int mIndexProj;
  int mIndexCameraPosition;


  // ��� �������
  std::list<IBaseObjectDX*> mListAllObject;// ������ �� ����������� ������ ��-��� ���� ��������
  // ������ �� ���������
  std::list<IBaseObjectDX*> mListReadyRender;// ��������� ������

  std::list<IBaseObjectDX*> mListAnimateObject;// ������ �������������. ��������� �������(������ ������), ������ ��� ������ ���������� ������

  std::list<IBaseObjectDX*> mListTransparencyObject;// ���������� �������, ��������� ������, ������ �� ����� �������� ������ �� �����������

  CModelViewerCamera mCamera;                // A model viewing camera

public:
  //----------------------------------------------------------------
  //                              INTERFACE
  //----------------------------------------------------------------
  TBigJack();
  virtual ~TBigJack();

  // �� ��������� ������� WinApi ���� � DirectX
  //virtual void Register(void*   pFunc, int type);
  //virtual void Unregister(void* pFunc, int type);
  // � ��������� ������� ���� � ����������
  //virtual void NotifyMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
  //virtual void NotifyFrameMove(double fTime, float fElapsedTime, void* pUserContext);
  //------------------------------------------------------------------------
  virtual void Init(HWND hwnd = NULL);
  virtual void Work(guint32 time_ms);
  virtual void Done();

  virtual bool IsFullScreen();
  virtual void ToggleFullScreen();
  
  virtual void  GetResolutionFrame(int& h, int& w );// ������ X8R8G8B8
  virtual void  SetResolutionFrame(int  h, int  w );// ������ X8R8G8B8
  //------------------------------------------------------------------------
  virtual void AddObject(IBaseObjectDX* pObject);
  virtual void Clear();
  // ������
  virtual void SetViewParams(D3DXVECTOR3* pvEyePt, D3DXVECTOR3* pvLookatPt);// ������������ ������
  
  // 21 ������� 2012 ���� ��������:
  // ���������� ������� ������, �� ������ �� ���������� ��������� ��������
  virtual void SetEffect(unsigned short id_effect/*���������� ������, ��. ������� ��������*/,
                 D3DVECTOR& coord,     // ���
                 D3DVECTOR& orient,    // ���������� �������
                 guint32 time_past/* ������ �������, ��*/ = 0);
  virtual void SetViewFPS(bool val);
  // ��������� ���������
  // ###
  // ���� ��������� ����������� ������� �� ������. �� ������ ��������� ���������
  virtual int GetCountLight(){return 1;}
  virtual const D3DVECTOR* GetCoordLight(int index){return NULL;}
  virtual const D3DVECTOR* GetCoordAtLight(int index){return NULL;}
  virtual unsigned int GetColorLight(int index){return 0;}
  virtual void SetCoordLight(int index,D3DVECTOR* m){}
  virtual void SetCoordAtLight(int index,D3DVECTOR* m){}
  virtual void SetColorLight(int index, unsigned int color){}
  virtual void AddLight(){}
  virtual void RemoveLight(int index){}
  // ###
  //----------------------------------------------------------------
  //                             ~INTERFACE
  //----------------------------------------------------------------
protected:
  //----------------------------------------------------------------
  // ��� ���������� ������� ������.
  //----------------------------------------------------------------
  friend class TDXUT;
  virtual bool IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed,void* pUserContext );
  virtual bool ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );
  virtual HRESULT OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,void* pUserContext );
  virtual HRESULT OnResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,void* pUserContext );
  virtual void OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );
  virtual void OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
  virtual LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,void* pUserContext );
  virtual void OnLostDevice( void* pUserContext );
  virtual void OnDestroyDevice( void* pUserContext );

protected:
  //std::set<TCallBackMsg>       mSetCallbackMsg;
  //std::set<TCallBackFrameMove> mSetCallbackFrameMove;

  void RegisterSet  (std::set<void*>* setCallback, void* pFunc);
  void UnregisterSet(std::set<void*>* setCallback, void* pFunc);

protected:
  
  void MakeVectorOnRender();
  void Optimize();
  void Render(IDirect3DDevice9* pd3dDevice);
  void Animate();// ��� �������� � ���������� ����� �������


  void SetCommonShaderStack();


  void OnLostDevice_Surface();
  void OnResetDevice_Surface(IDirect3DDevice9 *pd3dDevice);


};

#endif