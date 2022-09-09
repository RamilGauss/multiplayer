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
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#include "BL_AppFile.h"
#include <glib.h>
#include <string.h>

static const char* argv_0; // название приложения (локальная кодировка)
static const char* suffix;

//-----------------------------------------------------------------------------
static void CheckInit()
{
  if( !argv_0 )
  {
    argv_0 = g_get_prgname();
    if( !argv_0 )
      argv_0 = "undefined";
  }
}
//-----------------------------------------------------------------------------
void BL_AppFile_Init( const char* _argv_0, const char* _suffix )
{
  argv_0 = _argv_0;
  suffix = _suffix;
}
//-----------------------------------------------------------------------------
// Полное имя приложения 
const char* BL_AppFile()
{
  CheckInit();
  return argv_0;
}
//-----------------------------------------------------------------------------
// Директория расположения исполняемого файла приложения
// buffer - буфер размещения результата
// Результат: buffer
char* BL_AppFile_Dir( char* buffer )
{
  CheckInit();

  gchar* dn = g_path_get_dirname( argv_0 );
  strcpy( buffer, dn );
  g_free( dn );

  return buffer;
}
//-----------------------------------------------------------------------------
// Сформировать полное имя файла или директории относительно директории
// расположения исполняемого файла приложения
// buffer - буфер размещения результата
// ...    - набор строк с элементами пути или названия. Последним элементом обязательно
//          должно быть значение NULL
// Результат: buffer
char* BL_AppDir_Append( char* buffer, ... )
{
  gchar *str;
  enum { maxCount = 10 };
  gchar *arrStr[maxCount];

  va_list args;
  va_start (args, buffer);

  arrStr[0] = BL_AppFile_Dir(buffer);
  int i = 1;
  while( i < maxCount-1 )
  {
    arrStr[i] = va_arg (args, gchar *);
    if( !arrStr[i++] ) break;
  }
  arrStr[i-1] = NULL;
  #if GLIB_CHECK_VERSION(2,8,0)
  str = g_build_pathv( G_DIR_SEPARATOR_S, arrStr );
  #else
  if( i == 1 )      str = g_build_path( G_DIR_SEPARATOR_S, arrStr[0], NULL );
  else if( i == 2 ) str = g_build_path( G_DIR_SEPARATOR_S, arrStr[0], arrStr[1], NULL );
  else if( i == 3 ) str = g_build_path( G_DIR_SEPARATOR_S, arrStr[0], arrStr[1], arrStr[2], NULL );
  else if( i == 4 ) str = g_build_path( G_DIR_SEPARATOR_S, arrStr[0], arrStr[1], arrStr[2], arrStr[3], NULL );
  else if( i == 5 ) str = g_build_path( G_DIR_SEPARATOR_S, arrStr[0], arrStr[1], arrStr[2], arrStr[3], arrStr[4], NULL );
  else              str = g_build_path( G_DIR_SEPARATOR_S, arrStr[0], arrStr[1], arrStr[2], arrStr[3], arrStr[4], NULL );
  #endif
  va_end (args);

  strcpy( buffer, str );
  g_free( str );
  return  buffer;
}
//-----------------------------------------------------------------------------
// Сформировать имя файла такое-же как и у приложения, но с расширением <ext>
char* BL_AppFile_Ext( char* buffer, const char* ext, bool withSuffix )
{
  CheckInit();

  strcpy( buffer, argv_0 );
  size_t len = strlen( buffer );
  while( len-- )
  {
    switch( buffer[len] )
    {
      case '.':
      {
        buffer[len] = '\0';
        goto l_stop;
      }
      case '\\':
      case '/':
        goto l_stop;
      default:;
    }
  }
l_stop:

  if( withSuffix && suffix )
    strcat( buffer, suffix );
  return strcat( buffer, ext );
}
//-----------------------------------------------------------------------------

