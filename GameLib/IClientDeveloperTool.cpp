/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012
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
you may contact in writing [ramil2085@gmail.com].
===========================================================================
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
IClientDeveloperTool* IClientDeveloperTool::GetSingleton()
{
  return g_ClientDeveloperTool;
}
//-----------------------------------------------------------------------
bool IClientDeveloperTool::HandleEvent(TEvent* pEvent)// если необходимо прервать работу движка - вернуть false
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
          if(KeyEvent(pKey)==false)
            return false;
          break;
        }
        case eMouse:
        {
          TMouseEvent* pMouse = (TMouseEvent*)pData;
          if(MouseEvent(pMouse)==false)
            return false;
          break;
        }
      }
    }
  }
  return Event(pEvent);
}
//-----------------------------------------------------------------------
