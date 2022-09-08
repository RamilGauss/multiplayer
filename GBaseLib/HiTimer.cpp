#include "TypeDef.h"
#include "HiTimer.h"
#include <stdio.h>

#define MS_SEC    1000
#define US_SEC    (1000*1000)

static guint64 tickPerSecond; // кол-во тиков в секунду

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
guint64 ht_GetTickCount()
{
  LARGE_INTEGER counter;
  if( !QueryPerformanceCounter( &counter ) )
  {
    return 0;
  }
  return counter.QuadPart;
}
//------------------------------------------------------------------------------
// Задержка на милисекунды
void ht_msleep( guint32 ms )
{
  Sleep( ms );
}
//------------------------------------------------------------------------------
guint32 ht_GetMSCount()
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

//    Получение тиков процессора (с тактовой частотой процессора)
guint64 ht_GetTickCount()
{
  guint32 iax = 0, idx = 0;
  union T64as32
  {
    struct
    {
      guint32 ax,dx;
    } _PACKED s ;
    guint64 l;
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
// Получение текущей секунды из часов реального времени
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
// Расчет скорости работы процессора
static guint64 ht_SpeedCPU()
{
  guint64 sec, old_sec, tb, te;

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
// Задержка на милисекунды
void ht_msleep( guint32 ms )
{
  usleep( ms*1000 );
}
//------------------------------------------------------------------------------
static long baseTimeSec;
// Время в милисекундах с момента запуска ЭВМ или программы.
// Точность 1/18 сек (~55 мсек)
guint32 ht_GetMSCount()
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
bool QueryPerformanceFrequency( guint64 *frequency )
{
	const char* sPathCPUInfo = "/proc/cpuinfo";
	int fd = open(sPathCPUInfo,O_EXCL,S_IREAD);

	if(fd<=0) return false;

	int size_buffer = getSizeFile(fd);
	//printf("size=%d\n",size_buffer);

	char* pData = (char*)malloc(size_buffer+1);// еще один байт под нуль-терминальный символ
  memset(pData,0,size_buffer+1);
	int cnt = read(fd,pData,size_buffer);
	close(fd);

	if(cnt==0||cnt==size_buffer){free(pData); return false;}

	// ищем (парсинг) начало появления
	// строка типа "cpu MHz : XXX.XXX"  символ окончания строки 0x0A
	char* strFind = "cpu MHz";
	char * sFreq = strstr(pData,strFind);
	if(sFreq==NULL){free(pData); return false;}
	
	strFind = ":";
	sFreq = strstr(sFreq,strFind);

	if(sFreq==NULL){free(pData); return false;}
	sFreq+=2;// сейчас строка вида ": XXX.XXX"

	char endString[2];
	endString[0] = 0x0A;
	endString[1] = 0x00;

	char* sEndFreq = strstr(sFreq,endString);
	int endFreq = (int)(sEndFreq-sFreq+1);
	sFreq[endFreq] = '\0';

	//printf("sFreq=%s\n",sFreq);

	double d_freq = atof(sFreq);
	*frequency = (guint64)(d_freq*1000000);

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
	guint64 frequency;
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
guint64 ht_us2tick( guint32 us )
{
  return tickPerSecond * us / US_SEC;
}
//------------------------------------------------------------------------------
guint32 ht_tick2us( guint64 tick )
{
  return guint32(tick * US_SEC / tickPerSecond);
}
//------------------------------------------------------------------------------
// Задержка на микросекунды
void ht_usleep( guint32 us )
{
  guint64 start  = ht_GetTickCount();
  guint64 finish = start + tickPerSecond * us / US_SEC;
  for(; ht_GetTickCount() < finish ;)
  {
  }
}
//------------------------------------------------------------------------------
// Задержка на микросекунды c дополнительной проверкой состояния
// Результат: true - выход по результату func, false - выход по таймауту
bool ht_usleep( guint32 us, THT_CheckFunc func )
{
  guint64 start  = ht_GetTickCount();
  guint64 finish = start + tickPerSecond * us / US_SEC;
  for( ; ht_GetTickCount() < finish; )
    if( func() )
      return true;
  return false;
}
//------------------------------------------------------------------------------

