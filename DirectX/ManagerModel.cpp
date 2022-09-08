#include "ManagerModel.h"
#include <d3d9.h>
#include "ModelDX.h"
#include "GlobalParamsTank.h"
#include "LoadFromHDD.h"
#include "LogerDX.h"
#include "BL_Debug.h"

TManagerModel GlobalManagerModel;

TManagerModel::TManagerModel()
{
  mArrPathModel = NULL;
  mCntPathModel = 0;
}
//--------------------------------------------------------------------------------------
TManagerModel::~TManagerModel()
{
  Done();
}
//--------------------------------------------------------------------------------------
void TManagerModel::Load(IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
             void* pUserContext )
{
  if(LoadListPath())
  {
    GlobalLoggerDX.WriteF_time("Не удалось загрузить список моделей.\n");
    BL_FIX_BUG();
    return;
  }

  for(int i = 0 ; i < mCntPathModel ; i++)
  {
    TModelDX* pModel = new TModelDX();
    pModel->Init(pd3dDevice,mArrPathModel[i]);
    mArrModel.Add(pModel);
  }
}
//--------------------------------------------------------------------------------------
void TManagerModel::ResetDevice()
{
  int cnt = mArrModel.Count();
  for(int i = 0 ; i < cnt ; i++)
  {
    TModelDX* pModel = (TModelDX*)mArrModel.Get(i);
    if(pModel)
      pModel->ResetDevice();
  }
}
//--------------------------------------------------------------------------------------
void TManagerModel::OnLostDevice()
{
  int cnt = mArrModel.Count();
  for(int i = 0 ; i < cnt ; i++)
  {
    TModelDX* pModel = (TModelDX*)mArrModel.Get(i);
    if(pModel)
      pModel->LostDevice();
  }
}
//--------------------------------------------------------------------------------------
void TManagerModel::OnDestroyDevice()
{
  int cnt = mArrModel.Count();
  for(int i = 0 ; i < cnt ; i++)
  {
    TModelDX* pModel = (TModelDX*)mArrModel.Get(i);
    if(pModel)
      pModel->Destroy();
  }
}
//--------------------------------------------------------------------------------------
bool TManagerModel::LoadListPath()//###
{
  TLoadFromHDD lfHDD;
  if(lfHDD.ReOpen(PATH_LIST_MODELS)==false)
    return false;
  
  //lfHDD.Read()
  return true;
}
//--------------------------------------------------------------------------------------
void TManagerModel::Done()
{
  for(int i = 0 ; i < mCntPathModel ; i++)
  {
    delete[]mArrPathModel[i];
    mArrPathModel[i] = NULL;
  }

  delete[] mArrPathModel;
  mArrPathModel = NULL;
  mCntPathModel = 0;

  mArrModel.Clear();
}
//--------------------------------------------------------------------------------------
TModelDX* TManagerModel::Find(unsigned int id)
{
  int cnt = mArrModel.Count();
  // скорость не имеет значения
  for(int i = 0 ; i < cnt ; i++ )
  {
    TModelDX* pModel = (TModelDX*)mArrModel.Get(i);
    if(pModel)
    {
      if(pModel->GetID()==id)
        return pModel;
    }
  }
  return NULL;
}
//--------------------------------------------------------------------------------------