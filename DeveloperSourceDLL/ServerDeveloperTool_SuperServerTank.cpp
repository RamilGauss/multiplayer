/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ServerDeveloperTool_SuperServerTank.h"
#include "MakerObjectCommon.h"

using namespace std;

TServerDeveloperTool_SuperServerTank::TServerDeveloperTool_SuperServerTank()
{
	mMakerObjectCommon = new TMakerObjectCommon;
}
//---------------------------------------------------------------------------------
TServerDeveloperTool_SuperServerTank::~TServerDeveloperTool_SuperServerTank()
{
  delete mMakerObjectCommon;
}
//---------------------------------------------------------------------------------
void TServerDeveloperTool_SuperServerTank::Init(TComponentServer* pComponent, const char* arg)
{
  mComponent = *pComponent;
}
//---------------------------------------------------------------------------------
int TServerDeveloperTool_SuperServerTank::GetTimeRefreshMS()// как часто происходит вызов Refresh()
{
	return 100;
}
//---------------------------------------------------------------------------------
void TServerDeveloperTool_SuperServerTank::Refresh()
{

}
//---------------------------------------------------------------------------------
IMakerObjectCommon* TServerDeveloperTool_SuperServerTank::GetMakerObjectCommon()
{
	return mMakerObjectCommon;
}
//------------------------------------------------------------------------------------
string TServerDeveloperTool_SuperServerTank::GetPathXMLFile()
{
	return "resources.xml";
}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_SuperServerTank::Done()
{

}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_SuperServerTank::Event(nsEvent::TEvent* pEvent)
{

}
//---------------------------------------------------------------------------------------------
void TServerDeveloperTool_SuperServerTank::InitLog()
{
  if(mFuncGetLogger)
  {
    mFuncGetLogger()->Register("Inner");// для логирования внутренних событий
    mFuncGetLogger()->Init("SuperServerTank");
  }
}
//---------------------------------------------------------------------------------------------
