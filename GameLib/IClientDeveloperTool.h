/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef IClientDeveloperToolH
#define IClientDeveloperToolH

#include <string>
#include <vector>

#include "IDeveloperTool.h"
#include "DstEvent.h"
#include "Client.h"

class TLogger;
class IMakerObjectCommon;
class IManagerStateMachine;
class IPhysicEngine;
class IManagerObjectCommon;
class IManagerTime;

class IClientDeveloperTool : public IDeveloperTool
{
public:
  struct TComponentClient
  {
    IControlCamera*         mControlCamera; // Camera
    IGUI*                   mGUI;           // GUI, MyGUI!
    IGraphicEngine*         mGraphicEngine; // BigJack отрисовка сцены
    IManagerStateMachine*   mMStateMachine; // конечный автомат, для HotKeys, Net (обработка пакетов)
    nsMelissa::TClient*     mNetClient;     // Melissa
    IPhysicEngine*          mPhysicEngine;  // Robert
    IManagerObjectCommon*   mMOC;           // GameLib
    IManagerTime*           mMTime;         // GameLib
    TComponentClient()
    {
      mControlCamera = NULL; // Camera
      mGraphicEngine = NULL; // BigJack отрисовка сцены
      mGUI           = NULL; // MyGUI
      mMStateMachine = NULL; // конечный автомат, для HotKeys
      mNetClient     = NULL; // Melissa
      mPhysicEngine  = NULL; // Robert
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

  virtual void Init(TComponentClient* pComponent, std::vector<std::string>& arg) = 0;
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
