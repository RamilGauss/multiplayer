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
#include <list>
#include "Struct3D.h"
#include "ManagerModelDX.h"


class TBaseObjectDX;

struct IDirect3DDevice9;

// BigJack - ����������� ������
class TManagerDirectX
{

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
  // ������� ������� �������� �����
  //GThread* threadLoadMap;
  //bool flgNeedLoadMap;
  //bool flgLoadingMap;
  enum{
    eWaitLoadModel=100
  };
  //unsigned int 

  volatile bool flgNeedLoadObjectDX;
  volatile bool flgNeedSendCorrectPacket;

  std::list<TBaseObjectDX*> mListAllObject;
  // ������ �� ���������
  std::list<TBaseObjectDX*> mListReadyRender;

  enum{
    eLoadMap   = 0,// ��� ���������� ��� ���������
    eCountDown = 1,
    eFight     = 2,
  };
  int mState;
  // ����� ���������� ������� ������� ����������
  guint32 mLastTimeSendMouseStream;
  enum{
    eIntervalMouseStream = 100,// ��
  };

  CModelViewerCamera mCamera;                // A model viewing camera

  D3DXHANDLE         hmWorldViewProjection;
  D3DXHANDLE         hmWorld;
  D3DXHANDLE         hfTime;

  IDirect3DDevice9* mD3DDevice;
  
  //TDX* pDX;// ��������� ������� Windows, 
  // !!! �� ������� �� ������, �.�. ������ �������� � ���� ���� #include "DX_main.h"
  //TBufferizator2Thread mBufferizator2Thread;// �����, ������ ����� ������� ��� ������
  TManagerModelDX        mManagerModel;

public:
  //----------------------------------------------------------------
  //                              INTERFACE
  //----------------------------------------------------------------
  TManagerDirectX();
  ~TManagerDirectX();

  //----------------------------------------------------------------
  //                             INTERFACE
  //----------------------------------------------------------------
  // ���������� ������� �������� (������) �����
  //void UnloadAll();
  //void LoadMap(unsigned short id_map);// ����������� �������� �����
  // ������: 1. ��������� ������������ ����� � ������,
  //         2. ��������� ������, � ����� ����� (������ ������ �� �����, ������ �������)
  //bool SaveCurrentMap();// �������� ������� ��������� �������� �� ����� (����� ������, ��� MobileDamageObject ### - ���-�� ���)
  // ���������� ��������� �����
  //TObjectDX* LoadObjectDX(unsigned int id_model/*���������� ������������� ������*/,
                          //unsigned int id_object/*���������� ������������� ������� �� �����*/,
                          //unsigned short type_object,/*��� �������*/
                          //bool flgNeedClear = false);// ����� ��� ������� ���� ��������� �������
  //TObjectDX* GetObject(unsigned int id_object);// ��� ���������
  //unsigned int GetCountObject();// ���-�� �������� �� �����
  // ���������� ������� ������, �� ������ �� ���������� ��������� ��������
  //void SetEffect(unsigned short id_effect/*���������� ������, ��. ������� ��������*/,
  //               D3DXMATRIXA16* loc/*��������������*/,
  //               double time_rest/*��������� �����*/ = 0);
  // ������
  //void SetCameraView(D3DXMATRIXA16* mView);// ������������ ������
  //----------------------------------------------------------------
  //                             ~INTERFACE
  //----------------------------------------------------------------
protected:
  // ���������� �� DXUT ���� �� ��������� ���� ��� ������� ������
  void Refresh();

protected:
  //friend class TDX;
  //friend void* ThreadLoadMap(void* p);
  //friend HRESULT WINAPI DXUTMainLoop( TManagerDirectX* pObjRefresh, HACCEL hAccel);

  //CModelViewerCamera* getCamera(){return &mCamera;}

  D3DXHANDLE* getWorldViewProjection(){return &hmWorldViewProjection;}
  D3DXHANDLE* getWorld(){return &hmWorld;}
  D3DXHANDLE* getTime(){return &hfTime;}

  void VisualEvent(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext);
  //void KeyEvent(unsigned int nChar, bool bKeyDown, bool bAltDown, void* pUserContext);
  //void MouseEvent(double fTime, float fElapsedTime, void* pUserContext);

  void CreateDeviceEvent(IDirect3DDevice9* pd3dDevice); 
    //const D3DSURFACE_DESC* pBackBufferSurfaceDesc,void* pUserContext );
  //void ResetDevice(IDirect3DDevice9* pd3dDevice,
    //const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
  //void OnLostDevice();
  //void OnDestroyDevice();

protected:

  void Calc();
  void Optimize();
  void Render(IDirect3DDevice9* pd3dDevice, double fTime = 0, float fElapsedTime = 0, void* pUserContext = NULL);
  //void SetStateByTypeStream();

  //void ThreadLoadMap();

  //void ParserPacket();// ��������� ������

protected:
  //TSoundDX  mSound;
};

#endif