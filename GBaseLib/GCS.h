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
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef GCSH
#define GCSH

#include "TypeDef.h"

#if TD_TARGET == TD_LINUX
  #ifndef __USE_UNIX98
    #define __USE_UNIX98
  #endif
  #include <pthread.h>
#endif

//---------------------------------------------------------------------------
// Класс объекта "Критическая секция"
// Защита от 
class GBASELIB_EI GCS
{
#ifdef TD_WINDOWS
  void *unit;
#else  
  pthread_mutex_t unit;
#endif

  const char* mDbgName;
public:
  GCS( const char * _dbgname = NULL );
  ~GCS();

  void setDbgName( const char *_dbgname ) { mDbgName = _dbgname; }
  const char * dbgName() { return mDbgName; }

  bool tryLock();
  void lock();
  void unlock();
};

//---------------------------------------------------------------------------

// Класс автоматического блокиратора
// Обеспечивает вход в критическую секцию при создании объекта
// и выход из нее при удалении объекта.
// Предназначен для создания объектов на стеке
class TBL_Locker
{
  GCS* mCS;
public:
  TBL_Locker( GCS* cs )     { mCS = cs; mCS->lock(); }

  ~TBL_Locker()             { done(); }

  void done()               { if( mCS ) { mCS->unlock(); mCS = NULL; } }
};

#define AUTO_LOCK( cs )     TBL_Locker locker( cs )
//===========================================================================
#ifndef TD_WINDOWS //========================================================
//===========================================================================

inline void GCS::lock()
{
  pthread_mutex_lock( &unit );
}
//---------------------------------------------------------------------------
inline bool GCS::tryLock()    
{ 
  return pthread_mutex_trylock( &unit ) == 0; 
}
//---------------------------------------------------------------------------
inline void GCS::unlock()
{
  pthread_mutex_unlock( &unit );
}
//===========================================================================
#endif // TD_WINDOWS //======================================================
//===========================================================================


#endif // GCSH

