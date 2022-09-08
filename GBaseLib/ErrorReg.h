#ifndef ErrorRegH
#define ErrorRegH

#include "TypeDef.h"
#include <errno.h>
#include "BL_Conv.h"
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif


//------------------------------------------------------------------------------
// ��������� ������������ �������� ������
typedef struct
{
  time_t      time;   // ����� �����������
  guint8      group;  // ��� ������������������ ������ ������
  size_t      code;   // ��� ������ � ������
              // ����� �������� ������:
  int         line;   // ����� ������
  const char* file;   // ��� �����

  void*       source; // �������� ������
} TErrorDef;

// �������� ���������� ������
extern GBASELIB_EI TErrorDef err_LastDef;

//------------------------------------------------------------------------------
// ��������� ����������������� �������� ������
typedef struct
{
  // ������ � ��������� ������ � �������:
  // %date,% %time,% %errSource:% %text% %,filename, ����� - line%
  enum { size_str = 1000 };
  char str[size_str];

  int length;       // ����� ����� ������

  int startTime;    // ������ ������ � ������ ���� %time%
  int startSource;  // ������ ������ � ������ ���� %errSource%
  int startFile;    // ������ ������ � ������ ���� %filename%
} TErrorDesc;

// �������� ���������� ������
extern GBASELIB_EI TErrorDesc err_LastDesc;

//------------------------------------------------------------------------------
// ��������� ���������  � ���������������� ��������� ������ (������)
// def  [IN]  - ��������� ����������� �������� ������ (����������� ������������� �������)
// desc [OUT] - ��������� ����������������� �������� ������
GBASELIB_EI void errDesc_Fill( TErrorDef &def, TErrorDesc &desc );

// ��������� � ��������� ��������� err_DescLast
// ��� ������ ���������� err_DefLast ��������� err_DescLast ������������
inline void err_LastDesc_Fill()
{
  if( !err_LastDesc.length )
    errDesc_Fill( err_LastDef, err_LastDesc );
}
//------------------------------------------------------------------------------

// ���/���� ������������ �������
GBASELIB_EI void err_Init();
GBASELIB_EI void err_Done();

GBASELIB_EI bool errSTR_Init();
GBASELIB_EI bool errSTD_Init();
#if defined(TD_WINDOWS)
  GBASELIB_EI bool errSDK_Init();
  GBASELIB_EI bool errWAVE_Init();
  #define errSND_Init()
#else
  #define errSDK_Init()
  #define errWAVE_Init()
  GBASELIB_EI bool errSND_Init();
#endif

// �������������� ������������������ ����� �������
extern GBASELIB_EI guint8 egSTD;
extern GBASELIB_EI guint8 egSTR;
#if defined(TD_WINDOWS)
  extern GBASELIB_EI guint8 egSDK;
#endif
//------------------------------------------------------------------------------
//----------     ���������� �������� ������������� �� �������    ---------------
//------------------------------------------------------------------------------

// ��� ��������� �������� ����������� �� �������
// user_data - ������������ ��������, ����������� ��� �����������
typedef void (*TErrorNotification)( void* user_data );

// �������� �������� ������ ������������� ������
struct TDefNotification
{
  TErrorNotification  proc;       // ��������� ������
  void*               user_data;  // ������������ ��������, ������������ � ��������� ������
  struct TDefNotification *next;  // ��������� ����������� � ������
};

// ���������/�������� �������� �����������
// pDef: ��������� �����������
// set:  true/false - ����������/�������
GBASELIB_EI void err_SetNotification( struct TDefNotification *def, bool set );

//-----------------------------------------------------------------------------
//-----    ����������� �� �������, ��������������� ������ � �����   -----------
//-----------------------------------------------------------------------------
// ���������/�������� �����������
// fileName - ������ ��� ����� ���������.
//            ���� �� ������, �� ����� � ���� � ��������� ����������� � ���������
//            ������������ �����, �� � ����������� ".err"
GBASELIB_EI void ErrNotify_File_Init( const char* fileName = 0 );
GBASELIB_EI void ErrNotify_File_Done();

//-----------------------------------------------------------------------------
//----------            ���������� �������� �������             ---------------
//-----------------------------------------------------------------------------

// ���� ������� ���������� ����� �������
// ������ ���������� �������� ������ �� ����
typedef int (*TGetStrError)( size_t code, char* buf, int len );
typedef void (*TClearBufError)(void); // �������� ������� �����
typedef void (*TDoneGroup)(void);     // ��������� ������

// ��������� ������ �������
typedef struct {
  TGetStrError    procGetStr;
  TClearBufError  procClearBuf;
  TDoneGroup      procDoneGroup;
} TDefErrorGroup;

typedef TDefErrorGroup* PDefErrorGroup;

// ���������������� ������ �������
// pdeg - �������� ������
// ���������: ���������� � �������� ������������� ������
GBASELIB_EI guint8 err_RegisterGroup( PDefErrorGroup pdeg );

// ������ ������� ����������� ������
#define ERR_REGGROUP( varIdGroup, varDefGroup )     \
  if( !varIdGroup ) {                               \
    varIdGroup = err_RegisterGroup( &varDefGroup ); \
    return varIdGroup != 0;                         \
  }                                                 \
  return true;

//-----------------------------------------------------------------------------
#ifdef __cplusplus
  // ���������������� �������� �������
  // name - �������� ��������� (��������)
  // cp - �������� ������� �������� (��. BL_Local.h:BL_CP_XXX). ����� ���� = 0
  // ���������: ���������� � �������� ������������� ��������� ��� NULL
  #if defined(__BORLANDC__)
    #pragma warning( push )
    #pragma warning( disable :8058 )
  #endif
  
  GBASELIB_EI void* err_RegisterSource( const char *name, const char* cp = BL_CP_WIN );

  #if defined(__BORLANDC__)
    #pragma warning( pop )
  #endif
#else
  GBASELIB_EI void* err_RegisterSource( const char *name, const char* cp );
#endif

// ������ ���������� �������� ��������� ������
GBASELIB_EI int err_GetStrSource( void* source, char* buf, int len );

//-----------------------------------------------------------------------------
// -------------  ������� ������� � ������� ����������� �������   -------------
//-----------------------------------------------------------------------------

// ���������������� �����
// group - ������������� ������ �������
// code - ��� ������ (size_t - ������������� ��� � ������������ ��������� � �� 32 � �� 64 ����������)
// file - �������� ��������� ����� � ������� ������������ �����
// line - ����� ������ � ��������� �����
// source - ������������������ �������� ������� (��. err_RegisterSource())
#ifdef __cplusplus
  GBASELIB_EI void err_SetError( guint8 group, size_t code, const char* file = NULL, int line = 0, void* source = NULL );
#else
  GBASELIB_EI void err_SetError( guint8 group, size_t code, const char* file, int line, void* source );
#endif

#define defStr( name )    #name
#define xdefStr( name )   defStr(name)

// ������� ������������ ������
#define err_SetErrorLINE( group, code )           err_SetError( group, code, __FILE__, __LINE__, NULL )
#define err_SetErrorSOURCE( group, code, source ) err_SetError( group, code, __FILE__, __LINE__, source )

//-----------------------------------------------------------------------------
// -------------          Windows platform: SDK API            ----------------
//-----------------------------------------------------------------------------
#if defined(TD_WINDOWS)

#ifdef __cplusplus
  GBASELIB_EI void errSDK_Set( guint32 code, const char* file = NULL, int line = 0, void* source = NULL );
#else
  GBASELIB_EI void errSDK_Set( guint32 code, const char* file, int line, void* source );
#endif

GBASELIB_EI guint32 errSDK_push();              // ��������� ��� ������
GBASELIB_EI void errSDK_pop( guint32 errCode ); // ������������ ��� ������

// ������� ������������ ������ ��� ������� ������ SDK ( Windows API )
#define errSDK( )                       errSDK_Set( GetLastError(), __FILE__, __LINE__, NULL )
#define errSDK_SET( code )              errSDK_Set( code, __FILE__, __LINE__, NULL )
#define errSDK_SOURCE( source )         errSDK_Set( GetLastError(), __FILE__, __LINE__, source )
#define errSDK_SETSOURCE( code, source )errSDK_Set( code, __FILE__, __LINE__, source )

#endif
//-----------------------------------------------------------------------------
// -------------          Windows platform: wave API            ---------------
//-----------------------------------------------------------------------------
#if defined(TD_WINDOWS)

#ifdef __cplusplus
  GBASELIB_EI void errWAVEMIX_Set( guint32 code, const char* file = NULL, int line = 0, void* source = NULL );
  GBASELIB_EI void errWAVEIN_Set( guint32 code, const char* file = NULL, int line = 0, void* source = NULL );
  GBASELIB_EI void errWAVEOUT_Set( guint32 code, const char* file = NULL, int line = 0, void* source = NULL );
#else
  GBASELIB_EI void errWAVEMIX_Set( guint32 code, const char* file, int line, void* source );
  GBASELIB_EI void errWAVEIN_Set( guint32 code, const char* file, int line, void* source );
  GBASELIB_EI void errWAVEOUT_Set( guint32 code, const char* file, int line, void* source );
#endif

	// ������� ������������ ������ ��� ������� ������ WAVEMIX ( wave Windows API )
#define errWAVEMIX_SET( code )              errWAVEMIX_Set( code, __FILE__, __LINE__, NULL )
#define errWAVEMIX_SETSOURCE( code, source )errWAVEMIX_Set( code, __FILE__, __LINE__, source )

// ������� ������������ ������ ��� ������� ������ WAVEIN ( wave Windows API )
#define errWAVEIN_SET( code )              errWAVEIN_Set( code, __FILE__, __LINE__, NULL )
#define errWAVEIN_SETSOURCE( code, source )errWAVEIN_Set( code, __FILE__, __LINE__, source )

// ������� ������������ ������ ��� ������� ������ WAVEOUT ( wave Windows API )
#define errWAVEOUT_SET( code )              errWAVEOUT_Set( code, __FILE__, __LINE__, NULL )
#define errWAVEOUT_SETSOURCE( code, source )errWAVEOUT_Set( code, __FILE__, __LINE__, source )

#endif
//-----------------------------------------------------------------------------
// -------------          ��������� ������� Linux ALSA API          -----------
//-----------------------------------------------------------------------------
#if TD_TARGET == TD_LINUX

#ifdef __cplusplus
  GBASELIB_EI void errSND_Set( guint32 code, const char* file = NULL, int line = 0, void* source = NULL );
#else
  GBASELIB_EI void errSND_Set( guint32 code, const char* file, int line, void* source );
#endif


  // ������� ������������ ������ ��� ������� ������ STD( std::errno )
#define errSND( )                       errSND_Set( errno, __FILE__, __LINE__, NULL )
#define errSND_SET( code )              errSND_Set( code, __FILE__, __LINE__, NULL )
#define errSND_SOURCE( source )         errSND_Set( errno, __FILE__, __LINE__, source )
#define errSND_SETSOURCE( code, source )errSND_Set( code, __FILE__, __LINE__, source )

#endif
//-----------------------------------------------------------------------------
// --------     ��������� ������� stdlib, ������������ ����� <errno>    -------
//-----------------------------------------------------------------------------
#ifdef __cplusplus
  GBASELIB_EI void errSTD_Set( guint32 code, const char* file = NULL, int line = 0, void* source = NULL );
#else
  GBASELIB_EI void errSTD_Set( guint32 code, const char* file, int line, void* source );
#endif

GBASELIB_EI guint32 errSTD_push();              // ��������� ��� ������
GBASELIB_EI void errSTD_pop( guint32 errCode ); // ������������ ��� ������

  // ������� ������������ ������ ��� ������� ������ STD( std::errno )
#define errSTD( )                       errSTD_Set( errno, __FILE__, __LINE__, NULL )
#define errSTD_SET( code )              errSTD_Set( code, __FILE__, __LINE__, NULL )
#define errSTD_SOURCE( source )         errSTD_Set( errno, __FILE__, __LINE__, source )
#define errSTD_SETSOURCE( code, source )errSTD_Set( code, __FILE__, __LINE__, source )

//-----------------------------------------------------------------------------
// -----   ��������� ������� � ������������ ��������� ���������     -----------
//-----------------------------------------------------------------------------

// cp - �������� ������� �������� (��. BL_Local.h:BL_CP_XXX). ����� ���� = 0
// text - ��������� �������� � ������� �������� <cp>
GBASELIB_EI void errStr_Set( const char* cp, const char* text, const char* file, int line, void* source );

#define errStr_SET( text )                errStr_Set( BL_CP_WIN, text, __FILE__, __LINE__, NULL )
#define errStr_SETSOURCE( text, source )  errStr_Set( BL_CP_WIN, text, __FILE__, __LINE__, source )

// ����������� �� ������ � ��������� ���� � ������������ ���������������
GBASELIB_EI void errStr_printf( const char* cp, const char* file, int line, void* source, const char* format, ... );

//-----------------------------------------------------------------------------

// ������ ���������� �������� ������ �� �� ������ � ����
GBASELIB_EI int err_GetStrError( guint8 group, size_t code, char* buf, int len );

// �������� ������� ������ ����� �������
GBASELIB_EI void  err_ClearBufError();

//-----------------------------------------------------------------------------

#ifdef __cplusplus
}//  extern "C"
#endif

#endif
