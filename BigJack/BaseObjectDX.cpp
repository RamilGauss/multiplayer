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


#include "BaseObjectDX.h"
#include <d3d9.h>

using namespace nsStruct3D;

TBaseObjectDX::TBaseObjectDX()
{
  SetOneMatrix(mWorld);
  
  mArrMatrix = NULL;
  mCntMatrix = 0;

  flgShow = true;// показан ли объект на сцене

  mModel = NULL;
}
//------------------------------------------------------------------------------------------------
TBaseObjectDX::~TBaseObjectDX()
{
  Done();
}
//------------------------------------------------------------------------------------------------
void TBaseObjectDX::Draw(D3DXMATRIXA16* mView,D3DXMATRIXA16* mProj)
{          
  if(flgShow==false) return;

  SetupArrMatrix();

  mModel->Draw(&mState,             //                       (От ObjectDX)
               &mMask,
               mArrMatrix,//кол-во совпадает с cSubset       (От ObjectDX)
               &mWorld,// где и как расположен объект        (От ObjectDX)
               mView, // расположение и ориентация камеры    (от ManagerDirectX)
               mProj);
}
//------------------------------------------------------------------------------------------------
void TBaseObjectDX::SetModel(TModelDX* pModel)
{
  mModel = pModel;
  mCntMatrix = mModel->GetCntEffect();
  mArrMatrix = new D3DXMATRIXA16[mCntMatrix];
  SetupArrMatrix();
  SetupState();
  SetupMask();
}
//------------------------------------------------------------------------------------------------
TModelDX* TBaseObjectDX::GetModel()
{
  return mModel;
}
//------------------------------------------------------------------------------------------------
void TBaseObjectDX::Done()
{
  delete[] mArrMatrix;
  mArrMatrix = NULL;
  mCntMatrix = 0;
}
//------------------------------------------------------------------------------------------------
void TBaseObjectDX::SetupArrMatrix()
{
  for(int i = 0 ; i < mCntMatrix ; i++ )
  {
    SetOneMatrix(mArrMatrix[i]);// по-умолчанию
  }
}
//------------------------------------------------------------------------------------------------
void TBaseObjectDX::SetupState()
{
  for(int i = 0 ; i < mCntMatrix ; i++ )
  {
    mState.push_back(1);// по-умолчанию
  }
}
//------------------------------------------------------------------------------------------------
void TBaseObjectDX::SetupMask()
{
  for(int i = 0 ; i < mCntMatrix ; i++ )
  {
    mMask.push_back(1);
  }
}
//------------------------------------------------------------------------------------------------
