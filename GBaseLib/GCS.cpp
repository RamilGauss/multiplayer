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
the "Tanks" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#include "GCS.h"
#include "BL_Debug.h"

//===========================================================================
#ifdef TD_WINDOWS //=========================================================
//===========================================================================

#include <windows.h>

#define UNIT  ((CRITICAL_SECTION*)unit)
//---------------------------------------------------------------------------
GCS::GCS( const char *_dbgname )
{
  mDbgName = _dbgname;
  unit = new CRITICAL_SECTION;
  InitializeCriticalSection( UNIT );
}
//---------------------------------------------------------------------------
GCS::~GCS()
{
#ifdef _DEBUG
  if( TryEnterCriticalSection(UNIT) )
    LeaveCriticalSection(UNIT);
  else
    BL_FIX_BUG();
#endif
  DeleteCriticalSection(UNIT);
  delete UNIT;
}
//---------------------------------------------------------------------------
void GCS::lock()
{
  EnterCriticalSection(UNIT);
}
//---------------------------------------------------------------------------
bool GCS::tryLock()    
{ 
  return TryEnterCriticalSection(UNIT) != FALSE; 
}
//---------------------------------------------------------------------------
void GCS::unlock()
{
  LeaveCriticalSection(UNIT);
}
//===========================================================================
#else //TD_WINDOWS //========================================================
//===========================================================================

GCS::GCS( const char * _dbgname )
{
  mDbgName = _dbgname;
  pthread_mutexattr_t mutexAttr;
  pthread_mutexattr_settype( &mutexAttr, PTHREAD_MUTEX_RECURSIVE_NP );
  pthread_mutex_init( &unit, &mutexAttr );
}
//---------------------------------------------------------------------------
GCS::~GCS()
{
#ifdef _DEBUG
  if( tryLock() )
    unlock();
  else
    BL_FIX_BUG();
#endif
  pthread_mutex_destroy( &unit );
}
//---------------------------------------------------------------------------
//===========================================================================
#endif //TD_WINDOWS //=======================================================
//===========================================================================

