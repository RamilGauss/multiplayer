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

#include "CallBackRegistrator.h"
#include <stddef.h>
#include "memory_operation.h"
#include <string.h>
#include "BL_Debug.h"
#include <stdio.h>


using namespace std;

TCallBackRegistrator::TCallBackRegistrator()
{
}
//--------------------------------------------------------------
TCallBackRegistrator::~TCallBackRegistrator()
{
  int size = mSetCallback.size();
  BL_ASSERT(size==0);
  mSetCallback.clear();
}
//--------------------------------------------------------------
void TCallBackRegistrator::Register(TCallBackFunc pFunc)
{
  mSetCallback.insert(pFunc);
}
//--------------------------------------------------------------
void TCallBackRegistrator::Unregister(TCallBackFunc pFunc)
{
  set<TCallBackFunc>::iterator fit = mSetCallback.find(pFunc);
  set<TCallBackFunc>::iterator eit = mSetCallback.end();
  if(fit!=eit)
    mSetCallback.erase(fit);
  else
    BL_FIX_BUG();
}
//--------------------------------------------------------------
void TCallBackRegistrator::Notify(void* data, int size)
{
  set<TCallBackFunc>::iterator bit = mSetCallback.begin();
  set<TCallBackFunc>::iterator eit = mSetCallback.end();
  while(bit!=eit)
	{
		TCallBackFunc pFunc = (*bit);
		pFunc(data,size);
    bit++;
	}
}
//--------------------------------------------------------------
