/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
√удаков –амиль —ергеевич 
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


#ifndef BL_ConvH
#define BL_ConvH

#include "TypeDef.h"

#define BL_CP_WINCYR  "CP1251"
#define BL_CP_KOI8R   "KOI8-R"
#define BL_CP_UTF8    "UTF-8"

#define BL_CP_WIN     BL_CP_WINCYR 

// название локальной кодовой страницы, на которую ориентирована GBaseLib
// название задаетс€ по posix::iconv: CP1251, KOI8-R, UTF-8, ...
// все строки, формируемые функци€ми библиотеки, выдаютс€ в указанной кодировке если не указано иное
GBASELIB_EI extern const char * bl_locale;
//GBASELIB_EI const char * bl_locale();

// ќпределить кодовую страницу GBaseLib
GBASELIB_EI void bl_initLocale();

// ѕринудительно назначить кодовую страницу GBaseLib
GBASELIB_EI void bl_setLocale( const char * const cp );

// конвертировать строку <text> из кодировки <cpSrc> в <cpDest>
// cpDest   - требуема€ кодировка
// textDest - выходной буфер.
// lenDest [in] - размер выходного буфера
//         [out]- длина результирующей строки
// cpSrc    - исходна€ кодировка
// textSrc  - исходна€ сткока
// lenSrc   - длина исходной строки. ≈сли строка 0-терминированна€, то можно указывать -1
// –езультат: адрес 0-терминированной строки-результата.
GBASELIB_EI char* bl_conv( const char* cpDest, char* textDest, int& lenDest,
                           const char* cpSrc, const char* textSrc, int lenSrc = -1 );

//..............................................................................
//................     ‘ункции-обертки укороченного вызова      ................
//..............................................................................

// конвертировать строку <text> из кодировки <cpSrc> в локальную кодировку
//inline char* bl_conv( const char* cpSrc, const char* textSrc, char* textDest )
//{ return bl_conv( bl_locale, cpSrc, textSrc, -1, textDest ); }
//
//// конвертировать строку <text> из кодировки WINCYR в локальную кодировку
inline char* bl_W2L( const char* textSrc, char* textDest, int sizeDest )
{ return bl_conv( bl_locale, textDest, sizeDest, BL_CP_WINCYR, textSrc ); }

// конвертировать строку <text> из локальной кодировки в кодировку WIN
inline char* bl_L2W( const char* textSrc, char* textDest, int sizeDest )
{ return bl_conv( BL_CP_WINCYR, textDest, sizeDest, bl_locale, textSrc ); }

// конвертировать строку <text> из локальной кодировки в кодировку KOI8-R
inline char* bl_L2KOI8( const char* textSrc, char* textDest, int sizeDest )
{ return bl_conv( BL_CP_KOI8R, textDest, sizeDest, bl_locale, textSrc ); }

// конвертировать строку <text> из локальной кодировки в кодировку UTF8
inline char* bl_L2UTF8( const char* textSrc, char* textDest, int sizeDest )
{ return bl_conv( BL_CP_UTF8, textDest, sizeDest, bl_locale, textSrc ); }

// конвертировать строку <text> из кодировки WINCYR в кодировку UTF8
inline char* bl_W2UTF8( const char* textSrc, char* textDest, int sizeDest )
{ return bl_conv( BL_CP_UTF8, textDest, sizeDest, BL_CP_WINCYR, textSrc ); }

// конвертировать строку <text> из кодировки UTF8 в локальную кодировку
inline char* bl_UTF82L( const char* textSrc, char* textDest, int sizeDest )
{ return bl_conv( bl_locale, textDest, sizeDest, BL_CP_UTF8, textSrc ); }
//..............................................................................

#endif

