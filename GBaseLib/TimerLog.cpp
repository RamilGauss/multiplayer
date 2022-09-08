#include "TimerLog.h"
#include "HiTimer.h"
#include "BL_Debug.h"
#include "ToStr.h"
#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include "BL_Log.h"

TBL_Log TTimerLog::mGlobalLog("performance");

//---------------------------------------------------------------------------
TTimerLog::TTimerLog( bool start, int precision ):
  mPoints(NULL,8) // резервируем начальный размер массива точек - 8 замеров
{
  mPrecision = precision;
  if( start ) Point();
}
//---------------------------------------------------------------------------
void TTimerLog::Point( const char* name )
{
  TPoint* point = mPoints.Append();
  point->name = name;
  if( mPrecision >= 2 )
    point->value = ht_GetMSCount();
  else
    point->value = ht_GetTickCount();
}
//---------------------------------------------------------------------------
void TTimerLog::Print( const char* comment, bool treeView, TBL_Log *log )
{
  if( !log ) log = &mGlobalLog;

  if( mPoints.Count() < 2 )
  {
    log->printfW( "%s: нет зафиксированных отсчетов", comment );
    return;
  }
  
  int lenSt;
  char st[10000];

  guint64 value = mPoints[mPoints.Count()-1]->value - mPoints[0]->value;
  if( mPrecision == tl_US ) value = ht_tick2us(value);

  lenSt = sprintf( st, "%s(", comment );
  n2a( st+lenSt, value );
  lenSt += (int)strlen( st+lenSt );
  switch( mPrecision )
  {
    case tl_Tick: lenSt += sprintf( st+lenSt, " tick)" ); break;
    case tl_US:   lenSt += sprintf( st+lenSt, " us)" ); break;
    case tl_MS:   lenSt += sprintf( st+lenSt, " ms)" ); 
  }
  lenSt += treeView ? sprintf( st+lenSt, ":\n" ) 
                    : sprintf( st+lenSt, "=( " );

  for( int i = 1; i < mPoints.Count(); i++ )
  {
    if( treeView )
      st[lenSt++] = '\t';

    if( mPoints[i]->name )
      lenSt += sprintf( st+lenSt, "%s=", mPoints[i]->name );

    guint64 value = mPoints[i]->value - mPoints[i-1]->value;
    if( mPrecision == tl_US ) value = ht_tick2us(value);
    n2a( st+lenSt, value );
    lenSt += (int)strlen( st+lenSt );

    st[lenSt++] = treeView ? '\n' : '+';
  }
  st[lenSt-1] = treeView ? ' ' : ')';
  st[lenSt] = '\0';
  log->printfW( st );
}
//---------------------------------------------------------------------------
// Зафиксировать точку измерения и вывести результат в лог
void TTimerLog::PointPrint( const char* comment, TBL_Log *log )
{
  guint64 value = ( mPrecision >= 2 ) ? ht_GetMSCount() : ht_GetTickCount();

  if( !log ) log = &mGlobalLog;

  if( !mPoints.Count() )
  {
    log->printfW( "%s: нет зафиксированных отсчетов", comment );
    return;
  }

  int lenSt;
  char st[10000];

  value -= mPoints[0]->value;
  if( mPrecision == tl_US ) value = ht_tick2us(value);

  lenSt = sprintf( st, "%s = ", comment );
  n2a( st+lenSt, value );
  lenSt += (int)strlen( st+lenSt );
  switch( mPrecision )
  {
    case tl_Tick: strcat( st+lenSt, " tick" ); break;
    case tl_US:   strcat( st+lenSt, " us" );   break;
    case tl_MS:   strcat( st+lenSt, " ms" ); 
  }
  log->printfW( st );
}
//---------------------------------------------------------------------------
void TTimerLog::Reset()
{
  mPoints.Clear();
}
//---------------------------------------------------------------------------

