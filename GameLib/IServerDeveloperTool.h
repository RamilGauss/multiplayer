/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012, 2013, 2013, 2013
===========================================================================
                        Common Information
"TornadoEngine" GPL Source Code

This file is part of the "TornadoEngine" GPL Source Code.

"TornadoEngine" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"TornadoEngine" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "TornadoEngine" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "TornadoEngine" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "TornadoEngine" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 

#ifndef IServerDeveloperToolH
#define IServerDeveloperToolH

#include "IDeveloperTool.h"
#include "Slave.h"
#include "SuperServer.h"
#include "Master.h"

class IMakerObjectCommon;
class IControlCamera;
class IManagerStateMachine;
class IGUI;
class IManagerScene;
class IGraphicEngine; // отрисовка сцены
class IManagerObjectCommon;
class IManagerConnectClient;

class IServerDeveloperTool : public IDeveloperTool
{
public:
	struct TComponentServer
	{
		union
		{
			nsMelissa::TSlave*       Slave;      // Melissa
			nsMelissa::TMaster*      Master;     // Melissa
			nsMelissa::TSuperServer* SuperServer;// Melissa
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
