#ifndef LayerBetween_QT_DirectXH
#define LayerBetween_QT_DirectXH

#include "glib\gthread.h"

class TLayerBetween_QT_DirectX
{

  GThread* thread;

  enum{eTimeWaitStop=50,
  };

public:

  TLayerBetween_QT_DirectX();
  ~TLayerBetween_QT_DirectX();

  bool StartDirectX(void *pFuncCallExit);
  void StopDirectX();

};

extern TLayerBetween_QT_DirectX g_LB_QT_DX;

#endif

