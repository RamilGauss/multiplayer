#ifndef ManagerDirectXH
#define ManagerDirectXH

#include "glib\gthread.h"
#include "ApplicationProtocolPacketAnswer.h"

#include "SoundDX.h"
#include <list>
#include "Struct3D.h"
#include "LoaderMap.h"
#include "DXUTcamera.h"
#include "Prediction.h"

class TDX;
struct IDirect3DDevice9;
class TObjectDX;

class TManagerDirectX
{

  TPrediction mPrediction;

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
  // владеет потоком загрузки карты
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

public:

  TManagerDirectX();
  ~TManagerDirectX();

  CModelViewerCamera* getCamera(){return &mCamera;}

  D3DXHANDLE* getWorldViewProjection(){return &hmWorldViewProjection;}
  D3DXHANDLE* getWorld(){return &hmWorld;}
  D3DXHANDLE* getTime(){return &hfTime;}


  void VisualEvent(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext);
  void KeyEvent(unsigned int nChar, bool bKeyDown, bool bAltDown, void* pUserContext);
  void MouseEvent(double fTime, float fElapsedTime, void* pUserContext);
  void LoadMap(TA_In_Fight& packet);

  // вызывается из DXUT либо на отрисовку либо для очистки буфера
  void Refresh();

  void CreateDeviceEvent(IDirect3DDevice9* pd3dDevice, 
    const D3DSURFACE_DESC* pBackBufferSurfaceDesc,void* pUserContext );
  void ResetDevice(IDirect3DDevice9* pd3dDevice,
    const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
  void OnLostDevice();
  void OnDestroyDevice();

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