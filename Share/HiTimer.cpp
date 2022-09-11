/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "HiTimer.h"

#ifdef WIN32
  #include <windows.h>
#endif

#include <boost/thread/thread.hpp>
#include <boost/chrono/include.hpp>
#include <boost/chrono/time_point.hpp>

using namespace boost;

//------------------------------------------------------------------------------
unsigned __int64 ht_GetUSCount()
{
  typedef chrono::process_real_cpu_clock type_clock;

  type_clock::time_point t = type_clock::now();
  return t.time_since_epoch().count();
}
//------------------------------------------------------------------------------
// �������� �� ������������
void ht_msleep( unsigned int ms )
{
	// � ���� ���� ��� ��� Windows XP boost �������� �������� (��. chrono)
#ifdef WIN32
	Sleep(ms);
#else
	chrono::milliseconds time_sleep(ms);
  this_thread::sleep_for( time_sleep );
#endif
}
//------------------------------------------------------------------------------
unsigned int ht_GetMSCount()
{
  typedef chrono::process_real_cpu_clock type_clock;

  type_clock::time_point t = type_clock::now();
  return (unsigned int)(t.time_since_epoch().count()/1000000);
}
//------------------------------------------------------------------------------
// �������� �� ������������
void ht_usleep( unsigned int us )
{
  bool ht_usleep( unsigned int us, THT_CheckFunc func );
  ht_usleep( us, (THT_CheckFunc)NULL );
}
//------------------------------------------------------------------------------
// �������� �� ������������ c �������������� ��������� ���������
// ���������: true - ����� �� ���������� func, false - ����� �� ��������
bool ht_usleep( unsigned int us, THT_CheckFunc func )
{
  unsigned __int64 start  = ht_GetUSCount();
  unsigned __int64 finish = start + us;
  for( ; ht_GetUSCount() < finish; )
    if( func )
      if( func() )
        return true;
  return false;
}
//------------------------------------------------------------------------------

