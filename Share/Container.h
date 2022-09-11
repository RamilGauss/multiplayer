/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef ContainerH
#define ContainerH

#include "TypeDef.h"

/*
  ���������� ��� ������� ��� �������� ������ ���������
  ���������� ������ ���������� ������,
  ������ ��� ���������
*/

class SHARE_EI TContainer
{
protected:
  char* pData;
  int size;
public:
  TContainer();
  virtual ~TContainer();
  // ���������� ������
  virtual void Done();
  // ������ ��� �������� ����� ������, ������ ������ ���� �������� new TClass[N]
  virtual void Entrust(char* p, int s);
  // ���������� ������ ������
  virtual void SetData(char* p, int s);
  // �������� ������ � ������
  virtual char* GetData(int &s);
  // ����� �������� ������ ��� ��������� �������
  virtual void* GetPtr()const;
  virtual int GetSize()const;
  // ��� �� �� ����������� ������ � ������� Done()
  virtual void Unlink();

  static void Free(char* p){delete[]p;}
};


#endif

