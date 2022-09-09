/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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

// �������� ��������� ������� ��������, �� ������� ������������� GBaseLib
// �������� �������� �� posix::iconv: CP1251, KOI8-R, UTF-8, ...
// ��� ������, ����������� ��������� ����������, �������� � ��������� ��������� ���� �� ������� ����
GBASELIB_EI extern const char * bl_locale;
//GBASELIB_EI const char * bl_locale();

// ���������� ������� �������� GBaseLib
GBASELIB_EI void bl_initLocale();

// ������������� ��������� ������� �������� GBaseLib
GBASELIB_EI void bl_setLocale( const char * const cp );

// �������������� ������ <text> �� ��������� <cpSrc> � <cpDest>
// cpDest   - ��������� ���������
// textDest - �������� �����.
// lenDest [in] - ������ ��������� ������
//         [out]- ����� �������������� ������
// cpSrc    - �������� ���������
// textSrc  - �������� ������
// lenSrc   - ����� �������� ������. ���� ������ 0-���������������, �� ����� ��������� -1
// ���������: ����� 0-��������������� ������-����������.
GBASELIB_EI char* bl_conv( const char* cpDest, char* textDest, int& lenDest,
                           const char* cpSrc, const char* textSrc, int lenSrc = -1 );

//..............................................................................
//................     �������-������� ������������ ������      ................
//..............................................................................

// �������������� ������ <text> �� ��������� <cpSrc> � ��������� ���������
//inline char* bl_conv( const char* cpSrc, const char* textSrc, char* textDest )
//{ return bl_conv( bl_locale, cpSrc, textSrc, -1, textDest ); }
//
//// �������������� ������ <text> �� ��������� WINCYR � ��������� ���������
inline char* bl_W2L( const char* textSrc, char* textDest, int sizeDest )
{ return bl_conv( bl_locale, textDest, sizeDest, BL_CP_WINCYR, textSrc ); }

// �������������� ������ <text> �� ��������� ��������� � ��������� WIN
inline char* bl_L2W( const char* textSrc, char* textDest, int sizeDest )
{ return bl_conv( BL_CP_WINCYR, textDest, sizeDest, bl_locale, textSrc ); }

// �������������� ������ <text> �� ��������� ��������� � ��������� KOI8-R
inline char* bl_L2KOI8( const char* textSrc, char* textDest, int sizeDest )
{ return bl_conv( BL_CP_KOI8R, textDest, sizeDest, bl_locale, textSrc ); }

// �������������� ������ <text> �� ��������� ��������� � ��������� UTF8
inline char* bl_L2UTF8( const char* textSrc, char* textDest, int sizeDest )
{ return bl_conv( BL_CP_UTF8, textDest, sizeDest, bl_locale, textSrc ); }

// �������������� ������ <text> �� ��������� WINCYR � ��������� UTF8
inline char* bl_W2UTF8( const char* textSrc, char* textDest, int sizeDest )
{ return bl_conv( BL_CP_UTF8, textDest, sizeDest, BL_CP_WINCYR, textSrc ); }

// �������������� ������ <text> �� ��������� UTF8 � ��������� ���������
inline char* bl_UTF82L( const char* textSrc, char* textDest, int sizeDest )
{ return bl_conv( bl_locale, textDest, sizeDest, BL_CP_UTF8, textSrc ); }
//..............................................................................

#endif

