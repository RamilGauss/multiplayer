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

#include "Tank.h"
#include "TypeTank.h"


IActor::IActor()
{
  pRoom = NULL;


  // �������� ��-���������, ��������, ���� ��� ��
  mProperty.mID_tank = 0;

  mTower = 0; // �����, 0 - ������� (����)
  mGun = 0;

  mProperty.mSpeedRotateTower  = 0.2f;
  mProperty.mSpeedReductionGun = 0.1f;

  mProperty.mVMaxGunUgol = 15.0f/180.0f*M_PI;  // �����, ���
  mProperty.mVMinGunUgol = -5.0f/180.0f*M_PI;  // ����, ���
  mProperty.mHMaxGunUgol = 0.0f;  // �� ������� �������, ���
  mProperty.mHMinGunUgol = 0.0f;  // ������ ������� �������, ���
}
//------------------------------------------------------------------------
IActor::~IActor()
{

}
//------------------------------------------------------------------------
void IActor::SetTypeTank(unsigned int id_tank)
{
  mProperty.mID_tank = id_tank;

  switch(mProperty.mID_tank)
  {
    case nsTank_ID::eID_KingTiger:
      ID_model = 0;
      break;
  }
}
//------------------------------------------------------------------------
unsigned int IActor::GetTypeTank()
{
  return mProperty.mID_tank;
}
//------------------------------------------------------------------------
TClient* IActor::GetMasterClient()
{
  return pMasterClient;
}
//------------------------------------------------------------------------
void IActor::SetMasterClient(TClient* _pClient)
{
  pMasterClient = _pClient;
}
//------------------------------------------------------------------------
int IActor::GetSizeProperty()
{
  int size = sizeof(TProperty)+sClientName.length();
  return size;
}
//------------------------------------------------------------------------
char* IActor::GetProperty()
{
  int size = GetSizeProperty();
  char* pData = new char[size];
  // ��������� �������
  *((TProperty*)pData) = mProperty;
  memcpy(pData+sizeof(TProperty),sClientName.data(),size-sizeof(TProperty));
  return pData;
}
//------------------------------------------------------------------------
void IActor::SetProperty(char* pData,int size)
{
  mProperty = *((TProperty*)pData);
  sClientName.insert(0,pData+sizeof(TProperty),size-sizeof(TProperty));
}
//------------------------------------------------------------------------

