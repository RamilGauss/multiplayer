/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
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
class IControlCamera;
class IGUI;
class IGraphicEngine;

class IClientDeveloperTool : public IDeveloperTool
{
public:
  struct TComponentClient : public TComponent
  {
    IControlCamera*         mControlCamera; // Camera
    IGUI*                   mGUI;           // GUI, MyGUI!
    IGraphicEngine*         mGraphicEngine; // BigJack ��������� �����
    IManagerStateMachine*   mMStateMachine; // �������� �������, ��� HotKeys, Net (��������� �������)
    nsMMOEngine::TClient*     mNetClient;   // MMOEngine
    IPhysicEngine*          mPhysicEngine;  // PhysicEngine
    IManagerObjectCommon*   mMOC;           // GameLib
    IManagerTime*           mMTime;         // GameLib
    TComponentClient()
    {
      mControlCamera = NULL; 
      mGraphicEngine = NULL; 
      mGUI           = NULL; 
      mMStateMachine = NULL; 
      mNetClient     = NULL; 
      mPhysicEngine  = NULL; 
      mMOC           = NULL; 
      mMTime         = NULL; 
    }
  };
protected:
  // ����������
  TComponentClient mComponent;
 
public:

  IClientDeveloperTool();
  virtual ~IClientDeveloperTool();

  virtual void Init(TComponentClient* pComponent, std::vector<std::string>& arg) = 0;
  virtual std::string GetTitleWindow() = 0;

  // ������ � �����������
  TComponentClient* GetComponent(){return &mComponent;}

  static IClientDeveloperTool* Singleton();
};


#endif
