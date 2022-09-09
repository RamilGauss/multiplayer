
#include "IClientDeveloperTool.h"
#include "BL_Debug.h"

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
