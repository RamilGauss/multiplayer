/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012
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


#ifndef BL_AppFileH
#define BL_AppFileH


#include "TypeDef.h"

//-----------------------------------------------------------------------------
// Инициализация BL_AppFile.
// argv_0 - значение передаваемого в приложение argv[0]
// suffix - произвольная строка добавляемая к собственно имени файла приложения
//          Предназначено для формирования различных производных имен файлов
//          формируемых функцией BL_AppFile_Ext()
GBASELIB_EI void BL_AppFile_Init( const char* argv_0, const char* suffix = 0 );

//-----------------------------------------------------------------------------
// Полное имя приложения 
GBASELIB_EI const char* BL_AppFile();

//-----------------------------------------------------------------------------
// Директория расположения исполняемого файла приложения
// buffer - буфер размещения результата
// Результат: buffer
GBASELIB_EI char* BL_AppFile_Dir( char* buffer );

//-----------------------------------------------------------------------------
// Сформировать полное имя файла или директории относительно директории
// расположения исполняемого файла приложения
// buffer - буфер размещения результата
// ...    - набор строк с элементами пути или названия. Последним элементом обязательно
//          должно быть значение NULL
// Результат: buffer
GBASELIB_EI char* BL_AppDir_Append( char* buffer, ... );

//-----------------------------------------------------------------------------
// Сформировать имя файла такое-же как и у приложения, но с с суффиксом
// <BL_AppFile_Init::suffix> и расширением <ext>
// withSuffix - флаг добавления к имени файла суффикса, заданного в BL_AppFile_Init::suffix
GBASELIB_EI char* BL_AppFile_Ext( char* buffer, const char* ext, bool withSuffix = true );

//-----------------------------------------------------------------------------

#endif

