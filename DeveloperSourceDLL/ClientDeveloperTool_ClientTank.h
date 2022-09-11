/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
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

class TClientMain;
class TGameRoomPrepare;
class TWaitForm;


class TClientDeveloperTool_ClientTank : public IClientDeveloperTool
{
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

  virtual void Init(TComponentClient* pComponent, const char* arg = NULL);

  virtual std::string GetTitleWindow();
  
	virtual void Event(nsEvent::TEvent* pEvent);
  virtual void MouseEvent(nsEvent::TMouseEvent* pEvent);
  virtual void KeyEvent(nsEvent::TKeyEvent* pEvent);

  virtual void PrepareForRender();
  virtual void Done();
  
	virtual IMakerObjectCommon* GetMakerObjectCommon();
  virtual std::string GetPathXMLFile();

protected:
  void InitLog();
  void HandleFromMelissa(nsMelissa::TBaseEvent* pBE);
};

#endif