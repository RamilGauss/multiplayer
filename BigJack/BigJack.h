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
#include "DXUTgui.h"


class IBaseObjectGE;

// BigJack - ����������� ������
class TBigJack : public IGraphicEngine
{
protected:

  CDXUTDialogResourceManager mDialogResourceManager;

  IDirectX_Realize* mDXUT;

  TManagerResourceDX mManagerResourceDX;
  TManagerModelDX mManagerModelDX;

  TShaderStack mMainShaderStack;

  TViewerFPS mViewerFPS;

  int mIndexView;
  int mIndexProj;
  int mIndexCameraPosition;


  // ��� �������
  std::list<IBaseObjectGE*> mListAllObject;// ������ �� ����������� ������ ��-��� ���� ��������
  // ������ �� ���������
  std::list<IBaseObjectGE*> mListReadyRender;// ��������� ������

  std::list<IBaseObjectGE*> mListAnimateObject;// ������ �������������. ��������� �������(������ ������), ������ ��� ������ ���������� ������

  std::list<IBaseObjectGE*> mListTransparencyObject;// ���������� �������, ��������� ������, ������ �� ����� �������� ������ �� �����������

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
  virtual void Init();
  virtual void Work(guint32 time_ms);
  virtual bool HandleInternalEvent();
  virtual void Done();

  virtual bool IsFullScreen();
  virtual void ToggleFullScreen();
  virtual void SetTitleWindow(const char* sTitle);
  virtual void ForceResizeEventGUI();

  virtual void  GetResolutionFrame(int& h, int& w );// ������ X8R8G8B8
  virtual void  SetResolutionFrame(int  h, int  w );// ������ X8R8G8B8
  //------------------------------------------------------------------------
  virtual void AddObject(IBaseObjectGE* pObject);
  virtual void Clear();
  // ������
  virtual void SetViewParams(nsStruct3D::TVector3* pvEyePt3, nsStruct3D::TVector3* pvLookatPt3);// ������������ ������

  // 21 ������� 2012 ���� ��������:
  // ���������� ������� ������, �� ������ �� ���������� ��������� ��������
  virtual void SetEffect(unsigned short id_effect/*���������� ������, ��. ������� ��������*/,
    nsStruct3D::TVector3* coord3,     // ���
    nsStruct3D::TVector3* orient3,    // ���������� �������
    guint32 time_past/* ������ �������, ��*/ = 0);
  virtual void SetViewFPS(bool val);
  // ��������� ���������
  // ###
  // ���� ��������� ����������� ������� �� ������. �� ������ ��������� ���������
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
  // ��� ���������� ������� ������.
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
  void OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
  
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
  void Animate();// ��� �������� � ���������� ����� �������


  void SetCommonShaderStack();

  virtual void* GetFuncEventGUI();
  
  virtual void* GetObjectForInitGUI();

};

#endif