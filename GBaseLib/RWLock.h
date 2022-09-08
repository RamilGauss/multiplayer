#ifndef RWLockH
#define RWLockH

#include <glib/gthread.h>

class TRWLock
{
  GStaticRWLock mRWL;

public:
  const char* dbgname;

  TRWLock(const char* _dbgname)     
  { 
    dbgname = _dbgname;
    g_static_rw_lock_init( &mRWL ); 
  }
  ~TRWLock(void)    { g_static_rw_lock_free(                  &mRWL ); }
  
public:
  // Блокировки на чтение
  void rlock()      { g_static_rw_lock_reader_lock(           &mRWL ); }
  bool rlock_try()  { return g_static_rw_lock_reader_trylock( &mRWL ) ? true : false; }
  void runlock()    { g_static_rw_lock_reader_unlock(         &mRWL ); }

  // Блокировки на запись
  void wlock()      { g_static_rw_lock_writer_lock(           &mRWL ); }
  bool wlock_try()  { return g_static_rw_lock_writer_trylock( &mRWL ) ? true : false; }
  void wunlock()    { g_static_rw_lock_writer_unlock(         &mRWL ); }
};

#endif //#ifndef RWLockH