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
the "Tanks" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 
#define _USE_MATH_DEFINES

#include <cmath>
#include "IBaseObjectGE.h"

#include "IModelGE.h"
#include "Struct3D.h"
#include "ShaderStack.h"

using namespace nsStruct3D;
using namespace std;

IBaseObjectGE::IBaseObjectGE(int typeDX)
{
  flgShow = true;// показан ли объект на сцене
  mTimeCreation = 0;
  mModel = NULL;
  pLoadedTree = NULL;
  mAlphaTransparency = 1.0f;

  mTypeGE = typeDX;// по-умолчанию не чисто анимированный
}
//------------------------------------------------------------------------------------------------
IBaseObjectGE::~IBaseObjectGE()
{
  Done();
}
//------------------------------------------------------------------------------------------------
//void IBaseObjectGE::Draw(D3DXMATRIXA16* mView)
void IBaseObjectGE::Draw(TMatrix16* mView)
{          
  if(flgShow==false) return;

  // назначить шейдерный стек
  SetupShaderStackModelGE();

  mModel->Draw(&mState,             //                       (От ObjectDX)
               &mMask,
               &mVectorMatrix,//кол-во совпадает с cSubset   (От ObjectDX)
               &mWorld,       // где и как расположен объект (От ObjectDX)
               mAlphaTransparency,
               mView );// расположение и ориентация камеры   (от ManagerDirectX)
}
//------------------------------------------------------------------------------------------------
void IBaseObjectGE::SetModel(IModelGE* pModel)
{
  mModel = pModel;
  // скинуть инфу об именах в базовый объект
  SetupVectorNamePart();
  SetupVectorOrderPart();
  SetupDefaultMapUse();

  EventSetModelGE();
}
//------------------------------------------------------------------------------------------------
IModelGE* IBaseObjectGE::GetModel()
{
  return mModel;
}
//------------------------------------------------------------------------------------------------
void IBaseObjectGE::Done()
{
}
//------------------------------------------------------------------------------------------------
void IBaseObjectGE::SetupVectorNamePart()
{
  int cnt = mModel->GetCntEffect();
  for(int i = 0 ; i < cnt ; i++)
  { 
    TPart part;
    part.name = mModel->GetNameByIndex(i);
    part.use  = mModel->GetNumUseByIndex(i);
    mVectorNamePart.push_back(part);
  }
}
//------------------------------------------------------------------------------------------------
void IBaseObjectGE::SetupVectorOrderPart()
{
  int cnt = mModel->GetCntEffect();
  for(int i = 0 ; i < cnt ; i++)
  {
    string name = mModel->GetNameByIndex(i);
    if(mVectorOrderPart.size()==0)
      mVectorOrderPart.push_back(name);
    else
    {
      int lastIndex = mVectorOrderPart.size()-1;
      if(mVectorOrderPart.at(lastIndex).compare(name)!=0)
        mVectorOrderPart.push_back(name);
    }
  }
}
//------------------------------------------------------------------------------------------------
void IBaseObjectGE::SetShaderStackMask(vector<int>* shaderStackMask)// настроить маску
{
  mModel->SetShaderStackMask(shaderStackMask);// настроить маску
}
//------------------------------------------------------------------------------------------------
void IBaseObjectGE::SetupShaderStack(int indexSS, int index, void* data,int size)
{
  TShaderStack* pSS = mModel->GetShaderStack(indexSS);
  pSS->SetData(index, data, size);
}
//------------------------------------------------------------------------------------------------
int IBaseObjectGE::GetShaderStackIndexByName(int index, const char* nameParam)
{
  TShaderStack* pSS = mModel->GetShaderStack(index);
  return pSS->GetIndexByName(nameParam);
}
//------------------------------------------------------------------------------------------------
