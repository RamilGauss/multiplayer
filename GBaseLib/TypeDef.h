/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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


#ifndef _TYPE_DEF_H_
#define _TYPE_DEF_H_

#define GBASELIB_VER      40  // ������ ���������� GBaseLib

//------------------------------------------------------------------------------
// ���������� FALSE � TRUE, ��� ��� ��� "������������" ���������� � gtypes.h
#ifndef	FALSE
#define	FALSE	0
#endif

#ifndef	TRUE
#define	TRUE	1
#endif
//------------------------------------------------------------------------------

#include <glib/gtypes.h>      // ������� ����������� �����

//------------------------------------------------------------------------------
// ��� ������� ���������:
#define TD_WIN_NATIVE 0   // windows, ��������� ���������� WinAPI
#define TD_WIN_CROSS  1   // windows, ������������������ ����������
#define TD_LINUX      2   // Linux

//------------------------------------------------------------------------------
//-------   ����������� ��������� ������ � ���������� ��������� BaseLib --------
//------------------------------------------------------------------------------
#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32)

  #define TD_WINDOWS

  #if defined(TD_CROSSPLATFORM)
              #define TD_TARGET TD_WIN_CROSS
  #else
              #define TD_TARGET TD_WIN_NATIVE
  #endif
#else //WIN32

  #define   TD_TARGET TD_LINUX

#endif

//------------------------------------------------------------------------------
//-----------   ����������� �������� ��������/������� �������    ---------------
//------------------------------------------------------------------------------
#if defined(TD_WINDOWS)

  #ifndef GBASELIB_EI
    #define GBASELIB_EI// __declspec( dllimport )
  #endif
  #ifndef QBASELIB_EI
    #define QBASELIB_EI //__declspec( dllimport )
  #endif

#else //TD_WINDOWS
  
  #define   GBASELIB_EI
  #define   QBASELIB_EI

#endif //TD_WINDOWS

//------------------------------------------------------------------------------
//-------------------   ���������� �������� ��������� ��������   ---------------
//------------------------------------------------------------------------------

#if defined( __GNUC__ )
  #define _PACKED  __attribute__ ((packed))
#elif defined( TD_WINDOWS ) //defined( __ICL ) || defined( _MSC_VER ) || defined(__BORLANDC__)
  #define _PACKED                     
#else
  #error unknown used _PACKED
#endif

//#define _PACKED  _PACKED

//������������ ������ ��������� �������:
//    typedef struct {...} _PACKED name_strunt;
//    or
//    struct name_strunt {...} _PACKED;
//    or
//    struct _PACKED name_strunt {...};
//
//��� ������������ ��� windows (Borlad, Intel, MVC) ������������ "�������"
//    #if defined( TD_WINDOWS )
//    #pragma pack(push, 1)
//    ...
//    #pragma pack(pop)
//    #endif


//------------------------------------------------------------------------------

#define DECLARE_TYPE( name )  \
              typedef T##name* P##name;         /*��������� �� ������*/\
              typedef T##name& R##name;         /*������ �� ������*/\
              typedef const T##name * PC##name; /*���������� ��������� �� const ������*/\
              typedef const T##name & RC##name; /*���������� ������ �� const ������*/
              //typedef T##name * const CP##name; /*const ��������� �� ���������� ������*/
              //typedef T##name & const CR##name; /*const ������ �� ���������� ������*/

#define DECLARE_CLASS( name )   class  T##name;   DECLARE_TYPE( name )
#define DECLARE_STRUCT( name )  struct T##name;   DECLARE_TYPE( name )


//------------------------------------------------------------------------------
//---------------          ��������� �������          --------------------------
//------------------------------------------------------------------------------
#define NO_TIME     -1              // �� �������������� �������� ������� (time_t)

#define PARAM( name, value )  value // ���������� � ������ �������� ��������� � ��� ��������

#ifdef WIN32      // ��� ���������� ����� ������� ����� ����� (� �����)
  #define MAX_PATH    260           // ������ ������������� � Linux PATH_MAX = 4096
#endif

//-----------------------------------------------------------------------------

#endif

