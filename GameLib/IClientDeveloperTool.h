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

#ifndef IClientDeveloperToolH
#define IClientDeveloperToolH

#include <string>

#include "IDeveloperTool.h"
#include "DstEvent.h"
#include "Client.h"

class TLogger;
class IMakerObjectCommon;
class IControlCamera;
class IManagerStateMachine;
class IGUI;
class IPhysicEngine;
class IGraphicEngine; // отрисовка сцены
class IManagerObjectCommon;
class IManagerTime;

class IClientDeveloperTool : public IDeveloperTool
{
public:
  struct TComponentClient
  {
    IControlCamera*         mControlCamera; // Camera
    IManagerStateMachine*   mMStateMachine; // конечный автомат, для HotKeys, Net (обработка пакетов)
    IGUI*                   mGUI;           // GUI, MyGUI!
    nsMelissa::TClient*     mNetClient;     // Melissa
    IPhysicEngine*          mPhysicEngine;  // Robert
    IGraphicEngine*         mGraphicEngine; // BigJack отрисовка сцены
    IManagerObjectCommon*   mMOC;           // GameLib
    IManagerTime*           mMTime;         // GameLib
    TComponentClient()
    {
      mControlCamera = NULL; // Camera
      mMStateMachine = NULL; // конечный автомат, для HotKeys
      mGUI           = NULL; // MyGUI
      mNetClient     = NULL; // Melissa
      mPhysicEngine  = NULL; // Robert
      mGraphicEngine = NULL; // BigJack отрисовка сцены
      mMOC           = NULL; // GameLib
      mMTime         = NULL; // GameLib
    }
  };
protected:
  // компоненты
  TComponentClient mComponent;
 
public:

  IClientDeveloperTool();
  virtual ~IClientDeveloperTool();

  virtual void Init(TComponentClient* pComponent, const char* arg = NULL) = 0;
  virtual std::string GetTitleWindow() = 0;

  virtual void PrepareForRender() = 0;

	virtual void MouseEvent(nsEvent::TMouseEvent* pEvent){}
	virtual void KeyEvent(nsEvent::TKeyEvent* pEvent){}

  void HandleEvent(nsEvent::TEvent* pEvent);

  // доступ к компонентам
  TComponentClient* GetComponent(){return &mComponent;}

  static IClientDeveloperTool* Singleton();
};


#endif
