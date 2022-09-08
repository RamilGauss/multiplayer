#ifndef HiTimerH
#define HiTimerH

#include "TypeDef.h"      // ������� ����������� �����

//------------------------------------------------------------------------------
// ������ ������ � �������� �������� ����������
//------------------------------------------------------------------------------

// ����������� �������������
bool GBASELIB_EI ht_Init();

// ��������� ����� ���������� (� �������� �������� ����������)
guint64 GBASELIB_EI ht_GetTickCount();

// �������� �� ������������
void GBASELIB_EI ht_usleep( guint32 us );

// �������� �� ������������
void GBASELIB_EI ht_msleep( guint32 ms );

// �������� �� ������������ c �������������� ��������� ���������
// ���������: true - ����� �� ���������� func, false - ����� �� ��������
typedef bool (*THT_CheckFunc)(void);
bool GBASELIB_EI ht_sleep( guint32 us, THT_CheckFunc func );

// ������� ����������� � ����
guint64 GBASELIB_EI ht_us2tick( guint32 us );

// ������� ����� � �����������
guint32 GBASELIB_EI ht_tick2us( guint64 tick );

// ��������� ����� ���������� ������������� � ������������
inline guint32 ht_GetUSCount()
{
  return ht_tick2us( ht_GetTickCount() );
}
// ����� � ������������� � ������� ������� ��� ��� ���������.
// �������� �� 55 ���� � ����
guint32 GBASELIB_EI ht_GetMSCount();
//------------------------------------------------------------------------------
#if !defined(MS2US)
  // ��������� ������������ � ������������
  #define MS2US( n )    ( (n) * 1000 )
  // ��������� ������� � ������������
  #define SEC2MS( n )   ( (n) * 1000 )
  // ��������� ������� � ������������
  #define SEC2US( n )   MS2US( SEC2MS(n) )
#endif
//------------------------------------------------------------------------------

#endif
