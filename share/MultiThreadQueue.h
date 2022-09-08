#ifndef __QUEUES_H__
#define __QUEUES_H__

#include "TypeDef.h"
#include "BL_Debug.h"
#include <memory.h>
#include "HiTimer.h"

/*
����� ��� ������ ������� ����� 2-�� ��������
� ����� ������ ������ ���������� Pop ��� PopFresh
� ������ ������ Push

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

		int BeginPos, // ������ �����
        EndPos;   // ������ �����
		volatile int CurrentQueueSize;// ������������ � 2 �������
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

		//�������� ������� � �������
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
		//������� ������� �� �������
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
    //������� ����� ��������� ������� �� �������
    bool PopFresh(void* buffer,int &size, int &time_ms)
    {
      if(CurrentQueueSize == 0) 
        return false;

      int shift = CurrentQueueSize-1;
      BeginPos += shift;
      if(BeginPos >= QueueSize) BeginPos = BeginPos - QueueSize;// �������� ��  BeginPos % QueueSize �� �����
      size = Queue[BeginPos].size;
      time_ms = Queue[BeginPos].time_ms;
      memcpy(buffer,Queue[BeginPos].pData,size);
      Queue[BeginPos].size = 0;
      if(++BeginPos == QueueSize) BeginPos = 0;
      
      CurrentQueueSize -= shift;

      return true;
    }

protected:
		//������� �������
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
