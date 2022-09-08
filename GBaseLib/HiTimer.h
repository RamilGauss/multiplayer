/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012
===========================================================================
                        Common Information
"Tanks" GPL Source Code

This file is part of the "Tanks" GPL Source Code.

"Tanks" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"Tanks" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "Tanks" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "Tanks" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "Tanks" Source Code.  If not, please request a copy in writing from id Software at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef HiTimerH
#define HiTimerH

#include "TypeDef.h"      // Базовые определения типов

//------------------------------------------------------------------------------
// Модуль работы с таймером высокого разрешения
//------------------------------------------------------------------------------

// Изначальная инициализация
bool GBASELIB_EI ht_Init();

// Получение тиков процессора (с тактовой частотой процессора)
guint64 GBASELIB_EI ht_GetTickCount();

// Задержка на микросекунды
void GBASELIB_EI ht_usleep( guint32 us );

// Задержка на миллисекунды
void GBASELIB_EI ht_msleep( guint32 ms );

// Задержка на микросекунды c дополнительной проверкой состояния
// Результат: true - выход по результату func, false - выход по таймауту
typedef bool (*THT_CheckFunc)(void);
bool GBASELIB_EI ht_sleep( guint32 us, THT_CheckFunc func );

// Перевод микросекунд в тики
guint64 GBASELIB_EI ht_us2tick( guint32 us );

// Перевод тиков в микросекунд
guint32 GBASELIB_EI ht_tick2us( guint64 tick );

// Получение тиков процессора пересчитанных в микросекунды
inline guint32 ht_GetUSCount()
{
  return ht_tick2us( ht_GetTickCount() );
}
// Время в миллисекундах с момента запуска ЭВМ или программы.
// Точность от 55 мсек и выше
guint32 GBASELIB_EI ht_GetMSCount();
//------------------------------------------------------------------------------
#if !defined(MS2US)
  // Перевести миллисекунды в микросекунды
  #define MS2US( n )    ( (n) * 1000 )
  // Перевести секунды в миллисекунды
  #define SEC2MS( n )   ( (n) * 1000 )
  // Перевести секунды в микросекунды
  #define SEC2US( n )   MS2US( SEC2MS(n) )
#endif
//------------------------------------------------------------------------------

#endif
