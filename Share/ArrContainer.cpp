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
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 

#include "ArrContainer.h"


TArrContainer::TArrContainer()
{

}
//-----------------------------------------------------------------------------
TArrContainer::~TArrContainer()
{

}
//-----------------------------------------------------------------------------
void TArrContainer::GetElem( char*& p, int& size, int index) 
{
  p    = NULL;
  size = 0 ;
  if(int(mVec.size())<=index) return;
  p    = (char*)mVec[index].GetPtr();
  size = mVec[index].GetSize();
}
//-----------------------------------------------------------------------------
void TArrContainer::SetElem( char* p, int size, int index)
{
  if(int(mVec.size())<=index) return;
  mVec[index].SetData(p,size);
}
//-----------------------------------------------------------------------------
void TArrContainer::SetCount(int v)
{
  mVec.clear();
  for(int i = 0 ; i < v ; i++)
    mVec.push_back(TContainer());
}
//-----------------------------------------------------------------------------
int TArrContainer::GetCount()
{
  return mVec.size();
}
//-----------------------------------------------------------------------------
char* TArrContainer::GetPtr()
{
  Collect();
  return (char*)mFC.GetPtr();
}
//-----------------------------------------------------------------------------
int TArrContainer::GetSize()
{
  int size = 0;
  int cnt = mVec.size(); 
  for(int i = 0 ; i < cnt ; i++ )
    size += mVec[i].GetSize();

  return size;
}
//-----------------------------------------------------------------------------
void TArrContainer::Collect()
{
  int size = GetSize();
  mFC.SetData(NULL,size);
  char* p = (char*)mFC.GetPtr();
  int cnt = mVec.size(); 
  for(int i = 0 ; i < cnt ; i++ )
  {
    int sizeElem = mVec[i].GetSize();
    memcpy(p,mVec[i].GetPtr(),sizeElem);
    p += sizeElem;
  } 
}
//-----------------------------------------------------------------------------
