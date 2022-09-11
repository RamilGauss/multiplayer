/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef ServerDeveloperTool_SlaveTankH
#define ServerDeveloperTool_SlaveTankH

#include "../GameLib/IServerDeveloperTool.h"
#include "BaseEvent.h"

class TServerDeveloperTool_SlaveTank : public IServerDeveloperTool
{

public:
  TServerDeveloperTool_SlaveTank();
  virtual ~TServerDeveloperTool_SlaveTank();

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
};

#endif
