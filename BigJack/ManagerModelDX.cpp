/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
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


#include "ManagerModelDX.h"
#include <d3d9.h>
#include "ModelDX.h"
#include "GlobalParamsTank.h"
#include "LoadFromHDD.h"
#include "LoggerDX.h"
#include "BL_Debug.h"
#include "LoaderListModel.h"
#include <atlconv.h>


TManagerModelDX::TManagerModelDX()
{
  mArrPathModel = NULL;
  mCntPathModel = 0;
}
//--------------------------------------------------------------------------------------
TManagerModelDX::~TManagerModelDX()
{
  Done();
}
//--------------------------------------------------------------------------------------
void TManagerModelDX::Load(IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
             void* pUserContext )
{
  if(LoadListPath()==false)
  {
    GlobalLoggerDX.WriteF_time("Не удалось загрузить список моделей.\n");
    BL_FIX_BUG();
    return;
  }

  for(int i = 0 ; i < mCntPathModel ; i++)
  {
    TModelDX* pModel = new TModelDX();
    USES_CONVERSION;
    pModel->Init(pd3dDevice,A2W(mArrPathModel[i]));
    mArrModel.Add(pModel);
  }
}
//--------------------------------------------------------------------------------------
void TManagerModelDX::ResetDevice()
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
void TManagerModelDX::OnLostDevice()
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
void TManagerModelDX::OnDestroyDevice()
{
  int cnt = mArrModel.Count();
  for(int i = 0 ; i < cnt ; i++)
  {
    TModelDX* pModel = (TModelDX*)mArrModel.Get(i);
    if(pModel)
      pModel->Destroy();
  }
  mArrModel.Clear();
}
//--------------------------------------------------------------------------------------
bool TManagerModelDX::LoadListPath()
{
  TLoaderListModel loader;
  if(loader.Open(PATH_LIST_MODELS)==false)
    return false;

  if(loader.Load( &mArrPathModel, mCntPathModel)==false)
    return false;

  return true;
}
//--------------------------------------------------------------------------------------
void TManagerModelDX::Done()
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
TModelDX* TManagerModelDX::Find(unsigned int id)
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
//--------------------------------------------------------------------------------------
