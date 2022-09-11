/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef DstEventH
#define DstEventH

#include <string>
#include <vector>
#include "TypeDef.h"
#include "DescEvent.h"
#include "ListMultiThread.h"

class TSrcEvent;

/*
  поглотитель событий. работает в связке с TSrcEvent
  пронаследоваться,
  зарегистрировать источники с помощью функции AddSrcEvent
  и получать события через GetEvent
*/

class SHARE_EI TDstEvent
{
  TListMultiThread<nsEvent::TEvent>* pListEvent;

public:

  TDstEvent();
  virtual ~TDstEvent();

  void AddEventInQueue(int from, void* data, int size, bool copy, unsigned int time_create_ms);

protected:
  // забрал объект - уничтожь с помощью delete
  nsEvent::TEvent* GetEvent();
  
  void AddSrcEvent(TSrcEvent* pSrcEvent);
};

#endif
