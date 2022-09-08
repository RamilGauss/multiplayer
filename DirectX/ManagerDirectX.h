#ifndef ManagerDirectXH
#define ManagerDirectXH

#include "glib\gthread.h"
#include "ApplicationProtocolPacketAnswer.h"

#include "SoundDX.h"
#include <list>
#include "StructDirectX.h"
#include "LoaderMap.h"
#include "DXUTcamera.h"

class TDX;
struct IDirect3DDevice9;

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

  TA_In_Fight mPacket;
  // ������� ������� �������� �����
  GThread* threadLoadMap;
  bool flgNeedLoadMap;
  bool flgLoadingMap;
  bool flgNeedLoadModel;
  enum{
    eWaitLoadModel=100
  };
  //unsigned int 

  volatile bool flgNeedLoadObjectDX;
  volatile bool flgNeedSendCorrectPacket;

  // ������ �� ���������
  std::list<nsStructDirectX::TObjectDX*> mListReadyRender;

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

  TLoaderMap         mLoaderMap;
  CModelViewerCamera mCamera;                // A model viewing camera
  ID3DXEffect*       mEffect;

  D3DXHANDLE         hmWorldViewProjection;
  D3DXHANDLE         hmWorld;
  D3DXHANDLE         hfTime;

public:

  TManagerDirectX();
  ~TManagerDirectX();

  CModelViewerCamera* getCamera(){return &mCamera;}
  ID3DXEffect*        getEffect(){return mEffect;}

  D3DXHANDLE* getWorldViewProjection(){return &hmWorldViewProjection;}
  D3DXHANDLE* getWorld(){return &hmWorld;}
  D3DXHANDLE* getTime(){return &hfTime;}


  void VisualEvent(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext);
  void KeyEvent(unsigned int nChar, bool bKeyDown, bool bAltDown, void* pUserContext);
  void MouseEvent(double fTime, float fElapsedTime, void* pUserContext);
  void LoadMap(TA_In_Fight& packet);

  // ���������� �� DXUT ���� �� ��������� ���� ��� ������� ������
  void Refresh();

  HRESULT CreateDeviceEvent(IDirect3DDevice9* pd3dDevice, 
    const D3DSURFACE_DESC* pBackBufferSurfaceDesc,void* pUserContext );


protected:

  friend class TDX;
  friend void* ThreadLoadMap(void* p);

  void Calc();
  void Optimize();
  void Render(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext);
  void SetStateByTypeStream();

  void ThreadLoadMap();

protected:
  TSoundDX  mSound;
};

extern TManagerDirectX GlobalManagerDirectX;

#endif