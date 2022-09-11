/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef ServerDeveloperTool_MasterTankH
#define ServerDeveloperTool_MasterTankH

#include "../GameLib/IServerDeveloperTool.h"
#include "BaseEvent.h"
#include "Events.h"

class TServerDeveloperTool_MasterTank : public IServerDeveloperTool
{
  unsigned int mCounterClient;
public:
  TServerDeveloperTool_MasterTank();
  virtual ~TServerDeveloperTool_MasterTank();

	virtual void Init(TComponentServer* pComponent, const char* arg = NULL);
	virtual int GetTimeRefreshMS();// как часто происходит вызов Refresh()
	virtual void Refresh();

	virtual void Done();
	virtual void Event(nsEvent::TEvent* pEvent);

	virtual IMakerObjectCommon* GetMakerObjectCommon();
	virtual std::string GetPathXMLFile();

protected:
  void InitLog();
  void HandleFromMelissa(nsMelissa::TBaseEvent* pBE);
  
  void TryLogin(nsMelissa::TEventTryLogin* pEvent);
};

#endif
