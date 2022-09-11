/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/


#ifndef LoadFromHDDH
#define LoadFromHDDH


#include <stdio.h>
#include "TypeDef.h"
/*
  �������� �� �����
*/

class SHARE_EI TLoadFromHDD
{
  char sPath[260];
  FILE* pFile;

public:

	TLoadFromHDD(char* path = NULL);
	~TLoadFromHDD();

	bool ReOpen(char* path);

	bool IsOpen();
	unsigned int Size();

  int Read(void* buffer, int size, int offset = 0);

 	void Close();

};

#endif

