#include "DX_main.h"
#include "LayerBetween_QT_DirectX.h"
#include "HiTimer.h"
#include "BL_Debug.h"


TLayerBetween_QT_DirectX g_LB_QT_DX;

TDX* pDX = NULL;

void* ThreadDirectX(void* pFuncCallExit)
{
  pDX = new TDX;
  // начали отрисовывать видео, озвучивать сцену, отлавливать события Key и Mouse
  pDX->Work(pFuncCallExit);
  delete pDX;
  pDX = NULL;
  return NULL;
}
//---------------------------------------------
TLayerBetween_QT_DirectX::TLayerBetween_QT_DirectX()
{
 thread = NULL;
}
//---------------------------------------------
TLayerBetween_QT_DirectX::~TLayerBetween_QT_DirectX()
{

}
//---------------------------------------------
bool TLayerBetween_QT_DirectX::StartDirectX(void *pFuncCallExit)
{
  thread = g_thread_create(ThreadDirectX,
                                    (gpointer)pFuncCallExit,
                                    true,
                                    NULL);
  return (thread!=NULL);
}
//---------------------------------------------
void TLayerBetween_QT_DirectX::StopDirectX()// синхронная функция
{
  if(pDX)
    if(pDX->Stop()==false)
      BL_FIX_BUG();

  while(pDX)
  {
    ht_msleep(eTimeWaitStop);
  }
}
//---------------------------------------------
