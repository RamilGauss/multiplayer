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


#include "ErrorReg.h"
#include <glib.h>
#include <string.h>
#include "BL_Conv.h"

//------------------------------------------------------------------------------
guint8 egSTR;

// куча под зарегистрированные описания отказов
// строки хранятся в локальной кодировке
static GStringChunk*  hHeap;
//------------------------------------------------------------------------------
static GStaticRecMutex CS = G_STATIC_REC_MUTEX_INIT;

#define begin()  g_static_rec_mutex_lock( &CS )
#define end()    g_static_rec_mutex_unlock( &CS )
//------------------------------------------------------------------------------
// cp - название кодовой страницы, по iconv(CP1251,KOI8-R,...)
// text - текстовое описание в кодовой странице <cp>
void errStr_Set( const char* cp, const char* text, const char* file, int line, void* source )
{
  begin();
  if( egSTR )
  {
    gchar* lp;
    // описание отказа указано в локальной кодировке
    if( !strcmp( cp, bl_locale) )
    {
      // вставляем в реестр отказов
      lp = g_string_chunk_insert( hHeap, text );
    }
    else
    {
      // перекодируем в локальную кодировку
      gsize bytes_written;
      gchar* str_cp = g_convert( text, -1, bl_locale, cp, NULL, &bytes_written, NULL );
      if( str_cp )
      { 
        // вставляем в реестр отказов
        lp = g_string_chunk_insert( hHeap, str_cp );
        g_free( str_cp );
      }
      else 
      {
        lp = g_string_chunk_insert( hHeap, "Unknown message (error of translation)." );
      }
    }
    end();

    if( lp )
    {
      err_SetError( egSTR, (size_t)lp, file, line, source );
      return;
    }
  }
  else end();
}
//------------------------------------------------------------------------------
void errStr_printf( const char* cp, const char* file, int line, void* source, const char* format, ...  )
{
  va_list arglist;
  va_start( arglist, format );
  gchar* text = g_strdup_vprintf( format, arglist );
  errStr_Set( cp, text, file, line, source );
  g_free( text );
  va_end( arglist );
}
//------------------------------------------------------------------------------
static int GetStr( size_t code, char* buf, int len )
{
  begin();
  if( egSTR )
  {
    int l = (int)strlen( (char*)code );
    len = ( l >= len ) ? len-1 : l;
    memcpy( buf, (char*)code, len );
    buf[len] = '\0';
  }
  else len = 0;
  end();
  return len;
}
//------------------------------------------------------------------------------
static void ClearBuf(void)
{
  begin();
  if( egSTR )
  {
    if( hHeap ) g_string_chunk_free(hHeap);
    hHeap = g_string_chunk_new( 4096 );
    if( !hHeap )
    {
      egSTR = 0;
    }
  }
  end();
}
//-----------------------------------------------------------------------------
static void DoneGroup(void)
{
  begin();
  if( egSTR )
  {
    if( hHeap ) { g_string_chunk_free(hHeap); hHeap = NULL; }
    egSTR = 0;
  }
}
//-----------------------------------------------------------------------------
static TDefErrorGroup deg = {
  GetStr,
  ClearBuf,
  DoneGroup
};
//-----------------------------------------------------------------------------
bool errSTR_Init()
{
  if( !egSTR )
  {
    hHeap = g_string_chunk_new( 4096 );
    if( hHeap )
    {
      egSTR = err_RegisterGroup( &deg );
      if( egSTR ) return true;

      g_string_chunk_free(hHeap);
    }
  }
  else return true;
  return false;
}
//-----------------------------------------------------------------------------
