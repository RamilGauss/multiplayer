/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef ContainerPtrH
#define ContainerPtrH

#include "TypeDef.h"

/*
  �� ���������� ���������
  ������ ��������� �� ������ � ������
  �� ��������� ������
*/

class SHARE_EI TContainerPtr
{
protected:
  char* pData;
  int size;
public:
  TContainerPtr();
  virtual ~TContainerPtr();
  // �������� ���������
  virtual void Unlink();

  virtual void SetData(char* p, int s);
  // �������� ������ � ������
  virtual char* GetData(int &s);
  // ����� �������� ������ ��� ��������� �������
  virtual void* GetPtr()const;
  virtual int GetSize()const;
};


#endif

