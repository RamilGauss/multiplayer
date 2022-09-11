/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_CLIENT_PRIVATE_H
#define MELISSA_CLIENT_PRIVATE_H

#include "ContainerContextSc.h"

namespace nsMelissa
{
  // � Slave ��� �������� ���������� ��������� �� ������ �������
	class TClientPrivate
	{
    TContainerContextSc mC;

    unsigned int mClientKey;// ����, � ������� ������������ ������
    unsigned int mID_SessionDown;
  public:	
		TClientPrivate();

    TContainerContextSc* GetC();
    
    void SetID_SessionDown(unsigned int v);
    unsigned int GetID_SessionDown();

    void SetKey(unsigned int v);
    unsigned int GetKey();
	};
}  
#endif