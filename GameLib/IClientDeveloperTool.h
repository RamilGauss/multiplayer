/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef IClientDeveloperToolH
#define IClientDeveloperToolH

#include <string>

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
  struct TComponentClient : public TComponent
  {
    IManagerStateMachine*   mMStateMachine; // �������� �������, ��� HotKeys, Net (��������� �������)
    nsMelissa::TClient*     mNetClient;     // Melissa
    IPhysicEngine*          mPhysicEngine;  // Robert
    IManagerObjectCommon*   mMOC;           // GameLib
    IManagerTime*           mMTime;         // GameLib
    TComponentClient()
    {
      mMStateMachine = NULL; // �������� �������, ��� HotKeys
      mNetClient     = NULL; // Melissa
      mPhysicEngine  = NULL; // Robert
      mMOC           = NULL; // GameLib
      mMTime         = NULL; // GameLib
    }
  };
protected:
  // ����������
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

  // ������ � �����������
  TComponentClient* GetComponent(){return &mComponent;}

  static IClientDeveloperTool* Singleton();
};


#endif
