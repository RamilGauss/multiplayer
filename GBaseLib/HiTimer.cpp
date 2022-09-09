/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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


#include "TypeDef.h"
#include "HiTimer.h"
#include <stdio.h>

#define MS_SEC    1000
#define US_SEC    (1000*1000)

static unsigned __int64 tickPerSecond; // ���-�� ����� � �������

//==============================================================================
#if TD_TARGET != TD_LINUX//=====================================================
//==============================================================================
#include <windows.h>
//------------------------------------------------------------------------------
bool ht_Init()
{
  LARGE_INTEGER frequency;
  if( !QueryPerformanceFrequency( &frequency ) )
  {
    tickPerSecond = 1;
    return false;
  }
  tickPerSecond = frequency.QuadPart;
  return true;
}
//------------------------------------------------------------------------------
unsigned __int64 ht_GetTickCount()
{
  LARGE_INTEGER counter;
  if( !QueryPerformanceCounter( &counter ) )
  {
    return 0;
  }
  return counter.QuadPart;
}
//------------------------------------------------------------------------------
// �������� �� �����������
void ht_msleep( unsigned int ms )
{
  Sleep( ms );
}
//------------------------------------------------------------------------------
unsigned int ht_GetMSCount()
{
  return GetTickCount();
}
//------------------------------------------------------------------------------
//==============================================================================
#else//=========================================================================
//==============================================================================
//------------------------------------------------------------------------------
#include <sys/perm.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

//    ��������� ����� ���������� (� �������� �������� ����������)
unsigned __int64 ht_GetTickCount()
{
  unsigned int iax = 0, idx = 0;
  union T64as32
  {
    struct
    {
      unsigned int ax,dx;
    } _PACKED s ;
    unsigned __int64 l;
  } _PACKED;
  T64as32 rec;

  __asm__ __volatile__ ( \
  "rdtsc;" \
  "mov %%eax, %0;" \
  "mov %%edx, %1;" \
   : "=r"(iax), "=r"(idx) : :"%eax","%edx");

  //printf("Tick: 0x%X:0x%X\n", iax, idx);

  rec.s.ax = iax;
  rec.s.dx = idx;
  return rec.l;
}
//------------------------------------------------------------------------------
// ��������� ������� ������� �� ����� ��������� �������
unsigned char ht_CurrentSecond()
{
  unsigned char sec;
  __asm__ __volatile__
 (\
 "\tcli\n"
 "1:\n\tmov $0x0A, %%al\n"
 "	out %%al, $0x70\n"
 "	jmp 2f\n"
 "2:\n\tin  $0x71, %%al\n"
 "	test $0x80, %%al\n"
 "	jne 1b\n"
 "	xor %%ax,%%ax\n"
 "	out %%al, $0x70\n"
 "	jmp 3f\n"
 "3:\n\tin  $0x71, %%al\n"
 "	mov %%al,%%bh\n"
 "	shr $0x4, %%al\n"
 "	mov $0x0A, %%bl\n"
 "	mul %%bl\n"
 "	and $0x0f, %%bh\n"
 "	add %%bh, %%al\n"
 "	mov %%al, %0\n"
 "	sti"\
 : "=r"(sec) : : "%ax","%bx" );
 return sec;
}
//------------------------------------------------------------------------------
// ������ �������� ������ ����������
static unsigned __int64 ht_SpeedCPU()
{
  unsigned __int64 sec, old_sec, tb, te;

  sec = ht_CurrentSecond();
  old_sec = sec;
  do
  {
    tb = ht_GetTickCount();
    sec = ht_CurrentSecond();
  } while( old_sec == sec );

  old_sec = sec;
  do
  {
    sec = ht_CurrentSecond();
    te = ht_GetTickCount();
  }while( old_sec == sec );

  tickPerSecond = ( te - tb );

  return tickPerSecond;
}
//------------------------------------------------------------------------------
// �������� �� �����������
void ht_msleep( unsigned int ms )
{
  usleep( ms*1000 );
}
//------------------------------------------------------------------------------
static long baseTimeSec;
// ����� � ������������ � ������� ������� ��� ��� ���������.
// �������� 1/18 ��� (~55 ����)
unsigned int ht_GetMSCount()
{
  timeval tv;
  gettimeofday( &tv, NULL );
  return (tv.tv_sec - baseTimeSec)*1000 + (tv.tv_usec+500)/1000;
}
//------------------------------------------------------------------------------
int getSizeFile(int fd)
{
	int size_buffer = 1024;
	char* buffer = (char*)malloc(size_buffer);

	for(;;)
	{
    int cnt_read = read(fd,buffer,size_buffer);
		if(cnt_read<size_buffer)
		{
      free(buffer);		 
			return cnt_read;
		}
		else
		{
      size_buffer *= 2;
      buffer = (char*)realloc(buffer,size_buffer);
		}
	}
}
//------------------------------------------------------------------------------
bool QueryPerformanceFrequency( unsigned __int64 *frequency )
{
	const char* sPathCPUInfo = "/proc/cpuinfo";
	int fd = open(sPathCPUInfo,O_EXCL,S_IREAD);

	if(fd<=0) return false;

	int size_buffer = getSizeFile(fd);
	//printf("size=%d\n",size_buffer);

	char* pData = (char*)malloc(size_buffer+1);// ��� ���� ���� ��� ����-������������ ������
  memset(pData,0,size_buffer+1);
	int cnt = read(fd,pData,size_buffer);
	close(fd);

	if(cnt==0||cnt==size_buffer){free(pData); return false;}

	// ���� (�������) ������ ���������
	// ������ ���� "cpu MHz : XXX.XXX"  ������ ��������� ������ 0x0A
	char* strFind = "cpu MHz";
	char * sFreq = strstr(pData,strFind);
	if(sFreq==NULL){free(pData); return false;}
	
	strFind = ":";
	sFreq = strstr(sFreq,strFind);

	if(sFreq==NULL){free(pData); return false;}
	sFreq+=2;// ������ ������ ���� ": XXX.XXX"

	char endString[2];
	endString[0] = 0x0A;
	endString[1] = 0x00;

	char* sEndFreq = strstr(sFreq,endString);
	int endFreq = (int)(sEndFreq-sFreq+1);
	sFreq[endFreq] = '\0';

	//printf("sFreq=%s\n",sFreq);

	double d_freq = atof(sFreq);
	*frequency = (unsigned __int64)(d_freq*1000000);

	//printf("freq = %u Hz\n",*((unsigned int*)frequency));

  free(pData);

	return true;
}
//------------------------------------------------------------------------------
bool ht_Init()
{
 // int res_iopl = iopl( 3 );
	//printf("res_iopl=%d\n",res_iopl);
 // ht_SpeedCPU();
 // //BL_Log( "speed = %d", ht_SpeedCPU() );
 // timeval tv;
 // gettimeofday( &tv, NULL );
 // baseTimeSec = tv.tv_sec;
 // return true;
	unsigned __int64 frequency;
	if( !QueryPerformanceFrequency( &frequency ) )
	{
		tickPerSecond = 1;
		return false;
	}
	tickPerSecond = frequency;
	return true;
}
//==============================================================================
#endif//========================================================================
//==============================================================================
//------------------------------------------------------------------------------
unsigned __int64 ht_us2tick( unsigned int us )
{
  return tickPerSecond * us / US_SEC;
}
//------------------------------------------------------------------------------
unsigned int ht_tick2us( unsigned __int64 tick )
{
  return unsigned int(tick * US_SEC / tickPerSecond);
}
//------------------------------------------------------------------------------
// �������� �� ������������
void ht_usleep( unsigned int us )
{
  unsigned __int64 start  = ht_GetTickCount();
  unsigned __int64 finish = start + tickPerSecond * us / US_SEC;
  for(; ht_GetTickCount() < finish ;)
  {
  }
}
//------------------------------------------------------------------------------
// �������� �� ������������ c �������������� ��������� ���������
// ���������: true - ����� �� ���������� func, false - ����� �� ��������
bool ht_usleep( unsigned int us, THT_CheckFunc func )
{
  unsigned __int64 start  = ht_GetTickCount();
  unsigned __int64 finish = start + tickPerSecond * us / US_SEC;
  for( ; ht_GetTickCount() < finish; )
    if( func() )
      return true;
  return false;
}
//------------------------------------------------------------------------------

