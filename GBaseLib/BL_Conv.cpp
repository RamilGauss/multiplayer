/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
√удаков –амиль —ергеевич 
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


#include "TypeDef.h"
#include "BL_Conv.h"
#include <string.h>                     
#include <glib.h>                            

//#define BL_CP_WIN     BL_CP_WINCYR 
//#define BL_CP_LINUX   BL_CP_KOI8R

// название локальной кодовой страницы
#if TD_TARGET == TD_LINUX
	const char *bl_locale = BL_CP_KOI8R;
#else
	const char *bl_locale = BL_CP_WINCYR;
#endif

//------------------------------------------------------------------------------
// ќпределить кодовую страницу GBaseLib
void bl_initLocale()
{
  const char* locale_charset = NULL;
  g_get_charset( &locale_charset );
  if( locale_charset )
    bl_setLocale( locale_charset );
}
//------------------------------------------------------------------------------
void bl_setLocale( const char * const cp )
{
  bl_locale = cp;
}
//------------------------------------------------------------------------------
char* bl_conv( const char* cpDest, char* bufDest, int& lenDest, const char* cpSrc, const char* textSrc, int lenSrc )
{
  if( cpDest && cpSrc && textSrc && bufDest )
  {
    // lenSrc - длина исходной строки без терминального /0
    if( lenSrc == -1 ) lenSrc = (int)strlen( textSrc );

    GIConv cd = g_iconv_open( cpDest, cpSrc );
    if( cd != (GIConv)-1 )
    {
      gchar** ptextSrc = (gchar**)&textSrc; // исходный текст
      gsize inbytes_left = lenSrc;          // размер исходного текста, байты
      gchar *outbuf = bufDest;             // выходной буфер
      gsize outbytes_left = lenDest-1;      // размер выходного буфера (без учета терминального /0)
    	   
      g_iconv( cd, ptextSrc, &inbytes_left, &outbuf, &outbytes_left );       

      // ƒлина результата = длина буфера - размер остатка
      lenDest = (lenDest - 1) - outbytes_left;
      // бонус - об€зательное завершение \0
      bufDest[lenDest] = '\0';

      g_iconv_close( cd );
    }
    else lenDest = 0;
  }
  return bufDest;
}
//------------------------------------------------------------------------------

