/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef ContainerRiseH
#define ContainerRiseH

#include "Container.h"
#include "TypeDef.h"

class SHARE_EI TContainerRise
{

	TContainer mC;
	int  mSizeUse;

public:
	TContainerRise();
	
	void SetData(char* p, int size);// ��������� ������ � ��������� ������
	// ��� ���������� ����������� ��������� memcpy

  void AddData(char*p, int size);

	char* GetPtr();
	int GetSize();

	void Alloc(int new_size);   // �� �������� 
	void Realloc( int new_size);// �������� ������ �����
};


#endif
