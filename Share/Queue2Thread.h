/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef Queue2ThreadH
#define Queue2ThreadH

#include <memory.h>

#include "TypeDef.h"
#include "BL_Debug.h"
#include "HiTimer.h"
#include "Container.h"

/*
����� ��� ������ ������� ����� 2-�� ��������
� ����� ������ ������ ���������� Pop ��� PopFresh
� ������ ������ Push
FIFO

ThreadSafe Class

*/
#if defined( TD_WINDOWS )
#pragma pack(push, 1)
#endif

class SHARE_EI TQueue2Thread
{
	int mCount;

	TContainer* pQueue;

	int mBeginPos, // ������ �����
			mEndPos;   // ������ �����
	volatile int mCurrentCount;// ������������ � 2 �������
public:
	TQueue2Thread(int queue_count);

	virtual ~TQueue2Thread();
	//�������� ������� � �������
	bool Push(void* buffer, int size);
	//������� ������� �� �������
	bool Pop(void* buffer,int &size);

protected:
	//������� �������
	void Clear();
};

#if defined( TD_WINDOWS )
#pragma pack(pop)
#endif


#endif
