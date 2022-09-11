/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
2011, 2012, 2013, 2013, 2013
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
you may contact in writing [ramil2085@mail.ru, ramil2085@mail.ru, ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 

#include <stddef.h>
#include <memory.h>

#include "ContainerRise.h"

TContainerRise::TContainerRise()
{
  mSizeUse = 0;
}
//----------------------------------------------------------
char* TContainerRise::GetPtr()
{
	return (char*)mC.GetPtr();
}
//----------------------------------------------------------
int TContainerRise::GetSize()
{
  return mSizeUse;
}
//----------------------------------------------------------
void TContainerRise::Alloc(int new_size)
{
  if(mC.GetSize() < new_size)
    mC.SetData(NULL, new_size);
  mSizeUse = new_size;
}
//----------------------------------------------------------
void TContainerRise::Realloc( int new_size)
{
  if(mC.GetSize() < new_size)
  {
    int oldSize = mC.GetSize();
    if(oldSize)
    {
      void* pOld = mC.GetPtr();
			mC.Unlink();
			mC.SetData(NULL, new_size);
			void* pNew = mC.GetPtr();
			memcpy(pNew, pOld, mSizeUse);
			TContainer::Free((char*)pOld);
    }
  }
  mSizeUse = new_size;
}
//----------------------------------------------------------
void TContainerRise::SetData(char* p, int size)
{
	if(size>mSizeUse)
		Alloc(size);
	else
		mSizeUse = size;
  if(p)
	  memcpy(mC.GetPtr(), p, mSizeUse);
}
//----------------------------------------------------------
void TContainerRise::AddData(char*p, int size)
{
  int oldSize = GetSize();
  int newSize = oldSize + size;
  Realloc(newSize);
  char* pBound = GetPtr();
  memcpy(pBound + oldSize, p, size);
}
//----------------------------------------------------------