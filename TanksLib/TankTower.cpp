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

#include "TankTower.h"
#include "HiTimer.h"

using namespace std;


TTankTower::TTankTower()
{
  //mVectorOrderPart.push_back("Hull");
  //mVectorOrderPart.push_back("Turret");
  //mVectorOrderPart.push_back("Gun");
  //mVectorOrderPart.push_back("ChassisR");
  //mVectorOrderPart.push_back("ChassisL");
  //mVectorOrderPart.push_back("TrackR");
  //mVectorOrderPart.push_back("TrackL");
}
//------------------------------------------------------------------------
TTankTower::~TTankTower()
{

}
//------------------------------------------------------------------------
bool TTankTower::GetMirror(char ** pData,int &size)
{

  return false;
}
//------------------------------------------------------------------------
void TTankTower::SetMirror(char *pData,int size)
{

}
//-----------------------------------------------------------------------
bool TTankTower::Animate(guint32 time_ms)
{
  return true;
}
//------------------------------------------------------------------------
void TTankTower::RotateTurret(float ugol)
{
  // вращаем башню
  D3DXMATRIXA16 matrix;
  D3DXMatrixIdentity(&matrix);
  D3DXMatrixRotationZ(&matrix, ugol);

  mTree.ChangeMatrix(string("Turret"),&matrix);
  mTree.GetMatrix(&mVectorMatrix);
}
//------------------------------------------------------------------------
void TTankTower::RotateVerticalGun(float ugol)
{
  // двигаем пушкой
  D3DXMATRIXA16 matrix;
  D3DXMatrixIdentity(&matrix);
  D3DXMatrixRotationX(&matrix, ugol);

  mTree.ChangeMatrix(string("Gun"),&matrix);
  mTree.GetMatrix(&mVectorMatrix);
}
//------------------------------------------------------------------------
void TTankTower::SetHuman(char* pData, int size)
{
  
}
//------------------------------------------------------------------------
