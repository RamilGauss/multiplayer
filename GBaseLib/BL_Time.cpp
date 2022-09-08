#include "HiTimer.h"
#include "BL_Time.h"

#if TD_TARGET == TD_LINUX
  #include <sys/time.h>
  #define _daylight __daylight  
  #define _timezone __timezone
#else
  #define STRICT
  #include <windows.h>
#endif
//------------------------------------------------------------------------------

// смещение программного времени относительно системного, мс
static int volatile shift_time_ms;

//------------------------------------------------------------------------------
// Входит ли указанное локальное время в летний сезон
bool IsSummerTime( time_t *pTime )
{
#if 0
  return _daylight != 0;
#else
  static int isSummerTime = -1;

  if( isSummerTime == -1 )
  {
    time_t t = pTime ? *pTime : time(0);
    isSummerTime = ( localtime(&t)->tm_isdst > 0 ) ? 1 : 0;
  }
  return isSummerTime != 0;
#endif
}
//------------------------------------------------------------------------------
time_t time_LocalToGMT( time_t t )
{
  t += _timezone;
  if( IsSummerTime(&t) )
    t -= 3600; 
  return t;
}

time_t time_GMTToLocal( time_t t )
{
  t -= _timezone;
  if( IsSummerTime(&t) ) 
    t += 3600; 
  return t; 
}
//------------------------------------------------------------------------------
void bl_time_to_system()
{
  if (  shift_time_ms == 0 ) return;
#if TD_TARGET == TD_LINUX                 // УСТАНОВКА СИСТЕМНЫХ ЧАСОВ
  struct timeval tv;
  tv.tv_sec = bl_time();
  tv.tv_usec = 0;
  settimeofday(&tv, NULL);
#else
/*  SYSTEMTIME st;                        // под Виндой используем WinAPI
  GetLocalTime(&st);
  QDateTime DT;
  DT.setTime_t( pu_time() );
  st.wHour         = (unsigned short)DT.time().hour();
  st.wMinute       = (unsigned short)DT.time().minute();
  st.wSecond       = (unsigned short)DT.time().second();
  st.wMilliseconds = 0;
  SetLocalTime(&st);   */
#endif
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//-------------------------------------------------------------------
// Получить системное время 
mtime_t mt_sysTime()
{
#ifndef G_OS_WIN32
  struct timeval result;

  /*this is required on alpha, there the timeval structs are int's
    not longs and a cast only would fail horribly*/
  gettimeofday (&result, NULL);
  return (mtime_t)result.tv_sec*1000 + result.tv_usec / 1000;

#else
  FILETIME ft;
  guint64 *time64 = (guint64 *) &ft;

  GetSystemTimeAsFileTime (&ft);

  /* Convert from 100s of nanoseconds since 1601-01-01
   * to Unix epoch. Yes, this is Y2038 unsafe.
   */
  *time64 -= G_GINT64_CONSTANT (116444736000000000);
  return *time64 / 10/ 1000;
#endif
}
//-------------------------------------------------------------------
// Получить программное время 
mtime_t mt_time()
{ 
  static mtime_t prevResult;

  mtime_t result = mt_sysTime() + shift_time_ms;

  if( result < prevResult )
  {
    if( prevResult - result < 100 )
      return prevResult;
  }
  prevResult = result;
  return result;
}
//-------------------------------------------------------------------
// Преобразовать mtime_t в системное время 
tm mt_toTm( mtime_t curTime )
{
  time_t curTime_t = mt_toTime_t(curTime);
  return *localtime( &curTime_t );
}
//-------------------------------------------------------------------

// Выполнить подстройку под точное время
// val - время подстраиваемое по приемнику СЕВ
void mt_correctByPrecision( mtime_t real )
{
  int delta = int(real - mt_time());
  if( delta > 0 )
    //bl_time_delta_ms += delta >> 3; // + delta*1/8
    shift_time_ms += delta; // + delta*1/8
  else
  {
    delta = (-delta) >> 3;
    if( delta > 0 )
      shift_time_ms--;
  }
}
//-------------------------------------------------------------------
void bl_time_correct( int delta )    
{ 
  shift_time_ms += delta*1000; 
}
//-------------------------------------------------------------------
