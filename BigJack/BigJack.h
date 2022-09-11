/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
2011, 2012, 2013
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
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef BigJackH
#define BigJackH

#ifdef WIN32
//-------------------------------
// WIN32
  #include "DXUT.h"
  #include "DispTextDX.h"
  #include "ManagerModelDX.h"
  #include "ManagerResourceDX.h"
  #include "EffectDX.h"
//end WIN32

#include <list>
#include <set>
#include <map>

#include "Struct3D.h"
#include "ShaderStack.h"
#include "IGraphicEngine.h"
#include "StateMachine.h"
#include "MapSystemKey.h"


class IBaseObjectGE;
class IDirectX_Realize;

// BigJack - ����������� ������
class TBigJack : public IGraphicEngine
{
protected:

  // Depth-stencil buffer ��� ������� Cube map
	IDirect3DSurface9* mSurfaceDepthStencilCube;       // ��� ������� Cube map

  IDirectX_Realize* mDXUT;
  TManagerResourceDX mManagerResourceDX;
  TManagerModelDX mManagerModelDX;

  TDispTextDX mViewerFPS;

	struct TDescTexturePostEffect
	{
		IDirect3DTexture9* pSrc;
		IDirect3DTexture9* pDst;

		TDescTexturePostEffect()
		{
			pSrc = NULL;
			pDst = NULL;
		}
	};

  typedef std::vector<IDirect3DTexture9*> TVectorPtr;
  typedef std::map<IBaseObjectGE*, TDescTexturePostEffect> TMapPtrDescTPE;


	TShaderStack mMainShaderStack;

  int mShHandleView;
  int mShHandleProj;
  int mShHandleCameraPosition;
	// ��� ���������
	int mShHandleLightPosition;
	int mShHandleLightColor;

  typedef std::list<IBaseObjectGE*> TListPtr;
  // ��� �������
  TListPtr mListAllObject;// ������ �� ����������� ������ ��-��� ���� ��������
  // ������ �� ���������
  TListPtr mListReadyRender;// ��������� ������

  TListPtr mListAnimateObject;// ������ �������������. ��������� �������(������ ������), ������ ��� ������ ���������� ������

  TListPtr mListTransparencyObject;// ���������� �������, ��������� ������, ������ �� ����� �������� ������ �� �����������

  bool flgNeedResizeGUI;

  // ��� ������������� �� ���������, ������� ������������ �������
  TMapSystemKey mMapSysytemKeys;

  ICamera* pCameraInnerNeed;

  //------------------------------------
  // PostEffect
  TMapPtrDescTPE mMapObjTexturePostEffect;

//-------------------------------------------------------
	IDirect3DVertexDeclaration9* mVertDeclPP;

	IDirect3DTexture9* mTextureMainRender;
	IDirect3DTexture9* mTextureSecondRender;

	typedef std::list<TEffectDX*> TListPtrEffect;
	TListPtrEffect mListPtrEffect;
  // ����������
  TEffectDX mEffectPreGlow_black;
  TEffectDX mEffectPreGlow_white;
	// post
	TEffectDX mEffectEdgeDetect;
	TEffectDX mEffectDownFilter4x;
	TEffectDX mEffectUpFilter4x;
	TEffectDX mEffectBloomH;
	TEffectDX mEffectBloomV;
	TEffectDX mEffectCombine4x;
	// �������� � �����
	TEffectDX mEffectBlend;
	TEffectDX mEffectScene;

	bool flgViewPostEffectInMirror;
//-------------------------------------------------------

public:
  //----------------------------------------------------------------
  //                              INTERFACE
  //----------------------------------------------------------------
  TBigJack(ICamera* pCamera);
  virtual ~TBigJack();

  //------------------------------------------------------------------------
  virtual void Init();
  virtual void Work(unsigned int time_ms);
  virtual bool HandleInternalEvent();
  virtual void Done();

  virtual bool IsFullScreen();
  virtual void ToggleFullScreen();
  virtual void SetTitleWindow(const char* sTitle);

  virtual void  GetResolutionFrame(int& h, int& w );// ������ X8R8G8B8
  virtual void  SetResolutionFrame(int  h, int  w );// ������ X8R8G8B8
  //------------------------------------------------------------------------
  virtual void AddObject(IBaseObjectGE* pObject);
  virtual void Clear();

  // ���������� ������� ������, �� ������ �� ���������� ��������� ��������
  virtual void SetEffect(unsigned short id_effect/*���������� ������, ��. ������� ��������*/,
    nsStruct3D::TVector3* coord3,     // ���
    nsStruct3D::TVector3* orient3,    // ���������� �������
    unsigned int time_past/* ������ �������, ��*/ = 0);
  virtual void SetViewFPS(bool val);
  // ��������� ���������
  // ���� ��������� ����������� ������� �� ������. �� ������ ��������� ���������
	virtual void SetLightEnable(int index, bool v);
	virtual bool GetLightEnable(int index);
	virtual int  GetCountAllLight();

	virtual const nsStruct3D::TVector3* GetLightPosition(int index);	
	virtual void SetLightPosition(int index,nsStruct3D::TVector3* m3);
	virtual const nsStruct3D::TVector3* GetLightColor(int index);
	virtual void SetLightColor(int index, nsStruct3D::TVector3* color);

	virtual bool GetPostEffectMirror();
	virtual void SetPostEffectMirror(bool v);
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

	void RenderCubeMap(IDirect3DDevice9* pd3dDevice);// ��� ���������� ���������� CubeMap

  // ������� ������, ��� �������� ������ ��������
  // ��������� ��������, ����� ����� ����� ������ ����� ���� �������� ��� �������� �� �����
  
  // ��� ������ ����� ������
  void RenderBlendTexturePostEffect(const nsStruct3D::TMatrix16* pView, 
                                    const nsStruct3D::TMatrix16* pProj,
                                    const nsStruct3D::TVector3* pPosCamera);
  void RenderSrcTextureForPostEffect(IBaseObjectGE* pObj, 
                                     const nsStruct3D::TMatrix16* view, 
                                     const nsStruct3D::TMatrix16* pProj,
                                     const nsStruct3D::TVector3* pPosCamera);
	void RenderGlow(IBaseObjectGE* pObject);// ����������� ���������� ��� ��������� � ������� �������
	void RenderByEffect(ID3DXEffect* g_pEffect);
  void BlendTextureFromPostEffect(IDirect3DTexture9* pTextureScreen, bool UsePostEffectTexture);

	void SetParamShaderForCubeMap();
	void SetParamShaderForMainRender();


  void LostEventForSurfacePestEffect();
  void ResetEventForSurfacePestEffect();
  void InitEffectForGlow();
	void DestroyEffectForGlow();
	void ResetShader(ID3DXEffect* m_pEffect, DWORD dwWidth, DWORD dwHeight);

  void PrepareForPostEffect(IBaseObjectGE* pObject);
  void RenderObject(IBaseObjectGE* pObject, const nsStruct3D::TMatrix16* view);

	void SetCommonShaderStack(const nsStruct3D::TMatrix16* pView, const nsStruct3D::TMatrix16* pProj, const nsStruct3D::TVector3* pCameraPos);

  virtual bool InitGUI();

  void SaveRenderState();
  void RestoreRenderState();


  // ���������� ��� ���� DXUT 21.12.2012 Gauss
  bool IsNeedResizeGUI(){return flgNeedResizeGUI;}
  void SetNeedResizeGUI(bool val){flgNeedResizeGUI=val;}

  void DispFPS();


  void GetMatrixForCubeMap(int nFace, nsStruct3D::TMatrix16& view);

  void InitLightArr();

  void HandleGUI();

	void ZBufferOn();
	void ZBufferOff();

protected:
	void SaveTexture(const char* name,IDirect3DTexture9* pTexture);
	void SaveSurface(const char* name, IDirect3DSurface9* pSurfa�e);
protected:
	// ���������
	enum{eCountLight=4,};
  struct TLight
  {
    nsStruct3D::TVector3 color;
    nsStruct3D::TVector3 position;
    bool active;
    TLight()
    {
      active   = false;
      color    = nsStruct3D::TVector3(1.0f,1.0f,1.0f);
      position = nsStruct3D::TVector3(0.0f,0.0f,2.4f);
    }
  };
  TLight mArrLight[eCountLight];

  // ����������� ����� ��������� � ������
  nsStruct3D::TVector3 mArrLightColor[eCountLight];
  nsStruct3D::TVector3 mArrLightPosition[eCountLight];

};

#endif
#endif