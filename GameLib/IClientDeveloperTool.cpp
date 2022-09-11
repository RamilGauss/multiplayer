/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "IClientDeveloperTool.h"
#include "BL_Debug.h"
#include "NameSrcEventID.h"

using namespace nsEvent;

static IClientDeveloperTool* g_ClientDeveloperTool = NULL;

IClientDeveloperTool::IClientDeveloperTool()
{
  BL_ASSERT(g_ClientDeveloperTool==NULL);// только один объект
  g_ClientDeveloperTool = this;
}
//-----------------------------------------------------------------------
IClientDeveloperTool::~IClientDeveloperTool()
{
  g_ClientDeveloperTool = NULL;
}
//-----------------------------------------------------------------------
IClientDeveloperTool* IClientDeveloperTool::Singleton()
{
  return g_ClientDeveloperTool;
}
//-----------------------------------------------------------------------
void IClientDeveloperTool::HandleEvent(TEvent* pEvent)// если необходимо прервать работу движка - вернуть false
{
  switch(pEvent->from)
  {
    case ID_SRC_EVENT_GRAPHIC_ENGINE:
    {
      int s;
      TBaseEvent* pData = (TBaseEvent*)pEvent->container.GetData(s);
      switch(pData->type)
      {
        case eKeyBoard:
        {
          TKeyEvent* pKey = (TKeyEvent*)pData;
          KeyEvent(pKey);
          break;
        }
        case eMouse:
        {
          TMouseEvent* pMouse = (TMouseEvent*)pData;
          MouseEvent(pMouse);
          break;
        }
      }
    }
  }
  Event(pEvent);
}
//-----------------------------------------------------------------------
