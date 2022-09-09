/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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


#include "IBaseObjectPrediction.h"
#include <memory.h>
#include <stddef.h>

IBaseObjectPrediction::IBaseObjectPrediction()
{
  //mType = type;
  //pUserData = NULL;
  //mUserDataSize = 0;
  //mArrDef = NULL;
  //mCntDef = 0;
}
//--------------------------------------------------------------------------
IBaseObjectPrediction::~IBaseObjectPrediction()
{
  //CleanUserData();
  //CleanArrDef();
}
////--------------------------------------------------------------------------
//unsigned short IBaseObjectPrediction::GetID()
//{
//  return mID;
//}
////--------------------------------------------------------------------------
//void IBaseObjectPrediction::SetID(unsigned short id)
//{
//  mID = id;
//}
//--------------------------------------------------------------------------
//unsigned int IBaseObjectPrediction::GetMaskState()
//{
//  return mMaskState;
//}
////--------------------------------------------------------------------------
//void IBaseObjectPrediction::SetUserData(char* pData, int size)
//{
//  CleanUserData();
//  mUserDataSize = size;
//  pUserData = new char[size];
//  memcpy(pUserData,pData,mUserDataSize);
//}
////--------------------------------------------------------------------------
//void IBaseObjectPrediction::GetUserData(char* pData, int& size)
//{
//  memcpy(pData,pUserData,mUserDataSize);
//}
////--------------------------------------------------------------------------
//void IBaseObjectPrediction::CleanUserData()
//{
//  delete[] pUserData;
//  pUserData = NULL;
//  mUserDataSize = 0;
//}
////--------------------------------------------------------------------------
//void IBaseObjectPrediction::CleanArrDef()
//{
//  delete []mArrDef;
//  mArrDef = NULL;
//  mCntDef = 0;
//}
////--------------------------------------------------------------------------
void IBaseObjectPrediction::SetupVectorNamePart()
{
  
}
//--------------------------------------------------------------------------