/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
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
