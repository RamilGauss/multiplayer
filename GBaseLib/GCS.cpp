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

