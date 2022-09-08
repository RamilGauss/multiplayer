#ifndef BL_TimeH
#define BL_TimeH

#include "TypeDef.h"
#include <time.h>

// Количество миллисекунд, прошедшее с 1 января 1970 г.
typedef gint64 mtime_t;

//---------------------------------------------------------------------
// Получить текущее системное время 
GBASELIB_EI mtime_t mt_sysTime();

// Получить текущее программное время (с коррекцией)
GBASELIB_EI mtime_t mt_time();

// текущее программное время
inline time_t bl_time()  { return (time_t)( mt_time()/1000 ); }
//---------------------------------------------------------------------
// Преобразовать time_t в mtime_t 
inline mtime_t mt_fromTime_t( time_t time ) { return time * 1000;}

// Преобразовать mtime_t в time_t
inline time_t  mt_toTime_t( mtime_t time )  { return (time_t)( time / 1000); }

// Преобразовать mtime_t в tm
GBASELIB_EI tm mt_toTm( mtime_t curTime );
//---------------------------------------------------------------------
// Выполнить подстройку программного времени под точное значение
// val - время подстраиваемое по приемнику СЕВ
GBASELIB_EI void mt_correctByPrecision( mtime_t val );

// Скорректировать программное время
// delta - величина коррекции, сек
GBASELIB_EI void bl_time_correct( int delta );

//---------------------------------------------------------------------
// Преобразование из локального времени во время по Гринвичу и обратно
time_t GBASELIB_EI time_LocalToGMT( time_t t );
time_t GBASELIB_EI time_GMTToLocal( time_t t );

// Входит ли указанное локальное время в летний сезон
// Если время не задано, то подразумевается текущее время
bool GBASELIB_EI IsSummerTime( time_t *curTime = 0 );

//------------------------------------------------------------------------------
// скорректировать системное время по программному
void GBASELIB_EI bl_time_to_system();
//------------------------------------------------------------------------------

#endif
