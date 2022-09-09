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
the "Tanks" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef BL_LogH
#define BL_LogH

#include <list>
#include "ILogConsole.h"
#include "BL_Conv.h" 
#include <stdarg.h>

class GBASELIB_EI TLogWriter
{
  friend class TLogManager;
  typedef std::list<ILogConsole*> TListConsole;
  // проверка разрешения работы
  bool checkEnabled_p();
  bool mIsLogThreadID;

public:
  // Создать и связать лог-объект с ключом
  // Параметры ключа задаются в секции [LOG] конфигурационного файла приложения (см. комментарии в ini-файле).
  // По умолчанию запись в лог-файл заблокирована.
  TLogWriter( const char* key = NULL );
  ~TLogWriter();

  // Связать лог-объект с указанной консолью вывода данных
  void setConsole( const char* name );

  // Связать лог-объект с указанной консолью вывода данных
  void setConsole( ILogConsole *console );

  // Добавить в лог-объект связь с указанными консолями
  void addConsole( ILogConsole *console );
  void addConsole( const TListConsole &list );

  // Задать имя лог-объекта 
  // Параметры ключа задаются в секции [LOG] конфигурационного файла приложения (см. комментарии в ini-файле).
  void setName( const char* key );
  const char* name() const { return mName; }
  
  // Уведомление об отключении консоли
  void disableConsole( ILogConsole* console );

  // сбросить текущие настройки объекта
  void reset();

public: // Управление временными метками записей
  enum 
  { 
    tsDate = (1<<0), // дата
    tsTime = (1<<1), // время (с точностью до секунд)
    tsMS   = (1<<2), // миллисекундные метки
    
    tsDateTime = tsDate | tsTime,
    tsTimeMS   = tsTime | tsMS,
  };
  // Задать формат временных меток записей
  // Например: 
  //    tsTime     - только время 
  //    tsDateTime - дата и время
  //    tsTimeMS   - время с точностью до миллисекунд
  void markTimeStamp( int timestamp );

  // Выводить ли идентификатор потока, из которого вызывается функция логирования
  void markThreadId( bool flag );

private:
  #ifdef _MSC_VER
    # pragma warning(push)
    # pragma warning( disable : 4251 )  
      TListConsole  mListConsole;
    # pragma warning(pop)
  #else
    TListConsole  mListConsole;
  #endif
  
  /// Название "писателя"
	char* mName;

  /// Разрешение записи
	bool mEnable;
  
  /// Флаг необходимости перевода курсора на новую строку после вывода
  bool mEOL;

  /// Флаг необходимости перевода курсора на новую строку после вывода
  int  mTimestamp;

  /// Рассылка строки по всем консолям
  void write( const char* st, int len = -1 );

  /// Получить строку с временной меткой
  int  getStrTime( char* inOutStr );

  /// Получить строку с идентификатором потока
  int  getStrThreadID( char* inOutStr );

public:
  /// проверка разрешения работы
  bool checkEnabled();

  // приостановить вывод данных в лог
  void suspend()  { mEnable = false; }
  // восстановить вывод данных в лог
  void resume()   { mEnable = true; }

  // Флаг необходимости перевода курсора на новую строку после вывода
  // Значение по умолчанию - true
  void setEOL( bool value ) { mEOL = value; }
  bool getEOL()const        { return mEOL; }

  // вывод строки
  // cp - code page (см. BL_Conv.h)
  void printf( const char* cp, const char* format, ... );
  void vprintf( const char* cp, const char* format, va_list arglist );

  // вывод блока данных в HEX формате - не будет работать с UTF-16!!!
  // cp - code page (см. BL_Conv.h)
  // comment - комментарий к блоку
  // buf, len - адрес и размер блока данных
  void hex( const char* cp, const char* comment, const void* buf, size_t len );

public: // Обертки для сообщений в кодировке CP1251

  void printfW( const char* format, ... );
  void hexW( const char* comment, const void* buf, size_t len )
  { hex( BL_CP_WIN, comment, buf, len ); }

public: // Обертки для сообщений в кодировке ANSI
  void printfA( const char* format, ... );
  void hexA( const char* comment, const void* buf, size_t len )
  { hex( bl_locale, comment, buf, len ); }
};
//-------------------------------------------------------------------------
// проверка разрешения работы
inline bool TLogWriter::checkEnabled()
{
  if( !mEnable ) return false;

  if( !mListConsole.empty() )
    return true;
  
  //if( !mName ) return false;

  return checkEnabled_p();
}
//-------------------------------------------------------------------------
inline void TLogWriter::printf( const char* cp, const char* format, ... )
{
  va_list arglist;
  va_start( arglist, format );
  if( checkEnabled() )
    vprintf( cp, format, arglist );
  va_end( arglist );
}
//-----------------------------------------------------------------------------
inline void TLogWriter::printfW( const char* format, ... )
{
  va_list arglist;
  va_start( arglist, format );
  if( checkEnabled() )
    vprintf( BL_CP_WIN, format, arglist );
  va_end( arglist );
}
//-----------------------------------------------------------------------------
inline void TLogWriter::printfA( const char* format, ... )
{
  va_list arglist;
  va_start( arglist, format );
  if( checkEnabled() )
    vprintf( bl_locale, format, arglist );
  va_end( arglist );
}
//-----------------------------------------------------------------------------

typedef TLogWriter TBL_Log;

//-------------------------------------------------------------------------
#define APP_LOG_NAME  "APP"

class GBASELIB_EI TBL_LogApp : public TBL_Log
{
public:
  TBL_LogApp() : TBL_Log( APP_LOG_NAME ) {}
};

// Единый лог-файл приложения
extern GBASELIB_EI TBL_LogApp AppLog;

//-------------------------------------------------------------------------
//
#define BL_Log      AppLog.printfW
#define BL_LogHEX   AppLog.hexW

//-------------------------------------------------------------------------
void GBASELIB_EI BL_LogTo( const char* cp, const char* key, const char* format, va_list arglist );
void GBASELIB_EI BL_LogToA( const char* key, const char* format, ... );
void GBASELIB_EI BL_LogToW( const char* key, const char* format, ... );
//-------------------------------------------------------------------------


#endif // BL_LogH
