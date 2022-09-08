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

