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


#ifndef HiTimerH
#define HiTimerH

#include "TypeDef.h"

//------------------------------------------------------------------------------
// Модуль работы с таймером высокого разрешения
//------------------------------------------------------------------------------

// Получение тиков процессора (с тактовой частотой процессора)
unsigned __int64 SHARE_EI ht_GetUSCount();

// Задержка на микросекунды
void SHARE_EI ht_usleep( unsigned int us );

// Задержка на миллисекунды
void SHARE_EI ht_msleep( unsigned int ms );

// Задержка на микросекунды c дополнительной проверкой состояния
// Результат: true - выход по результату func, false - выход по таймауту
typedef bool (*THT_CheckFunc)(void);
bool SHARE_EI ht_sleep( unsigned int us, THT_CheckFunc func );

// Время в миллисекундах с момента запуска ЭВМ или программы.
// Точность от 55 мсек и выше
unsigned int SHARE_EI ht_GetMSCount();
//------------------------------------------------------------------------------

#endif
