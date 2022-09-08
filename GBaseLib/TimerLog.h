#ifndef TimerLogH
#define TimerLogH

#include "hArray.h"
#include "BL_Log.h"

class GBASELIB_EI TTimerLog
{
  struct TPoint
  {
    guint64 value;
    const char* name;
  };
  typedef TArrayVAR<TPoint> TArrayPoint;

  int mPrecision;
#ifdef _MSC_VER
  // 1. отключим "дурацкое" предупреждение VC "class 'TArrayVAR<TItem>' needs to have dll-interface to be used by clients of class 'TSensParam'"
# pragma warning(push)
# pragma warning(disable:4251)
    TArrayPoint mPoints;
# pragma warning(pop)
#else // _MSC_VER
    TArrayPoint mPoints;
#endif // _MSC_VER
  
  static TBL_Log mGlobalLog;
public:
  
  // точность измерения: 
  enum {
    tl_Tick, // процессорные тики
    tl_US,   // микросекунды
    tl_MS,   // миллисекунды
  };
  
  // start - начать отчет времени с текущего момента
  // precision - точность измерения (см. tp_XXX)
  TTimerLog( bool start = false, int precision = tl_MS );
  ~TTimerLog() {}

  // Зафиксировать новую точку измерения
  // name - название точки
  void Point( const char* name = 0 );
  
  // Вывести в лог результаты замеров
  // comment  - заголовок отчета
  // treeView - вид отчета: treeView - древовидный, false - однострочный
  // log      - объект потока логирования. Если не задан, то вывод 
  //            осуществляется в поток "performance"
  void Print( const char* comment, bool treeView, TBL_Log *log = NULL );

  // Зафиксировать точку измерения и вывести результат в лог
  // comment  - заголовок отчета
  // log      - объект потока логирования. Если не задан, то вывод 
  //            осуществляется в поток "performance"
  void PointPrint( const char* comment, TBL_Log *log = NULL );

  // Сброс замеров
  void Reset();
};
#endif //TimerLogH

#if !defined(TIMER_LOG_MACRO_DISABLE) && !defined(TL_START_TICK)
  #if defined(TIMER_LOG_ENABLE)
    #define TL_START_TICK( start )                TTimerLog TL( start, TTimerLog::tl_Tick )
    #define TL_START_US( start )                  TTimerLog TL( start, TTimerLog::tl_US )     
    #define TL_START_MS( start )                  TTimerLog TL( start, TTimerLog::tl_MS )
    #define TL_START( start )                     TTimerLog TL( start, TTimerLog::tl_MS )
    #define TL_POINT( name )                      TL.Point( name )

    #define TL_PRINT( comment, treeView )         TL.Print( comment, treeView )
    #define TL_PRINTLOG( comment, treeView, log ) TL.Print( comment, treeView, log )

    #define TL_POINTPRINT( comment )              TL.PointPrint( comment )
    #define TL_POINTPRINTLOG( comment, log )      TL.PointPrint( comment, log )

    #define TL_RESTART()                          { TL.Reset(); TL.Point(); }
  #else
    #define TL_START_TICK( start )                
    #define TL_START_US( start )                  
    #define TL_START_MS( start )                  
    #define TL_START( start )                     
    #define TL_POINT( name )
    #define TL_PRINT( comment, treeView )
    #define TL_PRINTLOG( comment, treeView, log )
    #define TL_POINTPRINT( comment )              
    #define TL_POINTPRINTLOG( comment, log )      
    #define TL_RESTART()                    
  #endif
#endif//TIMER_LOG_MACRO_DISABLE



