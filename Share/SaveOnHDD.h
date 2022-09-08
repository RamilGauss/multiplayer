#ifndef SaveOnHDDH
#define SaveOnHDDH


#include <stdio.h>


class TSaveOnHDD
{

	char sPath[260];

	FILE* pFile;

  bool flgPrintf;
  bool flgDebug;
public:

	TSaveOnHDD(char* path = NULL);
	~TSaveOnHDD();

	bool ReOpen(char* path);

	bool IsOpen();

	void Write(void* buffer, int size);
  void WriteF(const char* format, ... );

	void Close();

  void SetPrintf(bool val){flgPrintf=val;};
  bool GetPrintf(){return flgPrintf;};

  void SetDebug(bool val){flgDebug=val;};
  bool GetDebug(){return flgDebug;};

};


#endif

