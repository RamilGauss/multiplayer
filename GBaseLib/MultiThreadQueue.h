/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012
===========================================================================
                        Common Information
"Tanks" GPL Source Code

This file is part of the "Tanks" GPL Source Code.

"Tanks" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"Tanks" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "Tanks" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "Tanks" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "Tanks" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef __QUEUES_H__
#define __QUEUES_H__

#include "TypeDef.h"
#include "BL_Debug.h"
#include <memory.h>
#include "HiTimer.h"

/*
Класс для обмена данными между 2-мя потоками
В одном потоке всегда вызывается Pop или PopFresh
в другом всегда Push

ThreadSafe Class

*/


#if defined( TD_WINDOWS )
#pragma pack(push, 1)

class TMultiThreadQueue
{
  //---------------------------------------------------------------------------
  struct TElem
  {
    guint32 time_ms;

    void* pData;
    int   size;

    TElem(){pData=NULL;size=0;}
    ~TElem(){delete pData;}
  };
  //---------------------------------------------------------------------------

private:
		int QueueSize;
    int SizeElem;

		TElem* Queue;

		int BeginPos, // первый поток
        EndPos;   // второй поток
		volatile int CurrentQueueSize;// используется в 2 потоках
public:
		TMultiThreadQueue(int _QueueSize, int _SizeElem)
		{
			QueueSize = _QueueSize;
      SizeElem = _SizeElem;

			Queue = (TElem*)new TElem[QueueSize];
			for (int i = 0; i < QueueSize; i++)
			{
				Queue[i].pData = new char[SizeElem];
			}

			BeginPos = 0;
			EndPos = 0;
			CurrentQueueSize = 0;
		};

		~TMultiThreadQueue()
		{
       Clear();
    };

		//Добавить элемент в очередь
		bool Push(void* pElem, int size)
    {
      if(CurrentQueueSize == QueueSize) {BL_FIX_BUG();return false;}

      Queue[EndPos].size = size;
      Queue[EndPos].time_ms = ht_GetMSCount();
      memcpy(Queue[EndPos].pData,pElem,size);
      if(++EndPos == QueueSize) EndPos = 0;
      CurrentQueueSize++;

      return true;
    }
		//Извлечь элемент из очереди
		bool Pop(void* buffer,int &size, guint32& time_ms)
    {
      if(CurrentQueueSize == 0) 
        return false;

      size = Queue[BeginPos].size;
      time_ms = Queue[BeginPos].time_ms;
      memcpy(buffer,Queue[BeginPos].pData,size);
      Queue[BeginPos].size = 0;
      if(++BeginPos == QueueSize) BeginPos = 0;
      CurrentQueueSize--;

      return true;
    }
    //Извлечь самый последний элемент из очереди
    bool PopFresh(void* buffer,int &size, guint32 &time_ms)
    {
      if(CurrentQueueSize == 0) 
        return false;

      int shift = CurrentQueueSize-1;
      BeginPos += shift;
      if(BeginPos >= QueueSize) BeginPos = BeginPos - QueueSize;// проверки на  BeginPos % QueueSize не делаю
      size = Queue[BeginPos].size;
      time_ms = Queue[BeginPos].time_ms;
      memcpy(buffer,Queue[BeginPos].pData,size);
      Queue[BeginPos].size = 0;
      if(++BeginPos == QueueSize) BeginPos = 0;
      
      CurrentQueueSize -= shift+1;

      return true;
    }

protected:
		//Очистка очереди
		void Clear()
    {
      delete[] Queue;

      BeginPos = 0;
      EndPos = 0;
      CurrentQueueSize = 0;
    }

};

#pragma pack(pop)
#endif


#endif
