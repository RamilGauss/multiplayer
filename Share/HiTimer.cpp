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

#include "HiTimer.h"

#ifdef WIN32
  #include <windows.h>
#endif

#include <boost/thread/thread.hpp>
#include <boost/chrono/include.hpp>
#include <boost/chrono/time_point.hpp>

using namespace boost;

//------------------------------------------------------------------------------
unsigned __int64 ht_GetUSCount()
{
  typedef chrono::process_real_cpu_clock type_clock;

  type_clock::time_point t = type_clock::now();
  return t.time_since_epoch().count();
}
//------------------------------------------------------------------------------
// Задержка на миллисекунды
void ht_msleep( unsigned int ms )
{
	// в силу того что под Windows XP boost кидается ассертом (см. chrono)
#ifdef WIN32
	Sleep(ms);
#else
	chrono::milliseconds time_sleep(ms);
  this_thread::sleep_for( time_sleep );
#endif
}
//------------------------------------------------------------------------------
unsigned int ht_GetMSCount()
{
  typedef chrono::process_real_cpu_clock type_clock;

  type_clock::time_point t = type_clock::now();
  return (unsigned int)(t.time_since_epoch().count()/1000000);
}
//------------------------------------------------------------------------------
// Задержка на микросекунды
void ht_usleep( unsigned int us )
{
  bool ht_usleep( unsigned int us, THT_CheckFunc func );
  ht_usleep( us, (THT_CheckFunc)NULL );
}
//------------------------------------------------------------------------------
// Задержка на микросекунды c дополнительной проверкой состояния
// Результат: true - выход по результату func, false - выход по таймауту
bool ht_usleep( unsigned int us, THT_CheckFunc func )
{
  unsigned __int64 start  = ht_GetUSCount();
  unsigned __int64 finish = start + us;
  for( ; ht_GetUSCount() < finish; )
    if( func )
      if( func() )
        return true;
  return false;
}
//------------------------------------------------------------------------------

