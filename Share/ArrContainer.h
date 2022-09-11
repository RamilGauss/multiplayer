/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef ArrContainerH
#define ArrContainerH

#include <vector>
#include "Container.h"

/*
  ����� ������ � �������� �����������
  ������ ������� ��������� ������
  ������, ��� � std::vector
*/
class SHARE_EI TArrContainer
{

  typedef std::vector<TContainer> TVectorContainer;

  TVectorContainer mVec;

  TContainer mFC;// ��� ������

public:

  TArrContainer();
  ~TArrContainer();
  // ������ � ���������
  void GetElem( char*& p, int& size, int index); 
  void SetElem( char*p,   int  size, int index); 
  // ���-�� ���������
  void SetCount(int v);
  int GetCount();

  // ������� ��������� �� ������ �������
  char* GetPtr();
  // ������ - ����� ���� �������� ���������
  int GetSize();

protected:
  void Collect();
};

#endif
