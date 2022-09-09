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
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 

#include "Container.h"
#include <memory.h>


TContainer::TContainer()
{
  pData = NULL;
  size  = 0;
}
//---------------------------------------------------------------
TContainer::~TContainer()
{
  Done();
}
//---------------------------------------------------------------
void TContainer::Done()
{
  delete[]pData;
  pData = NULL;
  size  = 0;
}
//---------------------------------------------------------------
void TContainer::SetData(char* p, int s)
{
  Done();
  size  = s;
  pData = new char[size];
  if(p)
    memcpy(pData,p,size);
}
//---------------------------------------------------------------
char* TContainer::GetData(int &s)
{
  s = size;
  return pData;
}
//---------------------------------------------------------------
void* TContainer::GetPtr()const
{
  return pData;
}
//---------------------------------------------------------------
int TContainer::GetSize()const
{
  return size;
}
//---------------------------------------------------------------
void TContainer::Unlink()
{
  pData = NULL;
  size = 0;
}
//---------------------------------------------------------------
