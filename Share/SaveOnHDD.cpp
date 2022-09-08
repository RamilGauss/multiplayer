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
#include "BL_Debug.h"


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
