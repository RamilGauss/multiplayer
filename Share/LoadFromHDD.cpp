#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "LoadFromHDD.h"

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>




TLoadFromHDD::TLoadFromHDD(char* path)
{
  pFile = NULL;
  sPath[0] = '\0';

	ReOpen(path);

}
//---------------------------------------------------------------
TLoadFromHDD::~TLoadFromHDD()
{
	Close();
}
//---------------------------------------------------------------
bool TLoadFromHDD::ReOpen(char* path)
{
  Close();

	if(path!=NULL)
    strcpy(sPath, path);

	if(sPath[0] == '\0') return false;

	pFile = fopen(sPath,"rb");
	if(pFile!=NULL) return true;

	return false;
}
//---------------------------------------------------------------
bool TLoadFromHDD::IsOpen()
{
  return (pFile!=NULL);
}
//---------------------------------------------------------------
unsigned int TLoadFromHDD::Size()
{
#ifdef WIN32
	int fd = _fileno(pFile);

  if(fd<0) return 0;

  struct _stat buffer;
  int resstat = _fstat( fd,&buffer);
  if(resstat!=0) return 0;

  return buffer.st_size;
#else
  int fd = fileno(pFile);

  if(fd<0) return 0;

  struct stat buffer;
  int resstat = fstat( fd,&buffer);
  if(resstat!=0) return 0;

  return buffer.st_size;
#endif
}
//---------------------------------------------------------------
void TLoadFromHDD::Read(void* buffer, int size)
{
	size = fread(buffer, size,1,pFile);
}
//---------------------------------------------------------------
void TLoadFromHDD::Close()
{
	if(pFile==NULL) return;
  fclose(pFile);
	pFile = NULL;
}
//---------------------------------------------------------------
