/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012, 2013
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
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef _TYPE_DEF_H_
#define _TYPE_DEF_H_

#define GBASELIB_VER      40  // Версия библиотеки GBaseLib

//------------------------------------------------------------------------------
// Определяем FALSE и TRUE, так как они "нестандартно" определены в gtypes.h
#ifndef	FALSE
#define	FALSE	0
#endif

#ifndef	TRUE
#define	TRUE	1
#endif
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Тип целевой платформы:
#define TD_WIN_NATIVE 0   // windows, разрешено применение WinAPI
#define TD_WIN_CROSS  1   // windows, кроссплатформенная реализация
#define TD_LINUX      2   // Linux

//------------------------------------------------------------------------------
//-------   Определение платформы сборки и выполнения библиотек BaseLib --------
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
//-----------   Определение макросов экспорта/импорта функций    ---------------
//------------------------------------------------------------------------------
#if defined(TD_WINDOWS)

  #ifndef GBASELIB_EI
    #define GBASELIB_EI __declspec( dllimport )
  #endif
  #ifndef SHARE_EI
    #define SHARE_EI __declspec( dllimport )
  #endif
  #ifndef QBASELIB_EI
    #define QBASELIB_EI __declspec( dllimport )
  #endif
  #ifndef MELISSA_EI
    #define MELISSA_EI __declspec( dllimport )
  #endif
	#ifndef NET_TRANSPORT_EI
		#define NET_TRANSPORT_EI __declspec( dllimport )
	#endif

#else //TD_WINDOWS
  
  #define   GBASELIB_EI
  #define   SHARE_EI
  #define   QBASELIB_EI
  #define   MELISSA_EI
  #define   NET_TRANSPORT_EI

#endif //TD_WINDOWS

//------------------------------------------------------------------------------
//-------------------   Управление байтовой упаковкой структур   ---------------
//------------------------------------------------------------------------------

#if defined( __GNUC__ )
  #define _PACKED  __attribute__ ((packed))
#elif defined( TD_WINDOWS ) //defined( __ICL ) || defined( _MSC_VER ) || defined(__BORLANDC__)
  #define _PACKED                     
#else
  #error unknown used _PACKED
#endif

//#define _PACKED  _PACKED

//Использовать макрос следующим образом:
//    typedef struct {...} _PACKED name_strunt;
//    or
//    struct name_strunt {...} _PACKED;
//    or
//    struct _PACKED name_strunt {...};
//
//Для компиляторов под windows (Borlad, Intel, MVC) использовать "сендвич"
//    #if defined( TD_WINDOWS )
//    #pragma pack(push, 1)
//    ...
//    #pragma pack(pop)
//    #endif


//------------------------------------------------------------------------------

#define DECLARE_TYPE( name )  \
              typedef T##name* P##name;         /*указатель на объект*/\
              typedef T##name& R##name;         /*ссылка на объект*/\
              typedef const T##name * PC##name; /*изменяемый указатель на const объект*/\
              typedef const T##name & RC##name; /*изменяемая ссылка на const объект*/
              //typedef T##name * const CP##name; /*const указатель на изменяемый объект*/
              //typedef T##name & const CR##name; /*const ссылка на изменяемый объект*/

#define DECLARE_CLASS( name )   class  T##name;   DECLARE_TYPE( name )
#define DECLARE_STRUCT( name )  struct T##name;   DECLARE_TYPE( name )


//------------------------------------------------------------------------------
//---------------          Сервисные макросы          --------------------------
//------------------------------------------------------------------------------
#define NO_TIME     -1              // не действительное значение времени (time_t)

#define PARAM( name, value )  value // Связывание в тексте названия параметра и его значения

#ifdef WIN32      // Мах допустимая длина полного имени файла (с путем)
  #define MAX_PATH    260           // Вместо определенного в Linux PATH_MAX = 4096
#endif

//-----------------------------------------------------------------------------

#endif

