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


#include "BL_Thread.h"
#include "ErrorReg.h"
#include "HiTimer.h"
               
#if defined TD_WINDOWS
  #include <windows.h>
#else
  #include <pthread.h>
#endif

//-----------------------------------------------------------------------------
TBL_Thread::TBL_Thread( const char* name, GThreadPriority priority ) //: GCS(name)
{
  mThread = NULL;
  mPriority = priority;
  mIsRunned = false;
  mErrSource = err_RegisterSource( name );
}
//-----------------------------------------------------------------------------
TBL_Thread::~TBL_Thread(void)
{
  Stop();                                                    
}
//-----------------------------------------------------------------------------
gpointer TBL_Thread_threadFunc( gpointer data )
{
  ((TBL_Thread*)data)->Engine();
  return NULL;
}
//-----------------------------------------------------------------------------
bool TBL_Thread::Start()
{
  if( mIsRunned ) return true;

  mDoDone = false;
  mIsPrepared = false;
  mIsSuccessfulStarted = false;
  GError* error = NULL;
  mThread = g_thread_create_full( 
              PARAM(func,TBL_Thread_threadFunc), 
              PARAM(data,this),
              PARAM(stack_size, 0), 
              PARAM(joinable, true), 
              PARAM(bound, false),
              PARAM(priority, mPriority ),
              PARAM(error, &error) );
  if( !mThread || error )
  {
    g_clear_error( &error );
    errStr_SETSOURCE( "Неудача при создании потока", mErrSource );
  }
  else
  {
    // Дожидаемся завершения функции подготовки потока
    while( !mIsPrepared )
      ht_msleep( 1 );
    if( mIsSuccessfulStarted )
      return true;
  }
  OnDestroy();
  return false;
}
//-----------------------------------------------------------------------------
void TBL_Thread::Stop()
{
  if( mThread ) 
  {
    DoStop();                 // Инициировать завершение потока
    g_thread_join( mThread ); // Дождаться завершение потока
    mThread = NULL;
  }
}
//-----------------------------------------------------------------------------
void TBL_Thread::Engine()
{
  if( DoPrepare() )
  {
    mIsSuccessfulStarted = true; // Поток считается успешно запущенным
    mIsRunned   = true; // Поток успешно запущен
    mIsPrepared = true; // Подготовка завершена
    DoWork();
  }
  else 
  {
    mIsPrepared = true; // Подготовка завершена
  }
  //mThread   = NULL; // Поток умрет сам
  mIsRunned = false;
  OnDestroy(); // должна быть последней функцией потока !
}
//-----------------------------------------------------------------------------
guint TBL_Thread::CurThreadId()
{
  #if defined TD_WINDOWS
    return (guint32)GetCurrentThreadId();
  #else
    return pthread_self();
  #endif
}
//-----------------------------------------------------------------------------
