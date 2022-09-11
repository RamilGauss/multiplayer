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
#include "InputCmdDevTool.h"
#include "Events.h"

class SlaveForm;

class TServerDeveloperTool_SlaveTank : public IServerDeveloperTool
{
  SlaveForm* mSlaveForm;

  TInputCmdDevTool mInputCmd;
public:
  TServerDeveloperTool_SlaveTank();
  virtual ~TServerDeveloperTool_SlaveTank();

	virtual void Init(TComponentServer* pComponent, std::vector<std::string>& arg);
	virtual int GetTimeRefreshMS();
	//virtual void Refresh();

	virtual void Done();
	virtual void Event(nsEvent::TEvent* pEvent);

	virtual IMakerObjectCommon* GetMakerObjectCommon();
	virtual std::string GetPathXMLFile();

protected:
  void ParseCmd(std::vector<std::string>& arg);
  void InitLog();

  void HandleFromMelissa(nsMelissa::TBaseEvent* pBE);
  void HandleFromQt(nsEvent::TEvent* pEvent);

  void InitQtForm();
  void ConnectUpQt();
  void DisconnectUpQt();

  void ConnectUp(nsMelissa::TEventConnectUp* pBE);
  void DisconnectUp(nsMelissa::TEventDisconnectUp* pBE);

};

#endif
