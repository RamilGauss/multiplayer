/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef _TYPE_DEF_H_
#define _TYPE_DEF_H_

//------------------------------------------------------------------------------
// ���������� FALSE � TRUE, ��� ��� ��� "������������" ���������� � gtypes.h
#ifndef	FALSE
#define	FALSE	0
#endif

#ifndef	TRUE
#define	TRUE	1
#endif
//------------------------------------------------------------------------------
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

  #ifndef SHARE_EI
    #define SHARE_EI         __declspec( dllimport )
  #endif
  #ifndef MELISSA_EI
    #define MELISSA_EI       __declspec( dllimport )
  #endif
	#ifndef NET_TRANSPORT_EI
		#define NET_TRANSPORT_EI __declspec( dllimport )
	#endif
  #ifndef QTLIB_EI
    #define QTLIB_EI         __declspec( dllimport )
  #endif
#else //TD_WINDOWS
  
  #define   SHARE_EI
  #define   MELISSA_EI
  #define   NET_TRANSPORT_EI
  #define   QTLIB_EI

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

