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


#include "BL_IOStream.h"
#include "ErrorReg.h"
#include <string.h>
#include <glib.h>
#include <glib/gprintf.h>

//---------------------------------------------------------------
TIOStream::TIOStream(void)
{
}
//---------------------------------------------------------------
// Прочитать данные из потока
bool TIOStream::get( void* buf, guint size )
{ 
  int res = read( buf, size );
  if( (guint)res == size ) return true;
  if( res >= 0 ) 
    errStr_SETSOURCE( "Неполное чтение блока данных", errSource() );
  return false;
}
//---------------------------------------------------------------
// Записать данные в поток
bool TIOStream::put( const void* buf, guint size )
{ 
  int res = write( buf, size );
  if( (guint)res == size ) return true;
  if( res >= 0 ) 
    errStr_SETSOURCE( "Неполная запись блока данных", errSource() );
  return false;
}
//---------------------------------------------------------------
bool TIOStream::writeStr( const char* st, int len )
{
  if( len < 0 ) len = st ? strlen( st ) : 0;
  if( len > 0xFFFF ) return false;
  if( put( &len, sizeof(guint16) ) )
  {
    return st ? put( st, len ) : true;
  }
  return false;
}
//---------------------------------------------------------------
bool TIOStream::writeStrS( const char* st, int len )
{
  if( len < 0 ) len = st ? strlen( st ) : 0;
  if( len > 0xFF ) return false;
  if( put( &len, sizeof(unsigned char) ) )
  {
    return st ? put( st, len ) : true;
  }
  return false;
}
//---------------------------------------------------------------
int TIOStream::readStr( char *buf, unsigned sizebuf )
{
  guint16 len;
  if( get( &len, sizeof(guint16) ) )
  {
    if( len < (guint16)sizebuf )
    {
      if( get( buf, len ) )
      {
        buf[len] = '\0';
        return true;
      }
    }
  }
  return -1;
}
//---------------------------------------------------------------
int TIOStream::readStrS( char *buf, unsigned sizebuf )
{
  unsigned char len;
  if( get( &len, sizeof(unsigned char) ) )
  {
    if( len < (unsigned char)sizebuf )
    {
      if( get( buf, len ) )
      {
        buf[len] = '\0';
        return true;
      }
    }
  }
  return -1;
}
//---------------------------------------------------------------
// Прочитать строку из потока
gchar* TIOStream::readStr()
{
  guint16 len;
  if( get( &len, sizeof(guint16) ) )
  {
    gchar* p = (gchar*)g_try_malloc( len+1 );
    if( p )
    {
      if( get( p, len ) )
      {
        p[len] = '\0';
        return p;
      }
      g_free( p );
    }
  }
  return NULL;
}
//---------------------------------------------------------------
bool TIOStream::writeStringEOL( const char* st, int len )
{
  if( len == -1 )
    len = st ? (int)strlen( st ) : 0 ;
#if TD_TARGET == TD_LINUX
  return st ? put( st, len ) && put( "\n", 1 ): true;
#else
  return st ? put( st, len ) && put( "\r\n", 2 ): true;
#endif
}
//---------------------------------------------------------------
bool TIOStream::vprintf( const char* format, va_list arglist )
{
  bool res = false;
  gchar* p;
  guint len = (guint)g_vasprintf( &p, format, arglist );
  if( len )
  {
    res = put( p, len );
    g_free( p );
  }
  return res;
}
//---------------------------------------------------------------
long TIOStream::curPos()      { errStr_SETSOURCE("Навигация по потоку не поддерживается", errSource() ); return -1; }
bool TIOStream::goTop()       { errStr_SETSOURCE("Навигация по потоку не поддерживается", errSource() ); return false; }
bool TIOStream::goPos(long)   { errStr_SETSOURCE("Навигация по потоку не поддерживается", errSource() ); return false; }
bool TIOStream::goBottom()    { errStr_SETSOURCE("Навигация по потоку не поддерживается", errSource() ); return false; }
bool TIOStream::shiftPos(long){ errStr_SETSOURCE("Навигация по потоку не поддерживается", errSource() ); return false; }
//---------------------------------------------------------------
long TIOStream::length()      { errStr_SETSOURCE("Операция не поддерживается", errSource() ); return -1; }
bool TIOStream::chsize(long ) { errStr_SETSOURCE("Операция не поддерживается", errSource() ); return false; }
//---------------------------------------------------------------
bool TIOStream::isOpen() const 
{ 
  return false; 
} 
bool TIOStream::flush() 
{ 
  return true; 
} 
//---------------------------------------------------------------
