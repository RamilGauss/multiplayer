/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef ClientDeveloperTool_ClientTankH
#define ClientDeveloperTool_ClientTankH

#include "../GameLib/IClientDeveloperTool.h"

#include <boost/smart_ptr/scoped_ptr.hpp>

#include "ClientMain.h"
#include "GameRoomPrepare.h"
#include "WaitForm.h"
#include "../Melissa/Include/BaseEvent.h"
#include "InputCmdDevTool.h"

class TClientMain;
class TGameRoomPrepare;
class TWaitForm;


class TClientDeveloperTool_ClientTank : public IClientDeveloperTool
{
  TInputCmdDevTool mInputCmd;

  TClientMain*      mClientMain;
  TGameRoomPrepare* mGameRoomPrepare;
  TWaitForm*        mWaitForm;

  bool flgDragCamera;
  int mOldX;
  int mOldY;

  unsigned int mIDkey;

  int mIndexCurObj;

public:
  TClientDeveloperTool_ClientTank ();
  virtual ~TClientDeveloperTool_ClientTank ();

  virtual void Init(TComponentClient* pComponent, std::vector<std::string>& arg);

  virtual std::string GetTitleWindow();
  
	virtual void Event(nsEvent::TEvent* pEvent);
  virtual void Done();
  
	virtual IMakerObjectCommon* GetMakerObjectCommon();
  virtual std::string GetPathXMLFile();

protected:
  void ParseCmd(std::vector<std::string>& arg);

  void InitLog();
  void HandleFromMelissa(nsMelissa::TBaseEvent* pBE);
  void HandleFromGUI(nsEvent::TBaseEvent* pData);

	void MouseEvent(nsEvent::TMouseEvent* pEvent);
	void KeyEvent(nsEvent::TKeyEvent* pEvent);
};

#endif
