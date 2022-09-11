/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef ServerGameH
#define ServerGameH

#include <vector>
#include <string>
#include <map>

#include "IGame.h"
#include "StatisticValue.h"

class TServerGame : public IGame
{
protected:

	TSaveOnHDD mLogLoad;

	typedef TStatType_double TStatLoad;
	TStatLoad mStatLoad;

  IServerDeveloperTool::TComponentServer mCServer;

	unsigned int mStartTime;
public:
	typedef enum{
		eSlave,eMaster,eSuperServer,
	}eTypeRealize;
  TServerGame(eTypeRealize type);
  virtual ~TServerGame();
protected:

	eTypeRealize mType;

	virtual bool Init(int variant_use, 
                    const char* sNameDLL, 
                    const char* arg = NULL);
	virtual void Done();
  virtual bool Work();

	virtual void MakeVectorModule();

	virtual void CollectEvent();
	virtual void HandleEvent(nsEvent::TEvent* pEvent);

  void Render();

  bool HandleGraphicEngineEvent();
  bool HandleNetEngineEvent();
  bool HandleSceneEvent();

  void CalcAndWaitRestTime();
	void SetLoad();
};
//---------------------------------------------------------
#endif
