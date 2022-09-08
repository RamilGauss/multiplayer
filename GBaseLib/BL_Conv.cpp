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
// Определить кодовую страницу GBaseLib
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

      // Длина результата = длина буфера - размер остатка
      lenDest = (lenDest - 1) - outbytes_left;
      // бонус - обязательное завершение \0
      bufDest[lenDest] = '\0';

      g_iconv_close( cd );
    }
    else lenDest = 0;
  }
  return bufDest;
}
//------------------------------------------------------------------------------

