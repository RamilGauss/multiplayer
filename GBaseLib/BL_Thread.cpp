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
