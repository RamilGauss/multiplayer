/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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

#include "BaseObjectDX.h"
#include <d3d9.h>

using namespace nsStruct3D;
using namespace std;

TBaseObjectDX::TBaseObjectDX(int typeDX)
{
  flgShow = true;// ������� �� ������ �� �����
  mTimeCreation = 0;
  mModel = NULL;
  pLoadedTree = NULL;
  mAlphaTransparency = 1.0f;

  mTypeDX = typeDX;// ��-��������� �� ����� �������������
}
//------------------------------------------------------------------------------------------------
TBaseObjectDX::~TBaseObjectDX()
{
  Done();
}
//------------------------------------------------------------------------------------------------
void TBaseObjectDX::Draw(D3DXMATRIXA16* mView)
{          
  if(flgShow==false) return;

  // ��������� ��������� ����
  SetupShaderStackModelDX();

  mModel->Draw(&mState,             //                       (�� ObjectDX)
               &mMask,
               &mVectorMatrix,//���-�� ��������� � cSubset       (�� ObjectDX)
               &mWorld,// ��� � ��� ���������� ������        (�� ObjectDX)
               mAlphaTransparency,
               mView );// ������������ � ���������� ������    (�� ManagerDirectX)
}
//------------------------------------------------------------------------------------------------
void TBaseObjectDX::SetModel(TModelDX* pModel)
{
  mModel = pModel;
  // ������� ���� �� ������ � ������� ������
  SetupVectorNamePart();
  SetupVectorOrderPart();
  SetupDefaultMapUse();

  EventSetModelDX();
}
//------------------------------------------------------------------------------------------------
TModelDX* TBaseObjectDX::GetModel()
{
  return mModel;
}
//------------------------------------------------------------------------------------------------
void TBaseObjectDX::Done()
{
}
//------------------------------------------------------------------------------------------------
void TBaseObjectDX::SetupVectorNamePart()
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
void TBaseObjectDX::SetupVectorOrderPart()
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
