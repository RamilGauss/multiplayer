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

#ifdef _MSC_VER
# pragma warning(push)
# pragma warning( disable : 4996 )
#endif


#include "LogManager.h"
#include "BL_AppFile.h"
#include "LogFile.h"
#include "BL_ConfigFile.h"
#include <glib.h>
#include <string.h>

TAutoCreateVarT<TLogManager> LogManager;

#define GRP_NAME        "LOG"

#if TD_TARGET == TD_LINUX
  #define stricmp strcasecmp
#endif

#ifdef TD_WINDOWS
  #include <windows.h>
#include <list>
#endif

//-----------------------------------------------------------------------------
TLogManager::TLogManager()
{
#ifdef TD_WINDOWS
  if( !GetConsoleWindow() ) return;
#endif  
  Register( &mLogStdOut );
  Register( &mLogStdErr );
}
//-----------------------------------------------------------------------------
TLogManager::~TLogManager()
{
  for( TListConsole::iterator item = mConsoles.begin(); item != mConsoles.end(); )
  {
    const char* name = (*item)->name();
    if( name[0] != ':' )
    {
      delete *item;
      item = mConsoles.erase( item );
    }
    else
      item++;
  }
}
//-----------------------------------------------------------------------------
ILogConsole* TLogManager::Find( const char* name )
{
  for( TListConsole::const_iterator item = mConsoles.begin(); item != mConsoles.end(); item++ )
  {
    if( stricmp( (*item)->name(), name ) == 0 ) 
      return *item;
  }
  return NULL;
}
//-----------------------------------------------------------------------------
void TLogManager::Register(ILogConsole* console)
{
  lock();
  mConsoles.push_front( console );
  unlock();
}
//-----------------------------------------------------------------------------
void TLogManager::Unregister(ILogConsole* console)
{
  lock();
  for( TListWriter::const_iterator item = mWriters.begin(); item != mWriters.end(); item++ )
    (*item)->disableConsole( console );

//  for( TListConsole::const_iterator item = mConsoles.begin(); item != mConsoles.end(); item++ )
//    (*item)->disableConsole( console );

  mConsoles.remove( console );
  unlock();
}
//-----------------------------------------------------------------------------
void TLogManager::Link( TBL_Log* writer )
{
  if( !writer || !writer->name() ) return;

  lock();
  // Если список консолей "писателя" не пуст, то его инициализация была уже проведена.
  // Это повторная проверка готовности "писателя" внутри критической секции, 
  // защита от гонок при инициализации.
  if( !writer->mListConsole.empty() )
  {
    unlock();    
    return;
  }

  setGroupComment();
  char st[MAX_PATH];

  // Формирование значения ключа по умолчанию
  if( stricmp( writer->name(), APP_LOG_NAME ) == 0 )
  {
    // Для общего лог-файла приложения - <файла приложения>.log
    BL_AppFile_Ext( st, ".log" );
    char* bn = g_path_get_basename( st );
    if( bn )
    {
      strcpy( st, bn );         // в файл
      strcat( st, " :strerr" ); // и на экран
      g_free( bn );
    }
    else strcpy( st, "off" );
  }
  else strcpy( st, "off" );

  // Чтение параметров "писателя" из ini-файла
  char *consoleDesc = AppConfigFile->GetValue( GRP_NAME, writer->name(), st );
  parseDescConsole( writer, consoleDesc ? consoleDesc : "" );
  if( consoleDesc ) g_free(consoleDesc);
  mWriters.push_front( writer );
  unlock();
}
//-----------------------------------------------------------------------------
void TLogManager::parseDescConsole( TBL_Log* writer, const char* descConsole )
{
  ILogConsole* result = NULL;
	bool setOff = false;
  gchar** arrStr = g_strsplit( descConsole, " ", -1 );
  for( int i = 0; arrStr[i] && arrStr[i][0]; i++ )
  {
    char* item = arrStr[i];
    if( stricmp( item, "off") == 0 )
    {
      writer->suspend();
			setOff = true;
    }
    else
    {
      ILogConsole* console;
      if( item[0] == ':' ) // Это программная консоль
      {
        console = Find( item );
      }
      else   // Это файл-консоль
      {
        char st[MAX_PATH];
        // Формируем имя лог-файла из абсолютного или относительного имени
        if( g_path_is_absolute( item ) )
          strcpy( st, item );
        else
          BL_AppDir_Append( st, item, 0 );

        console = Find( st );
        if( !console )
        {
          console = new TLogFile( st, true );
          mConsoles.push_back( console );
        }
      }
			if( console )
			{
        writer->addConsole( console );
				result = console;
			}
    }
  }
  g_strfreev( arrStr );

	// консоль не задана и не задана блокировка
  if( !result && !setOff )
  {
    // Это общий лог приложения?
    if( stricmp( writer->name(), APP_LOG_NAME ) == 0 )
    { // Для лог-файла приложения:
      char st[MAX_PATH];
      BL_AppFile_Ext( st, ".log" );                    // имя по умолчанию
      result = new TLogFile( st, true );
      mConsoles.push_front( result );                  // регистрация
      writer->addConsole( result );
    }
    else 
    {
      AppLog.checkEnabled();            // инициируем общий лог приложения
      //result = AppLog.mConsole;         // цепляемся на его консоль 
      writer->addConsole( AppLog.mListConsole );
    }
  }
//  writer->mConsole = result;
//  return result != NULL;
}
//-----------------------------------------------------------------------------
void TLogManager::Link( TBL_Log* writer, const char* descConsole )
{
  if( !descConsole || descConsole[0]=='\0' ) return Link( writer );

  lock();
  parseDescConsole( writer, descConsole );
  mWriters.push_front( writer );
  unlock();
}
//-----------------------------------------------------------------------------
void TLogManager::Unlink( TBL_Log* writer )
{
  lock();
  mWriters.remove( writer );
  unlock();
}
//-----------------------------------------------------------------------------
ILogConsole* TLogManager::CheckConsole( const char* consoleName )
{
  ILogConsole* console;
  if( consoleName[0] == ':' ) // Это программная консоль
  {
    console = Find( consoleName );
  }
  else   // Это файл-консоль
  {
    char st[MAX_PATH];
    // Формируем имя лог-файла из абсолютного или относительного имени
    if( g_path_is_absolute( consoleName ) )
      strcpy( st, consoleName );
    else
      BL_AppDir_Append( st, consoleName, 0 );

    console = Find( st );
    if( !console )
    {
      console = new TLogFile( st, true );
      mConsoles.push_back( console );
    }
  }
  return console;
}
//-----------------------------------------------------------------------------
// Формирование комментария к секции конфигурационного файла
void TLogManager::setGroupComment()
{ 
  bool hasComment;
  {
    char *comment = AppConfigFile->GetComment( GRP_NAME, NULL );
    hasComment = comment && strlen(comment);
    g_free( comment );     
  }

  if( !hasComment ) // комментарий еще не задан
  {
    const char commentW[] = {
      "---------------------------------------------------------\n"
      "------    Секция управления потоками логирования   ------\n"
      "---------------------------------------------------------\n"
      "Структура параметров секции LOG: \n"
      "   <название потока>=<описание потока>, где \n"
      " \n"
      "Стандартные потоки логирования: \n"
      "   APP - общий лог-поток приложения. Если описание не задано,  \n"
      "         то подразумевается лог-файл с названием <имя приложения>.log \n"
      " \n"
      "<описание потока> = [off] [консоль] ..., где \n"
      "   off  - признак блокирования потока \n"
      "   консоль - название консоли вывода потока. Консолей может быть \n"
      "          указано несколько - поток будет выводиться на все. \n"
      "          Если название консоли начинается с символа ':' то это указывает \n"
      "          на \"специальную\" консоль - программный модуль. \n"
      "          Иначе название консоли интерпретируется как имя лог-файла. \n"
      "          Имя лог-файла может быть с полным путем к файлу или с путем \n"
      "          относительно каталога запуска приложения.   \n"
      "          Если имя не задано, то подразумевается консоль, назначенная  \n"
      "          для стандартного потока <APP>.\n"
      " \n"
      "Стандартные консоли: \n"
      "   :infoLine - информационная консоль приложения (для GUI приложений) \n"
      "   :stdout   - системная консоль вывода (по умолчанию - монитор) \n"
      "   :stderr   - системная консоль отказов(по умолчанию - монитор) \n"
    };
    gsize bytes_written;
    gchar* commentL = g_convert( commentW, -1, bl_locale, BL_CP_WIN, NULL, &bytes_written, NULL );
    if( commentL )
    {
      AppConfigFile->SetComment( GRP_NAME, NULL, commentL );
      g_free( commentL );
    }
  }
}
//-----------------------------------------------------------------------------

#ifdef _MSC_VER
# pragma warning(pop)
#endif
