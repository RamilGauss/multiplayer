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
the "Tanks" Source Code.  If not, please request a copy in writing from id Software at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#include "ObjectPrediction.h"
#include <memory.h>

TObjectPrediction::TObjectPrediction(unsigned char type)
{
  mType = type;
  pUserData = NULL;
  mUserDataSize = 0;
  mArrDef = NULL;
  mCntDef = 0;
}
//--------------------------------------------------------------------------
TObjectPrediction::~TObjectPrediction()
{
  CleanUserData();
  CleanArrDef();
}
//--------------------------------------------------------------------------
unsigned short TObjectPrediction::GetID()
{
  return mID;
}
//--------------------------------------------------------------------------
void TObjectPrediction::SetID(unsigned short id)
{
  mID = id;
}
//--------------------------------------------------------------------------
unsigned int TObjectPrediction::GetMaskState()
{
  return mMaskState;
}
//--------------------------------------------------------------------------
void TObjectPrediction::SetUserData(char* pData, int size)
{
  CleanUserData();
  mUserDataSize = size;
  pUserData = new char[size];
  memcpy(pUserData,pData,mUserDataSize);
}
//--------------------------------------------------------------------------
void TObjectPrediction::GetUserData(char* pData, int& size)
{
  memcpy(pData,pUserData,mUserDataSize);
}
//--------------------------------------------------------------------------
void TObjectPrediction::CleanUserData()
{
  delete[] pUserData;
  pUserData = NULL;
  mUserDataSize = 0;
}
//--------------------------------------------------------------------------
void TObjectPrediction::CleanArrDef()
{
  delete []mArrDef;
  mArrDef = NULL;
  mCntDef = 0;
}
//--------------------------------------------------------------------------