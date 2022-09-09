/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012
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
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 
#define _USE_MATH_DEFINES

#include <cmath>

#include "IActor.h"
//#include "TypeTank.h"


IActor::IActor()
{
  pRoom = NULL;

}
//------------------------------------------------------------------------
IActor::~IActor()
{

}
//------------------------------------------------------------------------
void IActor::SetType(unsigned int id)
{
  //mProperty.mID_tank = id;

  //switch(mProperty.mID_tank)
  //{
  //  case nsTank_ID::eID_KingTiger:
  //    ID_model = 0;
  //    break;
  //}
}
//------------------------------------------------------------------------
unsigned int IActor::GetType()
{
  return 0;//mProperty.mID_tank;
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
//int IActor::GetSizeProperty()
//{
//  int size = sizeof(TProperty)+sClientName.length();
//  return size;
//}
////------------------------------------------------------------------------
//char* IActor::GetProperty()
//{
//  int size = GetSizeProperty();
//  char* pData = new char[size];
//  // заполнить данными
//  *((TProperty*)pData) = mProperty;
//  memcpy(pData+sizeof(TProperty),sClientName.data(),size-sizeof(TProperty));
//  return pData;
//}
////------------------------------------------------------------------------
//void IActor::SetProperty(char* pData,int size)
//{
//  mProperty = *((TProperty*)pData);
//  sClientName.insert(0,pData+sizeof(TProperty),size-sizeof(TProperty));
//}
////------------------------------------------------------------------------

