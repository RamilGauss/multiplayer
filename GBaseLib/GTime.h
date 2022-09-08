#ifndef GTimeH
#define GTimeH

#include <time.h>   // ������� �����������!!! � <glib/gdate.h> ������������� struct tm
                    // ��� namespace std
#include <glib.h>

//------------------------------------------------------------------------------
#if !defined(MS2US)
  // ��������� ����������� � ������������
  #define MS2US( n )    ( (n) * 1000 )
  // ��������� ������� � �����������
  #define SEC2MS( n )   ( (n) * 1000 )
  // ��������� ������� � ������������
  #define SEC2US( n )   MS2US( SEC2MS(n) )
#endif
//------------------------------------------------------------------------------

// ���������� ������������ ��������� ����� ����� ���������� �������, ��
inline glong gtime_Interval( const GTimeVal &begin, const GTimeVal &end )
{
  glong delay = (end.tv_sec - begin.tv_sec) * 1000 +
                (end.tv_usec - begin.tv_usec) / 1000;
  return delay;
}
//------------------------------------------------------------------------------
// ���������� ������������ ��������� ����� ������� �������� � ��������� ������, ��
inline glong gtime_Interval( const GTimeVal &begin )
{
  GTimeVal end;
  g_get_current_time( &end );
  return gtime_Interval( begin, end );
}
//------------------------------------------------------------------------------
// ��������� ���� ������
// ���������:
// 1: t1 > t2
// -1: t1 < t2
// 0: t1 == t2
inline int gtime_compare( const GTimeVal &t1, const GTimeVal &t2 )
{
  return  ( t1.tv_sec > t2.tv_sec ) ? 1 :
          ( t1.tv_sec < t2.tv_sec ) ? -1 :
          ( t1.tv_usec > t2.tv_usec ) ? 1 :
          ( t1.tv_usec < t2.tv_usec ) ? -1 :
          0;
}
//------------------------------------------------------------------------------
// �������� ����� ��������� �� �������� �� �������� ��������
// shift      - �������� ������, ���
// res [OUT]  - ������� �����
inline void gtime_ShiftFromCurrent( GTimeVal &res, glong shift )
{
  g_get_current_time( &res );
  g_time_val_add( &res, shift );
}
//------------------------------------------------------------------------------
// ���������� ��������� ���������� ������
inline bool gtime_IsFinished( GTimeVal &val )
{
  GTimeVal cur;
  g_get_current_time( &cur );
  return gtime_compare( cur, val ) >= 0;
}
//------------------------------------------------------------------------------

#endif
