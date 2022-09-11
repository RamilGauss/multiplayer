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
    nsMelissa::TClient*     mNetClient;     // Melissa
    IPhysicEngine*          mPhysicEngine;  // Robert
    IManagerObjectCommon*   mMOC;           // GameLib
    IManagerTime*           mMTime;         // GameLib
    TComponentClient()
    {
      mControlCamera = NULL; // Camera
      mGraphicEngine = NULL; // BigJack ��������� �����
      mGUI           = NULL; // MyGUI
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

  virtual void Init(TComponentClient* pComponent, std::vector<std::string>& arg) = 0;
  virtual std::string GetTitleWindow() = 0;

  // ������ � �����������
  TComponentClient* GetComponent(){return &mComponent;}

  static IClientDeveloperTool* Singleton();
};


#endif
