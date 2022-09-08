#ifndef ManagerDirectXH
#define ManagerDirectXH

#include "glib\gthread.h"
#include "ApplicationProtocolPacketAnswer.h"

#include "VisualDX.h"
#include "SoundDX.h"
#include "MouseDX.h"
#include "KeyDX.h"

class TDX;

class TManagerDirectX
{

  TA_In_Fight mPacket;
  // владеет потоком загрузки карты
  GThread* threadLoadMap;
  bool flgNeedLoadMap;
  bool flgLoadingMap;
  //unsigned int 

  volatile bool flgNeedLoadObjectDX;
  volatile bool flgNeedSendCorrectPacket;

public:

  TManagerDirectX();
  ~TManagerDirectX();


  void Work();
  void LoadMap(TA_In_Fight& packet);

  // вызывается из DXUT либо на отрисовку либо для очистки буфера
  void Refresh();

protected:

  friend class TDX;
  friend void* ThreadLoadMap(void* p);

  void Calc();
  void Optimize();
  void Render();


  void ThreadLoadMap();


  TVisualDX mVisual;
  TSoundDX  mSound;
  TMouseDX  mMouse;
  TKeyDX    mKey;

};

extern TManagerDirectX GlobalManagerDirectX;

#endif