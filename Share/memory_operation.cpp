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
#include "memory_operation.h"
#include <malloc.h>
#include <memory.h>
#include "BL_Debug.h"


void* mo_realloc(void* old_mem, int old_size, int new_size)
{
  if(old_size)
  {
    void* volatileBuffer = malloc(old_size);
    memcpy(volatileBuffer,old_mem,old_size);
    free(old_mem);
    old_mem = malloc( new_size);
    memcpy(old_mem,volatileBuffer,old_size);
    free(volatileBuffer);
  }
  else
    old_mem = malloc(new_size);

  return old_mem;
}
//--------------------------------------------------------------------------
char* mo_realloc_bound(char* old_mem, int old_size, int size_bound, int size_paste)
{
  if(old_size==0) BL_FIX_BUG();

  char* new_mem = (char*)malloc(old_size+size_paste);
  memcpy(new_mem, old_mem,size_bound);
  memcpy(new_mem+size_bound+size_paste,old_mem+size_bound,old_size-size_bound);
  free(old_mem);
  return new_mem;
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
void* mo_realloc_new(void* old_mem, int old_size, int new_size)
{
  if(old_size)
  {
    void* volatileBuffer = new char[old_size];
    memcpy(volatileBuffer,old_mem,old_size);
    delete[](char*)old_mem;
    old_mem = new char[new_size];
    memcpy(old_mem,volatileBuffer,old_size);
    delete[](char*)volatileBuffer;
  }
  else
    old_mem = new char[new_size];

  return old_mem;
}
//--------------------------------------------------------------------------
char* mo_realloc_bound_new(char* old_mem, int old_size, int size_bound, int size_paste)
{
  if(old_size==0) BL_FIX_BUG();

  char* new_mem = new char[old_size+size_paste];
  memcpy(new_mem, old_mem,size_bound);
  memcpy(new_mem+size_bound+size_paste,old_mem+size_bound,old_size-size_bound);
  delete[]old_mem;
  return new_mem;
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
