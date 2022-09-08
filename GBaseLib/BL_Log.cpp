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


#include "BL_Log.h"
#include "BL_Time.h"
#include <glib.h>
#include "LogManager.h"
#include <string.h>
#include <stdarg.h>
#include "glib/gprintf.h"
#include "BL_Thread.h"


GBASELIB_EI TBL_LogApp AppLog;

//-----------------------------------------------------------------------------
TLogWriter::TLogWriter( const char* name )
{
  mName = name ? g_strdup( name ) : NULL;
  mEnable = true;
  mEOL = true;
  mTimestamp = 0;
  mIsLogThreadID = false;
}
//-----------------------------------------------------------------------------
TLogWriter::~TLogWriter()
{
  reset();
  LogManager->Unlink( this );
  if( mName ) { g_free( mName ); mName = NULL; }
}
//-----------------------------------------------------------------------------
void TLogWriter::reset()
{
  mListConsole.clear();
}
//-----------------------------------------------------------------------------
// Связать лог-объект с указанной консолью вывода данных
void TLogWriter::setConsole( const char* consoleName )
{
  reset();
  LogManager->Link( this, consoleName );
}
//-----------------------------------------------------------------------------
// Связать лог-объект с указанной консолью вывода данных
void TLogWriter::setConsole( ILogConsole *console )
{
  reset();
  mListConsole.push_back( console );
}
//-----------------------------------------------------------------------------
void TLogWriter::addConsole( ILogConsole *console )
{
  mListConsole.push_back( console );
}
//-----------------------------------------------------------------------------
void TLogWriter::addConsole( const TListConsole &list )
{
  for( TListConsole::const_iterator i = list.begin(); i != list.end(); i++ )
    mListConsole.push_back( (*i) );
}
//-----------------------------------------------------------------------------
// Задать имя лог-объекта 
void TLogWriter::setName( const char* name )
{
  reset();

  if( mName ) g_free( mName );

  if( name ) mName = g_strdup( name );
  else       mName = NULL;
}
//-----------------------------------------------------------------------------
// проверка разрешения работы
bool TLogWriter::checkEnabled_p()
{
  LogManager->Link( this );
  return !mListConsole.empty();
}
//-------------------------------------------------------------------------
// Рассылка строки по всем консолям
void TLogWriter::write( const char* st, int len )
{
  for( TListConsole::iterator i = mListConsole.begin(); i != mListConsole.end(); i++ )
    (*i)->write( st, len, mEOL );
}
//-------------------------------------------------------------------------
// Получить строку с временной меткой
int TLogWriter::getStrTime( char* inOutStr )
{
  int len = 0;
  mtime_t curTime = mt_sysTime();
  
  if( (mTimestamp & tsDate) || (mTimestamp & tsTime) )  
  {
    tm t = mt_toTm( curTime );

    if( mTimestamp & tsDate )  
      len += g_sprintf( inOutStr+len, "%04d-%02d-%02d", t.tm_year+1900, t.tm_mon+1, t.tm_mday );

    if( mTimestamp & tsTime )  
    {
      if( len )
        inOutStr[len++] = ' ';
      len += g_sprintf( inOutStr+len, "%02d:%02d:%02d", t.tm_hour, t.tm_min, t.tm_sec );
    }
  }

  if( mTimestamp & tsMS )  
  {
    if( len )
      inOutStr[len++] = ',';

    int ms = (int)(curTime % 1000);
    len += g_sprintf( inOutStr+len, "%3d", ms );
  }
  return len;
}
//-------------------------------------------------------------------------
// Получить строку с идентификатором потока
int TLogWriter::getStrThreadID( char* inOutStr )
{
  guint id = TBL_Thread::CurThreadId();
  int len = g_sprintf( inOutStr, "ThreadID: %6u", id );
  return len;
}
//-----------------------------------------------------------------------------
void TLogWriter::vprintf( const char* cp, const char* format, va_list arglist )
{
  if( !checkEnabled() ) 
    return;

  char stLabel[1024];
  int lenLabel = 0;
  // Назначена вставка временных меток
  if( mTimestamp ) 
  { 
    lenLabel += getStrTime( stLabel + lenLabel ); 
    lenLabel += g_strlcpy( stLabel + lenLabel, "; ", sizeof(stLabel)-lenLabel ); 
  }

  // Назначена вставка меток идентификатора потока
  if( mIsLogThreadID ) 
  {
    lenLabel += getStrThreadID( stLabel + lenLabel );
    lenLabel += g_strlcpy( stLabel + lenLabel, "; ", sizeof(stLabel)-lenLabel ); 
  }

  gchar* str;
  if( lenLabel )
  {
    g_strlcpy( stLabel + lenLabel, format, sizeof(stLabel) - lenLabel );
    str = g_strdup_vprintf( stLabel, arglist );
  }
  else 
    str = g_strdup_vprintf( format, arglist );

  gsize bytes_written;
  gchar* str_cp = g_convert( str, -1, bl_locale, cp, NULL, &bytes_written, NULL );
  if( str_cp )
  {
    write( str_cp, bytes_written );
    g_free( str_cp );
  }
  g_free( str );
}
//-----------------------------------------------------------------------------
static const char hexCODE[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

void TLogWriter::hex( const char* cp, const char* comment, const void* _buf, size_t len )
{
  if( !checkEnabled() ) 
    return;

  char stLabel[1024];
  int lenLabel = 0;
  // Назначена вставка временных меток
  if( mTimestamp ) 
  { 
    lenLabel += getStrTime( stLabel + lenLabel ); 
    lenLabel += g_strlcpy( stLabel + lenLabel, "; ", sizeof(stLabel)-lenLabel ); 
  }
    
  // Назначена вставка меток идентификатора потока
  if( mIsLogThreadID ) 
  {
    lenLabel += getStrThreadID( stLabel + lenLabel );
    lenLabel += g_strlcpy( stLabel + lenLabel, "; ", sizeof(stLabel)-lenLabel ); 
  }

  int lenComment = comment ? (int)strlen(comment) : 0;
  size_t lenData = len*3+2+1; // " 12"
  size_t lenBuf = lenLabel + lenComment*2 + lenData + 2;
  
  if( lenBuf )
  {
    char* st = new char[lenBuf];
    int lenSt = 0;
    
    if( lenLabel )
    {
      strcpy( st, stLabel );
      lenSt = lenLabel;
    }   
    
    if( lenComment )
    {
      int L = lenComment*2;
      bl_conv( bl_locale, st+lenSt, L, cp, comment, lenComment ); 
      lenSt += L;
    } 

    if( lenData )
    {
      unsigned char* buf = (unsigned char*)_buf;
      for ( size_t i=0; i < len; i++ )
      {         
        st[lenSt++] = ' ';
        st[lenSt++] = hexCODE[ (buf[i]>>4) & 0x0F ];
        st[lenSt++] = hexCODE[ buf[i] & 0x0F ];        
      }
    }
    write( st, lenSt );
    delete[] st;
  }
}
//-----------------------------------------------------------------------------
// Уведомление об отключении консоли
void TLogWriter::disableConsole( ILogConsole* console )
{
  mListConsole.remove( console );
  if( mListConsole.empty() )
      mEnable = false;
}
//-----------------------------------------------------------------------------
void TLogWriter::markTimeStamp( int timestamp )
{
  mTimestamp = timestamp;
}
// Выводить ли идентификатор потока, из которого вызывается функция логирования
//-----------------------------------------------------------------------------
void TLogWriter::markThreadId( bool flag )
{
  mIsLogThreadID = flag;
} 
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void BL_LogTo( const char* cp, const char* key, const char* format, va_list arglist )
{
  TLogWriter log( key );
  log.vprintf( cp, format, arglist );
}
//-----------------------------------------------------------------------------
void BL_LogToA( const char* key, const char* format, ... )
{
  va_list arglist;
  va_start( arglist, format );
  TLogWriter log( key );
  log.vprintf( bl_locale, format, arglist );
  va_end( arglist );
}
//-------------------------------------------------------------------------
void BL_LogToW( const char* key, const char* format, ... )
{
  va_list arglist;
  va_start( arglist, format );
  TLogWriter log( key );
  log.vprintf( BL_CP_WIN, format, arglist );
  va_end( arglist );
}
//-------------------------------------------------------------------------
