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

#include "CallBackRegistrator.h"
#include <stddef.h>
#include "memory_operation.h"
#include <string.h>
#include "BL_Debug.h"


TCallBackRegistrator::TCallBackRegistrator()
{
	mArrCallBack = NULL;
	mCntCallBack = 0;
}
//--------------------------------------------------------------
TCallBackRegistrator::~TCallBackRegistrator()
{

}
//--------------------------------------------------------------
void TCallBackRegistrator::Register(TCallBackFunc pFunc)
{
	if(!InArr(pFunc))
		AddElement(pFunc);
	else
		BL_FIX_BUG();
}
//--------------------------------------------------------------
void TCallBackRegistrator::Unregister(TCallBackFunc pFunc)
{
	for(int i = 0 ; i < mCntCallBack ; i++)
	{
		if(mArrCallBack[i]==pFunc)
    {
      RemoveInArr(i);
      return;
    }
	}
  BL_FIX_BUG();
}
//--------------------------------------------------------------
bool TCallBackRegistrator::InArr(TCallBackFunc pFunc)
{
	for(int i = 0; i< mCntCallBack ; i++)
	{
		if(pFunc==mArrCallBack[i])
			return true;
	}
	return false;
}
//--------------------------------------------------------------
void TCallBackRegistrator::AddElement(TCallBackFunc pFunc)
{
	int size = mCntCallBack*sizeof(TCallBackFunc);

	mArrCallBack = (TCallBackFunc(*))mo_realloc(mArrCallBack, size, size+sizeof(TCallBackFunc));

	mArrCallBack[mCntCallBack] = pFunc;
	mCntCallBack++;
}
//--------------------------------------------------------------
void TCallBackRegistrator::RemoveInArr(int index)
{
	if(mCntCallBack==1){mCntCallBack=0;return;}

	if(mCntCallBack-1==index) {mCntCallBack--;return;}

	memmove(mArrCallBack+index,mArrCallBack+index+1,sizeof(TCallBackFunc)*(mCntCallBack-index-1));
	mCntCallBack--;
}
//--------------------------------------------------------------
void TCallBackRegistrator::Notify(void* data, int size)
{
	for(int i = 0 ; i < mCntCallBack ; i++)
	{
		TCallBackFunc pFunc = mArrCallBack[i];
		pFunc(data,size);
	}
}
//--------------------------------------------------------------