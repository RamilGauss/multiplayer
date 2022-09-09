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


#include <stddef.h>
#include "BL_Debug.h"
#include "LoaderDLLImpl.h"

#ifdef WIN32
  #pragma warning(disable:4800)
#else

#endif

TLoaderDLLImp::TLoaderDLLImp()
{
#ifdef WIN32
  hModule = NULL;
#else
#endif
}
//------------------------------------------------------------------
TLoaderDLLImp::~TLoaderDLLImp()
{
  Done();
}
//------------------------------------------------------------------
bool TLoaderDLLImp::Init(const char* sPath)
{
#ifdef WIN32
  hModule = LoadLibraryA(sPath);
  if(hModule==NULL)
  {
    BL_FIX_BUG();
    return false;
  }
#else
#endif
  return true;
}
//------------------------------------------------------------------
void* TLoaderDLLImp::Get(const char* nameFunc)
{
  void* ptrFunc = NULL;
#ifdef WIN32
  ptrFunc = GetProcAddress(hModule, nameFunc);
  if(ptrFunc==NULL)
    BL_FIX_BUG();
#else
#endif
  return ptrFunc;
}
//------------------------------------------------------------------
void TLoaderDLLImp::Done()
{
  bool res = false;
#ifdef WIN32
  res = (bool)FreeLibrary(hModule);
#else
#endif
  BL_ASSERT(res);
  hModule = NULL;
}
//------------------------------------------------------------------
