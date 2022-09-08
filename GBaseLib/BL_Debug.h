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


#ifndef BL_DebugH
#define BL_DebugH

#include "TypeDef.h"

// Вывести на экран сообщение об отказе.
GBASELIB_EI void BL_MessageBug( const char* lpszFileName, int nLine );

#ifdef _DEBUG
  // Проверить выражение <f> и вывести на экран сообщение об отказе в случае его ложности
  #define BL_ASSERT(f)          (void) ( (f) || (BL_MessageBug(__FILE__, __LINE__),0) )

  // Выполнить действие описываемое выражением <f>
  #define BL_DEBUG(f)           f

  // Вывести на экран сообщение об отказе. Глюк уже наступил
  #define BL_FIX_BUG()          BL_MessageBug(__FILE__, __LINE__)

#else
  #define BL_ASSERT(f)          
  #define BL_DEBUG(f)           
  #define BL_FIX_BUG()
#endif  //_DEBUG


#endif  //BL_DebugH

