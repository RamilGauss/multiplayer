#ifndef LoadFromHDDH
#define LoadFromHDDH


#include <stdio.h>


class TLoadFromHDD
{

char sPath[260];

FILE* pFile;

public:

	TLoadFromHDD(char* path = NULL);
	~TLoadFromHDD();

	bool ReOpen(char* path);

	bool IsOpen();
	unsigned int Size();
  void Read(void* buffer, int size);

	void Close();


};


#endif

