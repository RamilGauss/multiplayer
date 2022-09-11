/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012, 2013
===========================================================================
                        Common Information
"TornadoEngine" GPL Source Code

This file is part of the "TornadoEngine" GPL Source Code.

"TornadoEngine" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"TornadoEngine" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "TornadoEngine" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "TornadoEngine" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "TornadoEngine" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef Queue2ThreadH
#define Queue2ThreadH

#include <memory.h>

#include "TypeDef.h"
#include "BL_Debug.h"
#include "HiTimer.h"
#include "Container.h"

/*
Класс для обмена данными между 2-мя потоками
В одном потоке всегда вызывается Pop или PopFresh
в другом всегда Push
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

	int mBeginPos, // первый поток
			mEndPos;   // второй поток
	volatile int mCurrentCount;// используется в 2 потоках
public:
	TQueue2Thread(int queue_count);

	virtual ~TQueue2Thread();
	//Добавить элемент в очередь
	bool Push(void* buffer, int size);
	//Извлечь элемент из очереди
	bool Pop(void* buffer,int &size);

protected:
	//Очистка очереди
	void Clear();
};

#if defined( TD_WINDOWS )
#pragma pack(pop)
#endif


#endif
