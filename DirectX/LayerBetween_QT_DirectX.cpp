#include "DX_main.h"
#include "LayerBetween_QT_DirectX.h"
#include "HiTimer.h"
#include "BL_Debug.h"
#include <winbase.h>


TLayerBetween_QT_DirectX g_LB_QT_DX;

TDX* pDX = NULL;

DWORD WINAPI ThreadDirectX(LPVOID pFuncCallExit)
{
  pDX = new TDX;
  // начали отрисовывать видео, озвучивать сцену, отлавливать события Key и Mouse
  pDX->Work(pFuncCallExit);
  delete pDX;
  pDX = NULL;
  return 0;
}
//---------------------------------------------
TLayerBetween_QT_DirectX::TLayerBetween_QT_DirectX()
{
  handle = NULL;
}
//---------------------------------------------
TLayerBetween_QT_DirectX::~TLayerBetween_QT_DirectX()
{

}
//---------------------------------------------
bool TLayerBetween_QT_DirectX::StartDirectX(void *pFuncCallExit)
{
  DWORD ThreadId;
  handle = CreateThread(NULL,              // default security attributes
                        0,                 // use default stack size  
                        ThreadDirectX,     // thread function 
                        pFuncCallExit,     // argument to thread function 
                        0,                 // use default creation flags 
                        &ThreadId);         // returns the thread identifier 
  return (handle!=NULL);
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
