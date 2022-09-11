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
#include "InputCmdDevTool.h"
#include "ListMultiThread.h"

class MasterForm;

class TServerDeveloperTool_MasterTank : public IServerDeveloperTool
{
  TListMultiThread<unsigned int> mListID_SessionAdd;
  TListMultiThread<unsigned int> mListID_SessionDelete;

  unsigned int mCounterClient;

  MasterForm* mMasterForm;

  TInputCmdDevTool mInputCmd;
public:
  TServerDeveloperTool_MasterTank();
  virtual ~TServerDeveloperTool_MasterTank();

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
  void AddSlaveQt();
  void DeleteSlaveQt();

  void TryLogin(nsMelissa::TEventTryLogin* pEvent);
  void ConnectDown(nsMelissa::TEventConnectDown* pEvent);
  void DisconnectDown(nsMelissa::TEventDisconnectDown* pEvent);

  void ConnectUp(nsMelissa::TEventConnectUp* pBE);
  void DisconnectUp(nsMelissa::TEventDisconnectUp* pBE);
};

#endif
