#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "SaveOnHDD.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <errno.h>
#include <time.h>
#include <sys/timeb.h>
#include "BL_Debug.h"
#include "HiTimer.h"


TSaveOnHDD::TSaveOnHDD(char* path)
{
  pFile = NULL;
	sPath[0] = '\0';

	ReOpen(path);
  
  flgPrintf = false;
  flgDebug  = true;
}
//---------------------------------------------------------------
TSaveOnHDD::~TSaveOnHDD()
{
	Close();
}
//---------------------------------------------------------------
bool TSaveOnHDD::ReOpen(char* path)
{
	Close();

	if(path!=NULL)
		strcpy(sPath, path);

	if(sPath[0] == '\0') return false;

	pFile = fopen(sPath,"wb");
	if(pFile!=NULL) return true;

	char sErr[1000];
	sprintf(sErr,"fopen Error: %s path=\"%s\"",strerror(errno),path);
	BL_MessageBug(sErr,0);

	return false;
}
//---------------------------------------------------------------
bool TSaveOnHDD::IsOpen()
{
	return (pFile!=NULL);
}
//---------------------------------------------------------------
void TSaveOnHDD::Write(void* buffer, int size)
{
	fwrite(buffer, size,1,pFile);
	fflush(pFile);
}
//---------------------------------------------------------------
void TSaveOnHDD::Close()
{
	if(pFile==NULL) return;
	fclose(pFile);
	pFile = NULL;
}
//---------------------------------------------------------------
void TSaveOnHDD::WriteF(const char* format, ... )
{
  if(flgDebug==false) return;

	va_list list;
	va_start(list,format);

  char s[10000]; 
	int res = vsprintf(s,format,list); 
	
  va_end(list);
  if(res==-1) return;
  // делаем то что хотели, будь то запись в файл или в консоль
  printf("%s",s);   
  Write(s,strlen(s));
}
//---------------------------------------------------------------
void TSaveOnHDD::WriteF_time(const char* format, ... )
{
  if(flgDebug==false) return;

  Write_Time();

  va_list list;
  va_start(list,format);

  char s[10000]; 
  int res = vsprintf(s,format,list); 

  va_end(list);
  if(res==-1) return;
  // делаем то что хотели, будь то запись в файл или в консоль
  printf("%s",s);   
  Write(s,strlen(s));
}
//---------------------------------------------------------------
void TSaveOnHDD::Write_Time()
{
  struct _timeb timebuffer;
  time_t time1;
  unsigned short millitm1;

  _ftime( &timebuffer );
  time1 = timebuffer.time;
  millitm1 = timebuffer.millitm;

  char str_time[1000];
  // формируем время в строку
  struct tm * my_time = localtime(&time1);

  sprintf(str_time,"%04d_%02d_%02d %02d:%02d:%02d.%03d\n\t",
    my_time->tm_year+1900,my_time->tm_mon+1,my_time->tm_mday,
    my_time->tm_hour,my_time->tm_min,my_time->tm_sec,millitm1);

  Write(str_time,strlen(str_time));
}
//---------------------------------------------------------------