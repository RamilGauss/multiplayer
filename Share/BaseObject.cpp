/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
√удаков –амиль —ергеевич 
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
#define _USE_MATH_DEFINES

#include <cmath>


#include "BaseObject.h"
#include "ManagerBaseObject.h"

using namespace nsStruct3D;

static TManagerBaseObject mManagerObject;

TBaseObject::TBaseObject()
{
  mPtrInherits = NULL;
  ID_model = 0;
  ID_map = 0;
  SetOneMatrix(&mWorld);
}
//------------------------------------------------------------------------------------------------
TBaseObject::~TBaseObject()
{
  int cnt = mVectorMatrix.size();
  for(int i = 0 ; i < cnt ; i++)
    delete mVectorMatrix[i];

  mVectorMatrix.clear();
  delete pLoadedTree;
  pLoadedTree = NULL;
}
//------------------------------------------------------------------------------------------------
void TBaseObject::SetState(std::vector<unsigned char>* state)
{
  mState = *state;
}
//------------------------------------------------------------------------------------------------
D3DXMATRIXA16* TBaseObject::SetOneMatrix(D3DXMATRIXA16* matrix)
{
  matrix->_11 = 1;matrix->_12 = 0;matrix->_13 = 0;matrix->_14 = 0;
  matrix->_21 = 0;matrix->_22 = 1;matrix->_23 = 0;matrix->_24 = 0;
  matrix->_31 = 0;matrix->_32 = 0;matrix->_33 = 1;matrix->_34 = 0;
  matrix->_41 = 0;matrix->_42 = 0;matrix->_43 = 0;matrix->_44 = 1;
  return matrix;
}
//------------------------------------------------------------------------------------------------
void TBaseObject::SetTree(TTreeJoint::TLoadedJoint* pTree)
{
  delete pLoadedTree;
  pLoadedTree = new TTreeJoint::TLoadedJoint;
  *pLoadedTree = *pTree;// копируем структуру
  mTree.Setup(pLoadedTree);// инициализаци€ внутренностей иерархии
  mTree.SetOrderMatrixByName(&mVectorOrderPart);// в таком пор€дке будут строитьс€ матрицы

  SetDefaultMatrix();  
  SetupState();
  SetupMask();
}
//------------------------------------------------------------------------------------------------
void TBaseObject::SetDefaultMatrix() 
{
  int cnt = mTree.GetCountPart();
  for(int i = 0 ; i < cnt ; i++) 
    mVectorMatrix.push_back(SetOneMatrix(new D3DXMATRIXA16));
  mTree.GetMatrix(&mVectorMatrix);
/*
  // ####
  // убрать это после настройки Tree
  FLOAT Angle = float(M_PI);
  for(int j = 3 ; j < 7 ; j++)
  {
    D3DXMatrixRotationZ(mVectorMatrix[j],Angle );// track
    mVectorMatrix[j]->_43 = -1.08f;
  }

  D3DXMatrixRotationZ(mVectorMatrix[2],Angle );// 
  mVectorMatrix[2]->_43 = 0.987328f+0.367343f;// Gun
  mVectorMatrix[2]->_42 = 1.421399f-0.079083f;// Gun
  mVectorMatrix[2]->_41 = 0.064711f-0.010775f;// Gun
  //0,064711 0,367343 1,421399	

  mVectorMatrix[1]->_41 = -0.010775f;// Turret 0,987328 -0,079083
  mVectorMatrix[1]->_42 = -0.079083f;// Turret
  mVectorMatrix[1]->_43 = 0.987328f;// Turret
  //0 - Hull
  */
}
//------------------------------------------------------------------------------------------------
void TBaseObject::SetupState()
{
  int cnt = mVectorMatrix.size();
  for(int i = 0 ; i < cnt ; i++ )
    mState.push_back(1);// по-умолчанию
}
//------------------------------------------------------------------------------------------------
void TBaseObject::SetupMask()
{
  int cnt = mVectorMatrix.size();
  for(int i = 0 ; i < cnt ; i++ )
    mMask.push_back(1);
}
//------------------------------------------------------------------------------------------------
void TBaseObject::SetID_Model(unsigned int id)
{
  ID_model=id;
  mManagerObject.AddObject(this);
}
//------------------------------------------------------------------------------------------------