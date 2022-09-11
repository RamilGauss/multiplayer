/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef IServerDeveloperToolH
#define IServerDeveloperToolH

#include "IDeveloperTool.h"
#include "Slave.h"
#include "SuperServer.h"
#include "Master.h"

class IMakerObjectCommon;
class IManagerStateMachine;
class IManagerScene;
class IManagerObjectCommon;
class IManagerConnectClient;

class IServerDeveloperTool : public IDeveloperTool
{
public:
	struct TComponentServer : public TComponent
	{
		union
		{ // Melissa
			nsMelissa::TSlave*       Slave;      
			nsMelissa::TMaster*      Master;     
			nsMelissa::TSuperServer* SuperServer;
			nsMelissa::TBase*        Base;
		}mNet;
		IManagerScene*             mManagerScene;  // только для Slave
    IManagerConnectClient*     mManagerCClient;// клиенты, которые подсоединились
		TComponentServer()
		{
			mNet.Base       = NULL; // Melissa
			mManagerScene   = NULL; 
      mManagerCClient = NULL;
		}
	};
protected:
	// компоненты
	TComponentServer mComponent;

public:
  IServerDeveloperTool();
  virtual ~IServerDeveloperTool();
  
	virtual void Init(TComponentServer* pComponent, const char* arg = NULL) = 0;
	// как часто происходит вызов Refresh(), временной интервал работы сервера
  virtual int GetTimeRefreshMS() = 0;
  // вызов не более одного раз в GetTimeRefreshMS
	virtual void Refresh() = 0;
	// доступ к компонентам
	TComponentServer* GetComponent(){return &mComponent;}

	static IServerDeveloperTool* Singleton();
};

#endif
