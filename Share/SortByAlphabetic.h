/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef SORTBYALPHABETICH
#define SORTBYALPHABETICH

#include <string>
#include <vector>
#include "TypeDef.h"

// ��� ��������������� ������� ������ � ������� � ������
// ���� ����� ����� �������, ������� ��������� �� ����� ������ ����

class SHARE_EI TSortByAlphabetic
{

public:
  struct T
  {
    std::string s;
    int         v;
    int         index;
  };
  
  TSortByAlphabetic();
  virtual ~TSortByAlphabetic();

  void Sort(std::vector<T>* pVecInOut );

};


#endif
