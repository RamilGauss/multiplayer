/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "SrcEvent.h"
#include "DstEvent.h"
#include <stddef.h>
#include "BL_Debug.h"

using namespace std;


TSrcEvent::TSrcEvent()
{
  pDstEvent = NULL;
}
//------------------------------------------------------------
TSrcEvent::~TSrcEvent()
{

}
//------------------------------------------------------------
void TSrcEvent::SetDstObject(TDstEvent* p)
{
  pDstEvent = p;
}
//------------------------------------------------------------
void TSrcEvent::AddEventCopy( void* data, int size)
{
  BL_ASSERT(pDstEvent);
  pDstEvent->AddEventInQueue(sSelfID, data, size, true, -1);
}
//------------------------------------------------------------
void TSrcEvent::AddEventCopy( void* data, int size, unsigned int time_create_ms)
{
  BL_ASSERT(pDstEvent);
  pDstEvent->AddEventInQueue(sSelfID, data, size, true, time_create_ms);
}
//------------------------------------------------------------
void TSrcEvent::AddEventWithoutCopy( void* data, int size)
{
  BL_ASSERT(pDstEvent);
  pDstEvent->AddEventInQueue(sSelfID, data, size, false, -1);
}
//------------------------------------------------------------
void TSrcEvent::AddEventWithoutCopy( void* data, int size, unsigned int time_create_ms)
{
  BL_ASSERT(pDstEvent);
  pDstEvent->AddEventInQueue(sSelfID, data, size, false, time_create_ms);
}
//------------------------------------------------------------
void TSrcEvent::SetSelfID(int selfID)
{
  sSelfID = selfID;
}
//------------------------------------------------------------
