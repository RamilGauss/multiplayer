/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef SaveOnHDDH
#define SaveOnHDDH


#include <stdio.h>
#include <vector>

#include "TypeDef.h"
#include "Container.h"
#include "ListPtr.h"

/*
  ���������� �����. 
  ����� � �������.
  ����������� ��� ���������� ��������� �����.
*/

class SHARE_EI TSaveOnHDD
{
  std::string sPath;

	FILE* pFile;

  bool flgPrintf;
  bool flgEnable;
  bool flgBuffer;

  typedef TListPtr<TContainer> TListContainer;
  
  TListContainer mListBuffer;

  enum{eMaxNumberForBufferization=5000};

public:

	TSaveOnHDD(char* path = NULL);
	virtual ~TSaveOnHDD();

  // � ������ append==false - ������� ���������� �����
	virtual bool ReOpen(char* path, bool append = false );

	virtual bool IsOpen();

	virtual void Write(void* buffer, int size);
  // ��������������� ������, �� ���� printf(...)
  virtual void WriteF(const char* format, ... );
  virtual void WriteF_time(const char* format, ... );

	virtual void Close();

  void SetPrintf(bool val){flgPrintf=val;};// ��� ��� ������������ - ����������� � ����� �� �������
  bool GetPrintf(){return flgPrintf;};     // �� ������ ��� ��������������� ������

  void SetEnable(bool val){flgEnable=val;};// ������ ���������� � Write, WriteF, WriteF_time
  bool GetEnable(){return flgEnable;};

  void SetBufferization(bool val){flgBuffer=val;};// �����������, ��� ��������� ����� ��� ������������ � ������
  bool GetBufferization(){return flgBuffer;};

protected:
  void Write_Time();

  void ClearBuffer();
  void FlushBuffer();
  void FlushInBuffer(char* buffer, int size);
};


#endif

