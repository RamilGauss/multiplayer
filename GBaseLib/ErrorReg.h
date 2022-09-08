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
// Структура программного описания отказа
typedef struct
{
  time_t      time;   // время регистрации
  guint8      group;  // код зарегистрированной группы ошибок
  size_t      code;   // код ошибки в группе
              // Точка фиксации отказа:
  int         line;   // номер строки
  const char* file;   // имя файла

  void*       source; // источник отказа
} TErrorDef;

// Описание последнего отказа
extern GBASELIB_EI TErrorDef err_LastDef;

//------------------------------------------------------------------------------
// Структура пользовательского описания отказа
typedef struct
{
  // Строка с описанием отказа в формате:
  // %date,% %time,% %errSource:% %text% %,filename, линия - line%
  enum { size_str = 1000 };
  char str[size_str];

  int length;       // общая длина строки

  int startTime;    // индекс начала в строке поля %time%
  int startSource;  // индекс начала в строке поля %errSource%
  int startFile;    // индекс начала в строке поля %filename%
} TErrorDesc;

// Описание последнего отказа
extern GBASELIB_EI TErrorDesc err_LastDesc;

//------------------------------------------------------------------------------
// Заполнить структуру  с пользовательским описанием ошибки (отказа)
// def  [IN]  - структура программого описания отказа (формируется регистратором отказов)
// desc [OUT] - структура пользовательского описания отказа
GBASELIB_EI void errDesc_Fill( TErrorDef &def, TErrorDesc &desc );

// Проверить и заполнить структуру err_DescLast
// При каждом заполнении err_DefLast структура err_DescLast сбрасывается
inline void err_LastDesc_Fill()
{
  if( !err_LastDesc.length )
    errDesc_Fill( err_LastDef, err_LastDesc );
}
//------------------------------------------------------------------------------

// Вкл/выкл регистратора отказов
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

// Идентификаторы зарегистрированных групп отказов
extern GBASELIB_EI guint8 egSTD;
extern GBASELIB_EI guint8 egSTR;
#if defined(TD_WINDOWS)
  extern GBASELIB_EI guint8 egSDK;
#endif
//------------------------------------------------------------------------------
//----------     Управление внешними уведомителями об отказах    ---------------
//------------------------------------------------------------------------------

// Тип процедуры внешнего уведомителя об ошибках
// user_data - произвольный параметр, указываемый при регистрации
typedef void (*TErrorNotification)( void* user_data );

// Описание элемента списка регистраторов ошибок
struct TDefNotification
{
  TErrorNotification  proc;       // процедура вызова
  void*               user_data;  // произвольный параметр, передаваемый в процедуру вызова
  struct TDefNotification *next;  // следующий регистратор в списке
};

// Установка/удаление внешнего уведомителя
// pDef: описатель уведомителя
// set:  true/false - установить/удалить
GBASELIB_EI void err_SetNotification( struct TDefNotification *def, bool set );

//-----------------------------------------------------------------------------
//-----    Уведомитель об отказах, протоколирующий отказы в файле   -----------
//-----------------------------------------------------------------------------
// Установка/удаление уведомителя
// fileName - полное имя файла протокола.
//            Если не задано, то пишет в файл с названием совпадающим с названием
//            исполняемого файла, но с расширением ".err"
GBASELIB_EI void ErrNotify_File_Init( const char* fileName = 0 );
GBASELIB_EI void ErrNotify_File_Done();

//-----------------------------------------------------------------------------
//----------            Управление группами отказов             ---------------
//-----------------------------------------------------------------------------

// Типы функции интерфейса групп отказов
// Запрос текстового описания отказа по коду
typedef int (*TGetStrError)( size_t code, char* buf, int len );
typedef void (*TClearBufError)(void); // Очистить рабочий буфер
typedef void (*TDoneGroup)(void);     // Завершить работу

// Интерфейс группы отказов
typedef struct {
  TGetStrError    procGetStr;
  TClearBufError  procClearBuf;
  TDoneGroup      procDoneGroup;
} TDefErrorGroup;

typedef TDefErrorGroup* PDefErrorGroup;

// Зарегистрировать группу отказов
// pdeg - описание группы
// Результат: уникальный в процессе идентификатор группы
GBASELIB_EI guint8 err_RegisterGroup( PDefErrorGroup pdeg );

// Макрос простой регистрации группы
#define ERR_REGGROUP( varIdGroup, varDefGroup )     \
  if( !varIdGroup ) {                               \
    varIdGroup = err_RegisterGroup( &varDefGroup ); \
    return varIdGroup != 0;                         \
  }                                                 \
  return true;

//-----------------------------------------------------------------------------
#ifdef __cplusplus
  // Зарегистрировать источник отказов
  // name - описание источника (название)
  // cp - название кодовой страницы (см. BL_Local.h:BL_CP_XXX). Может быть = 0
  // Результат: уникальный в процессе идентификатор источника или NULL
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

// Запрос текстового описания источника отказа
GBASELIB_EI int err_GetStrSource( void* source, char* buf, int len );

//-----------------------------------------------------------------------------
// -------------  Базовые функции и макроса регистрации отказов   -------------
//-----------------------------------------------------------------------------

// Зарегистрировать отказ
// group - идентификатор группы отказов
// code - код отказа (size_t - целочисленный тип с размерностью указателя и на 32 и на 64 платформах)
// file - название исходного файла в котором зафиксирован отказ
// line - номер строки в указанном файле
// source - зарегистрированный источник отказов (см. err_RegisterSource())
#ifdef __cplusplus
  GBASELIB_EI void err_SetError( guint8 group, size_t code, const char* file = NULL, int line = 0, void* source = NULL );
#else
  GBASELIB_EI void err_SetError( guint8 group, size_t code, const char* file, int line, void* source );
#endif

#define defStr( name )    #name
#define xdefStr( name )   defStr(name)

// Макросы укороченного вызова
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

GBASELIB_EI guint32 errSDK_push();              // Сохранить код отказа
GBASELIB_EI void errSDK_pop( guint32 errCode ); // Восстановить код отказа

// Макросы укороченного вызова для отказов группы SDK ( Windows API )
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

	// Макросы укороченного вызова для отказов группы WAVEMIX ( wave Windows API )
#define errWAVEMIX_SET( code )              errWAVEMIX_Set( code, __FILE__, __LINE__, NULL )
#define errWAVEMIX_SETSOURCE( code, source )errWAVEMIX_Set( code, __FILE__, __LINE__, source )

// Макросы укороченного вызова для отказов группы WAVEIN ( wave Windows API )
#define errWAVEIN_SET( code )              errWAVEIN_Set( code, __FILE__, __LINE__, NULL )
#define errWAVEIN_SETSOURCE( code, source )errWAVEIN_Set( code, __FILE__, __LINE__, source )

// Макросы укороченного вызова для отказов группы WAVEOUT ( wave Windows API )
#define errWAVEOUT_SET( code )              errWAVEOUT_Set( code, __FILE__, __LINE__, NULL )
#define errWAVEOUT_SETSOURCE( code, source )errWAVEOUT_Set( code, __FILE__, __LINE__, source )

#endif
//-----------------------------------------------------------------------------
// -------------          Обработка отказов Linux ALSA API          -----------
//-----------------------------------------------------------------------------
#if TD_TARGET == TD_LINUX

#ifdef __cplusplus
  GBASELIB_EI void errSND_Set( guint32 code, const char* file = NULL, int line = 0, void* source = NULL );
#else
  GBASELIB_EI void errSND_Set( guint32 code, const char* file, int line, void* source );
#endif


  // Макросы укороченного вызова для отказов группы STD( std::errno )
#define errSND( )                       errSND_Set( errno, __FILE__, __LINE__, NULL )
#define errSND_SET( code )              errSND_Set( code, __FILE__, __LINE__, NULL )
#define errSND_SOURCE( source )         errSND_Set( errno, __FILE__, __LINE__, source )
#define errSND_SETSOURCE( code, source )errSND_Set( code, __FILE__, __LINE__, source )

#endif
//-----------------------------------------------------------------------------
// --------     Обработка отказов stdlib, возвращаемых через <errno>    -------
//-----------------------------------------------------------------------------
#ifdef __cplusplus
  GBASELIB_EI void errSTD_Set( guint32 code, const char* file = NULL, int line = 0, void* source = NULL );
#else
  GBASELIB_EI void errSTD_Set( guint32 code, const char* file, int line, void* source );
#endif

GBASELIB_EI guint32 errSTD_push();              // Сохранить код отказа
GBASELIB_EI void errSTD_pop( guint32 errCode ); // Восстановить код отказа

  // Макросы укороченного вызова для отказов группы STD( std::errno )
#define errSTD( )                       errSTD_Set( errno, __FILE__, __LINE__, NULL )
#define errSTD_SET( code )              errSTD_Set( code, __FILE__, __LINE__, NULL )
#define errSTD_SOURCE( source )         errSTD_Set( errno, __FILE__, __LINE__, source )
#define errSTD_SETSOURCE( code, source )errSTD_Set( code, __FILE__, __LINE__, source )

//-----------------------------------------------------------------------------
// -----   Обработка отказов с произвольным текстовым описанием     -----------
//-----------------------------------------------------------------------------

// cp - название кодовой страницы (см. BL_Local.h:BL_CP_XXX). Может быть = 0
// text - текстовое описание в кодовой странице <cp>
GBASELIB_EI void errStr_Set( const char* cp, const char* text, const char* file, int line, void* source );

#define errStr_SET( text )                errStr_Set( BL_CP_WIN, text, __FILE__, __LINE__, NULL )
#define errStr_SETSOURCE( text, source )  errStr_Set( BL_CP_WIN, text, __FILE__, __LINE__, source )

// Уведомление об отказе в текстовом виде с произвольным форматированием
GBASELIB_EI void errStr_printf( const char* cp, const char* file, int line, void* source, const char* format, ... );

//-----------------------------------------------------------------------------

// Запрос текстового описания отказа по ее группе и коду
GBASELIB_EI int err_GetStrError( guint8 group, size_t code, char* buf, int len );

// Очистить рабочие буфера групп отказов
GBASELIB_EI void  err_ClearBufError();

//-----------------------------------------------------------------------------

#ifdef __cplusplus
}//  extern "C"
#endif

#endif
