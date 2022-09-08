///////////////////////////////////////////////////////////
//  ILogConsole.h
//  Implementation of the Interface ILogConsole
//  Created on:      29-���-2010 19:28:11
//  Original author: KIT
///////////////////////////////////////////////////////////

#if !defined(EA_8E69BCFA_17EA_4797_A94F_221DE2574BF5__INCLUDED_)
#define EA_8E69BCFA_17EA_4797_A94F_221DE2574BF5__INCLUDED_

#include "GCS.h"

class GBASELIB_EI ILogConsole : public GCS
{
public:
  ILogConsole() {}
  virtual ~ILogConsole() {}
  
  // ������� ������ �� �������
	// str - ������������ ������
  // len - ����� ������ (���� = -1, �� ��������� �� strlen)
  // eol - ���� ������������� �������� ������� �� ����� ������ ����� ������
  virtual void write(const char* str, int len = -1, bool eol = true ) = 0;

  // �������� �������
  // ���������� ����� �������� ������� ���, ����������� ��������� � ���������� � 
  // ������� �� ������ ����������������� �����
	virtual const char* name() const = 0;
};

#endif // !defined(EA_8E69BCFA_17EA_4797_A94F_221DE2574BF5__INCLUDED_)
