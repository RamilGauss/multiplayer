/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012, 2013
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


#ifndef GTimeH
#define GTimeH

#include <time.h>   // Вставка обязательно!!! В <glib/gdate.h> декларируется struct tm
                    // вне namespace std
#include <glib.h>

//------------------------------------------------------------------------------
#if !defined(MS2US)
  // Перевести милисекунды в микросекунды
  #define MS2US( n )    ( (n) * 1000 )
  // Перевести секунды в милисекунды
  #define SEC2MS( n )   ( (n) * 1000 )
  // Перевести секунды в микросекунды
  #define SEC2US( n )   MS2US( SEC2MS(n) )
#endif
//------------------------------------------------------------------------------

// Вычисление длительности интервала между двумя временными метками, мс
inline glong gtime_Interval( const GTimeVal &begin, const GTimeVal &end )
{
  glong delay = (end.tv_sec - begin.tv_sec) * 1000 +
                (end.tv_usec - begin.tv_usec) / 1000;
  return delay;
}
//------------------------------------------------------------------------------
// Вычисление длительности интервала между текущим временем и временной меткой, мс
inline glong gtime_Interval( const GTimeVal &begin )
{
  GTimeVal end;
  g_get_current_time( &end );
  return gtime_Interval( begin, end );
}
//------------------------------------------------------------------------------
// Сравнение двух времен
// Результат:
// 1: t1 > t2
// -1: t1 < t2
// 0: t1 == t2
inline int gtime_compare( const GTimeVal &t1, const GTimeVal &t2 )
{
  return  ( t1.tv_sec > t2.tv_sec ) ? 1 :
          ( t1.tv_sec < t2.tv_sec ) ? -1 :
          ( t1.tv_usec > t2.tv_usec ) ? 1 :
          ( t1.tv_usec < t2.tv_usec ) ? -1 :
          0;
}
//------------------------------------------------------------------------------
// Получить время сдвинутое от текущего на заданную величину
// shift      - величину сдвига, мкс
// res [OUT]  - искомое время
inline void gtime_ShiftFromCurrent( GTimeVal &res, glong shift )
{
  g_get_current_time( &res );
  g_time_val_add( &res, shift );
}
//------------------------------------------------------------------------------
// Превышение заданного временного порога
inline bool gtime_IsFinished( GTimeVal &val )
{
  GTimeVal cur;
  g_get_current_time( &cur );
  return gtime_compare( cur, val ) >= 0;
}
//------------------------------------------------------------------------------

#endif
