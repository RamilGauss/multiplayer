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

#include "IBaseObject.h"

#include "ManagerBaseObject.h"
#include <map>
#include "BL_Debug.h"

using namespace nsStruct3D;
using namespace std;

static TManagerBaseObject mManagerObject;

IBaseObject::IBaseObject()
{
  mPtrInherits = NULL;
  ID_model = 0;
  ID_map = 0;
  mV = 0;
  mA = 0;

  VectorIdentity(&mWorld);
  //D3DXMatrixIdentity(&mWorld);
}
//------------------------------------------------------------------------------------------------
IBaseObject::~IBaseObject()
{
  int cnt = mVectorMatrix.size();
  for(int i = 0 ; i < cnt ; i++)
    delete mVectorMatrix[i];

  mVectorMatrix.clear();
  delete pLoadedTree;
  pLoadedTree = NULL;
}
//------------------------------------------------------------------------------------------------
void IBaseObject::SetState(vector<unsigned char>* state)
{
  BL_ASSERT(mState.size()==state->size());
  mState = *state;
}
//------------------------------------------------------------------------------------------------
void IBaseObject::SetTree(TTreeJoint::TLoadedJoint* pTree)
{
  delete pLoadedTree;
  pLoadedTree = new TTreeJoint::TLoadedJoint;
  *pLoadedTree = *pTree;// копируем структуру
}
//------------------------------------------------------------------------------------------------
// считываем данные либо из СУБД либо с GUI -
// считать с объекта информацию о частях, 
// выставить использование на первой неповторяющейся части и задать в окне в виде CheckBox
void IBaseObject::SetMapUse(map<string, int>* mapUse)
{
  if(mapUse)
    mMapUse = *mapUse;
  mTree.Setup(pLoadedTree,&mMapUse);// инициализация внутренностей иерархии
  mTree.SetOrderMatrixByName(&mVectorOrderPart);// в таком порядке будут строиться матрицы

  SetDefaultMatrix();// в соответствии с деревом настроить матрицы по-умолчанию
  SetupState();// теперь известно сколько элементов
  SetupMask();// известно кол-во и порядок следования элементов
}
//------------------------------------------------------------------------------------------------
void IBaseObject::SetDefaultMatrix() 
{
  int cnt = mTree.GetCountPart();
  for(int i = 0 ; i < cnt ; i++) 
  {
    //D3DXMATRIXA16* mIdentity = new D3DXMATRIXA16;
    //D3DXMatrixIdentity((D3DXMATRIX*)mIdentity);
    TVector4_4* pIdentity = new TVector4_4;
    VectorIdentity(pIdentity);

    mVectorMatrix.push_back(pIdentity);
  }
  mTree.GetMatrix(&mVectorMatrix);
}
//------------------------------------------------------------------------------------------------
void IBaseObject::SetupState()
{
  int cnt = mTree.GetCountPart();
  if(cnt==mState.size()) return;
  //------------------------------------
  mState.clear();
  for(int i = 0 ; i < cnt ; i++ )
    mState.push_back(1);// по-умолчанию
}
//------------------------------------------------------------------------------------------------
void IBaseObject::SetupMask()
{
  BL_ASSERT(mVectorNamePart.size());// сначала должна быть проинициализирована модель!
  //-------------------------------------------------------------------------------
  int cntMask = mVectorNamePart.size();
  mMask.clear();
  for(int i = 0 ; i < cntMask ; i++ )
    mMask.push_back(0);// размер маски должен быть таким же как все части модели
  //-----------------------------------------------
  int sumMask = 0;
  // mMask - маска
  // mapUse - имя - номер, не сортированный
  // mVectorNamePart - перечисление имен частей, сортированный
  for(int i = 0 ; i < cntMask ; i++)
  {
    TPart part = mVectorNamePart.at(i);
    // заполнить часть маски для данного имени
    map<string,int>::iterator fit = mMapUse.find(part.name);
    if(fit->second==part.use)
    {
      mMask.at(i) = 1;
      sumMask++;
    }
  }
  BL_ASSERT(sumMask==mVectorOrderPart.size());
}
//------------------------------------------------------------------------------------------------
void IBaseObject::SetID_Model(unsigned int id)
{
  ID_model = id;
  mManagerObject.AddObject(this);
}
//------------------------------------------------------------------------------------------------
int IBaseObject::GetCountPart(const char* name,vector<string>* pVec)
{
  int found = 0;
  int cnt = pVec->size();
  for(int i = 0 ; i < cnt ; i++)
  {
    if(pVec->at(i).compare(name)==0)
      found++;
  }
  return found;
}
//------------------------------------------------------------------------------------------------
void IBaseObject::GetDefaultMapUse(map<string,int>* mapUse)
{
  *mapUse = mMapUse;
}
//------------------------------------------------------------------------------------------------
void IBaseObject::SetupDefaultMapUse()
{
  mMapUse.clear();
  int cnt = mVectorOrderPart.size();
  for(int i = 0 ; i < cnt ; i++)
  { 
    string name = mVectorOrderPart.at(i);
    map<string,int>::value_type val(name,0);
    mMapUse.insert(val);
  }
  SetMapUse();
}
//------------------------------------------------------------------------------------------------
void IBaseObject::RegisterOnEvent(TCallBackRegistrator::TCallBackFunc pFunc)
{
  mCallBackEvent.Register(pFunc);
}
//------------------------------------------------------------------------------------------------
void IBaseObject::UnregisterOnEvent(TCallBackRegistrator::TCallBackFunc pFunc)
{
  mCallBackEvent.Unregister(pFunc);
}
//------------------------------------------------------------------------------------------------
void IBaseObject::Notify(int event)
{
  mCallBackEvent.Notify(&event,sizeof(event));
}
//------------------------------------------------------------------------------------------------
void IBaseObject::SetWorld(TVector4_4* world)
{
  mWorld=*world;Notify(eWorld);
}
//------------------------------------------------------------------------------------------------
