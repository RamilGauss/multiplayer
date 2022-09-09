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


#include "BaseObjectPrediction.h"
#include <memory.h>
#include <stddef.h>

TBaseObjectPrediction::TBaseObjectPrediction()
{
  //mType = type;
  //pUserData = NULL;
  //mUserDataSize = 0;
  //mArrDef = NULL;
  //mCntDef = 0;
}
//--------------------------------------------------------------------------
TBaseObjectPrediction::~TBaseObjectPrediction()
{
  //CleanUserData();
  //CleanArrDef();
}
////--------------------------------------------------------------------------
//unsigned short TBaseObjectPrediction::GetID()
//{
//  return mID;
//}
////--------------------------------------------------------------------------
//void TBaseObjectPrediction::SetID(unsigned short id)
//{
//  mID = id;
//}
//--------------------------------------------------------------------------
//unsigned int TBaseObjectPrediction::GetMaskState()
//{
//  return mMaskState;
//}
////--------------------------------------------------------------------------
//void TBaseObjectPrediction::SetUserData(char* pData, int size)
//{
//  CleanUserData();
//  mUserDataSize = size;
//  pUserData = new char[size];
//  memcpy(pUserData,pData,mUserDataSize);
//}
////--------------------------------------------------------------------------
//void TBaseObjectPrediction::GetUserData(char* pData, int& size)
//{
//  memcpy(pData,pUserData,mUserDataSize);
//}
////--------------------------------------------------------------------------
//void TBaseObjectPrediction::CleanUserData()
//{
//  delete[] pUserData;
//  pUserData = NULL;
//  mUserDataSize = 0;
//}
////--------------------------------------------------------------------------
//void TBaseObjectPrediction::CleanArrDef()
//{
//  delete []mArrDef;
//  mArrDef = NULL;
//  mCntDef = 0;
//}
////--------------------------------------------------------------------------
void TBaseObjectPrediction::SetupVectorNamePart()
{
  
}
//--------------------------------------------------------------------------