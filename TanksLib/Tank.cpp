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


#include "Tank.h"
#include "TypeTank.h"


TTank::TTank()
{
  mProperty.mID_tank = 0;
  pRoom = NULL;

  mTower = 0; // номер, 0 - базовая (сток)
  mGun = 0;

}
//------------------------------------------------------------------------
TTank::~TTank()
{

}
//------------------------------------------------------------------------
void TTank::SetTypeTank(unsigned int id_tank)
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
unsigned int TTank::GetTypeTank()
{
  return mProperty.mID_tank;
}
//------------------------------------------------------------------------
TClient* TTank::GetMasterClient()
{
  return pMasterClient;
}
//------------------------------------------------------------------------
void TTank::SetMasterClient(TClient* _pClient)
{
  pMasterClient = _pClient;
}
//------------------------------------------------------------------------
int TTank::GetSizeProperty()
{
  int size = sizeof(TProperty)+sClientName.length();
  return size;
}
//------------------------------------------------------------------------
char* TTank::GetProperty()
{
  int size = GetSizeProperty();
  char* pData = new char[size];
  // заполнить данными
  *((TProperty*)pData) = mProperty;
  memcpy(pData+sizeof(TProperty),sClientName.data(),size-sizeof(TProperty));
  return pData;
}
//------------------------------------------------------------------------
void TTank::SetProperty(char* pData,int size)
{
  mProperty = *((TProperty*)pData);
  sClientName.insert(0,pData+sizeof(TProperty),size-sizeof(TProperty));
}
//------------------------------------------------------------------------

