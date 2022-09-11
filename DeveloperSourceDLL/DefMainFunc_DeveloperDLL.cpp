/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "../GameLib/DeveloperTool_DLL.h"
#include <stddef.h>
#include "ClientDeveloperTool_ViewerModel.h"

#include "ServerDeveloperTool_SlaveTank.h"
#include "ServerDeveloperTool_MasterTank.h"
#include "ServerDeveloperTool_SuperServerTank.h"
#include "ClientDeveloperTool_ClientTank.h"


DllExport IClientDeveloperTool* GetClientDeveloperTool(int variant_use)
{
  switch(variant_use)
  {
    case 0:
      return new TClientDeveloperTool_ViewerModel;
    case 1:
      return new TClientDeveloperTool_ClientTank;
  }
  return NULL;
}
//-------------------------------------------------------------------
DllExport IServerDeveloperTool* GetServerDeveloperTool(int variant_use)
{
  switch(variant_use)
  {
    case 0:
      return new TServerDeveloperTool_SlaveTank;
    case 1:
      return new TServerDeveloperTool_MasterTank;
    case 2:
      return new TServerDeveloperTool_SuperServerTank;
  }
  return NULL;
}
//-------------------------------------------------------------------
DllExport void FreeDeveloperTool(IDeveloperTool* p)
{
  delete p;
}
//-------------------------------------------------------------------
