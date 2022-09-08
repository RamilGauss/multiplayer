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

