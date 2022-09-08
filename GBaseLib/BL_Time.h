#ifndef BL_TimeH
#define BL_TimeH

#include "TypeDef.h"
#include <time.h>

// ���������� �����������, ��������� � 1 ������ 1970 �.
typedef gint64 mtime_t;

//---------------------------------------------------------------------
// �������� ������� ��������� ����� 
GBASELIB_EI mtime_t mt_sysTime();

// �������� ������� ����������� ����� (� ����������)
GBASELIB_EI mtime_t mt_time();

// ������� ����������� �����
inline time_t bl_time()  { return (time_t)( mt_time()/1000 ); }
//---------------------------------------------------------------------
// ������������� time_t � mtime_t 
inline mtime_t mt_fromTime_t( time_t time ) { return time * 1000;}

// ������������� mtime_t � time_t
inline time_t  mt_toTime_t( mtime_t time )  { return (time_t)( time / 1000); }

// ������������� mtime_t � tm
GBASELIB_EI tm mt_toTm( mtime_t curTime );
//---------------------------------------------------------------------
// ��������� ���������� ������������ ������� ��� ������ ��������
// val - ����� �������������� �� ��������� ���
GBASELIB_EI void mt_correctByPrecision( mtime_t val );

// ��������������� ����������� �����
// delta - �������� ���������, ���
GBASELIB_EI void bl_time_correct( int delta );

//---------------------------------------------------------------------
// �������������� �� ���������� ������� �� ����� �� �������� � �������
time_t GBASELIB_EI time_LocalToGMT( time_t t );
time_t GBASELIB_EI time_GMTToLocal( time_t t );

// ������ �� ��������� ��������� ����� � ������ �����
// ���� ����� �� ������, �� ��������������� ������� �����
bool GBASELIB_EI IsSummerTime( time_t *curTime = 0 );

//------------------------------------------------------------------------------
// ��������������� ��������� ����� �� ������������
void GBASELIB_EI bl_time_to_system();
//------------------------------------------------------------------------------

#endif
