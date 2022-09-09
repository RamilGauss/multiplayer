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


#include "TankTower.h"
#include "HiTimer.h"
#include "BL_Debug.h"
#include "Struct3D.h"

#pragma warning(disable: 4305)

using namespace std;
using namespace nsStruct3D;

const char* NameTrackR = "TrackR";
const char* NameTrackL = "TrackL";

const char* ParamTime = "Time";
const char* ParamaVelocity = "Velocity";

TTankTower::TTankTower()
{
  mIndexTrackR = 0;
  mIndexTrackL = 0;
  mIndexTime = 0;
  mIndexVelocity = 0;

  //------------------------------------------------
  // свойства по-умолчанию, временно, пока нет БД
  mProperty.mID_tank = 0;

  mTower = 0; // номер, 0 - базовая (сток)
  mGun = 0;

  mProperty.mSpeedRotateTower  = 0.2f;
  mProperty.mSpeedReductionGun = 0.1f;

  mProperty.mVMaxGunUgol = 15.0f/180.0f*M_PI;  // вверх, рад
  mProperty.mVMinGunUgol = -5.0f/180.0f*M_PI;  // вниз, рад
  mProperty.mHMaxGunUgol = 0.0f;  // по часовой стрелке, рад
  mProperty.mHMinGunUgol = 0.0f;  // против часовой стрелке, рад
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
  //D3DXMATRIXA16 matrix;
  //D3DXMatrixIdentity(&matrix);
  //D3DXMatrixRotationZ(&matrix, ugol);

  TMatrix16 matrix;
  SetMatrixIdentity(&matrix);
  SetMatrixRotateZ(&matrix,ugol);

  mTree.ChangeMatrix(string("Turret"),&matrix);
  mTree.GetMatrix(&mVectorMatrix);
}
//------------------------------------------------------------------------
void TTankTower::RotateVerticalGun(float ugol)
{
  // двигаем пушкой
  //D3DXMATRIXA16 matrix;
  //D3DXMatrixIdentity(&matrix);
  //D3DXMatrixRotationX(&matrix, ugol);
  TMatrix16 matrix;
  SetMatrixIdentity(&matrix);
  SetMatrixRotateX(&matrix,ugol);

  mTree.ChangeMatrix(string("Gun"),&matrix);
  mTree.GetMatrix(&mVectorMatrix);
}
//------------------------------------------------------------------------
void TTankTower::SetHuman(char* pData, int size)
{
  
}
//------------------------------------------------------------------------
void TTankTower::SetupShaderStackModelGE()
{
  float time_ms = (float)ht_GetMSCount(); 
  time_ms /= 1000.0f;

  SetShaderStackMask(&mShaderStackMask);// настроить маску
  // настроить сам шейдерный стек
  SetupShaderStack(mIndexTrackL,mIndexTime,    &time_ms,sizeof(time_ms));
  SetupShaderStack(mIndexTrackL,mIndexVelocity,&mV,     sizeof(mV));
  SetupShaderStack(mIndexTrackR,mIndexTime,    &time_ms,sizeof(time_ms));
  SetupShaderStack(mIndexTrackR,mIndexVelocity,&mV,     sizeof(mV));

  //mModel->SetShaderStackMask(&mShaderStackMask);// настроить маску
  //// настроить сам шейдерный стек
  //TShaderStack* pSS = mModel->GetShaderStack(mIndexTrackL);
  //pSS->SetData(mIndexTime,    &time_ms,sizeof(time_ms));// что R что L - тот же самый шейдер
  //pSS->SetData(mIndexVelocity,&mV,     sizeof(mV));
  //pSS = mModel->GetShaderStack(mIndexTrackR);
  //pSS->SetData(mIndexTime,    &time_ms,sizeof(time_ms));// что R что L - тот же самый шейдер
  //pSS->SetData(mIndexVelocity,&mV,     sizeof(mV));
}
//------------------------------------------------------------------------------------------------
void TTankTower::EventSetModelGE()
{
  int cnt = mVectorNamePart.size();
  for(int i = 0 ; i < cnt ; i++)
  {
    if(mVectorNamePart.at(i).name.compare(NameTrackR)==0)
      mIndexTrackR = i;
    if(mVectorNamePart.at(i).name.compare(NameTrackL)==0)
      mIndexTrackL = i;
  }
  //-----------------------------------------------------
  mIndexTime        = GetShaderStackIndexByName(mIndexTrackR,ParamTime);
  mIndexVelocity    = GetShaderStackIndexByName(mIndexTrackR,ParamaVelocity);
  //TShaderStack* pSS = mModel->GetShaderStack(mIndexTrackR);
  //mIndexTime        = pSS->GetIndexByName(ParamTime);
  //mIndexVelocity    = pSS->GetIndexByName(ParamaVelocity);

  cnt  = mVectorNamePart.size();
  for( int i = 0 ; i < cnt ; i++)
    mShaderStackMask.push_back(0);
  mShaderStackMask.at(mIndexTrackR) = 1;
  mShaderStackMask.at(mIndexTrackL) = 1;
}
//------------------------------------------------------------------------------------------------