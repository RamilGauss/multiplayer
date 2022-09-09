/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012, 2013, 2013, 2013
===========================================================================
                        Common Information
"TornadoEngine" GPL Source Code

This file is part of the "TornadoEngine" GPL Source Code.

"TornadoEngine" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"TornadoEngine" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "TornadoEngine" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "TornadoEngine" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "TornadoEngine" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@mail.ru, ramil2085@mail.ru, ramil2085@mail.ru, ramil2085@gmail.com].
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
void IBaseObjectGE::Draw(const TMatrix16* mView, void* pEffect)
{          
  if(flgShow==false) return;

  // назначить шейдерный стек
  SetupShaderStackModelGE();

  mModel->Draw(&mVectorUseCubeMap,
		           &mState,             //                       
               &mMask,
               &mVectorMatrix,//кол-во совпадает с cSubset   
               &mWorld,       // где и как расположен объект 
               mAlphaTransparency,
               mView,// расположение и ориентация камеры   
               pEffect);
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
	// если используется cube map
	SetupVectorForCubeMap();
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
int IBaseObjectGE::GetCountPartForCubeMap()
{
	int nFind = 0;
	int cnt = mVectorUseCubeMap.size();
	for(int i = 0 ; i < cnt ; i++)
	{
		if(mVectorUseCubeMap[i])
			nFind++;
	}
	return nFind;
}
//------------------------------------------------------------------------------------------------
void* IBaseObjectGE::GetTextureForCubeMap(int index)
{
	int nFind = 0;
	int cnt = mVectorUseCubeMap.size();
	for(int i = 0 ; i < cnt ; i++)
	{
		if(mVectorUseCubeMap[i])
			nFind++;
		if(index==nFind-1) return mVectorUseCubeMap[i];
	}
	return NULL;
}
//------------------------------------------------------------------------------------------------
void IBaseObjectGE::SetupVectorForCubeMap()
{
	int cnt = mModel->GetCntEffect();
	for(int i = 0 ; i < cnt ; i++ )
		mVectorUseCubeMap.push_back(mModel->MakeTextureForCubeMap(i));
}
//------------------------------------------------------------------------------------------------
