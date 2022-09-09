/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012
===========================================================================
                        Common Information
"Tanks" GPL Source Code

This file is part of the "Tanks" GPL Source Code.

"Tanks" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"Tanks" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "Tanks" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "Tanks" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "Tanks" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 

#ifndef IClientDeveloperToolH
#define IClientDeveloperToolH

#include "IDeveloperTool.h"
#include "DstEvent.h"
#include <string>

class IMakerObjectCommon;

class IGUI;
class TNET_LevelClientServer;
class IPhysicEngine;
class IGraphicEngine; // отрисовка сцены
class IManagerObjectCommon;
class IManagerTime;

class IClientDeveloperTool : public IDeveloperTool
{
protected:
  IMakerObjectCommon* mMakerObjectCommon;
public:
  struct TComponentClient
  {
    //IReplay*              mReplay;        // HDD
    //ISoundEngine*         mSound;         // Sound 
    IGUI*                   mGUI;           // GUI, MyGUI!
    TNET_LevelClientServer* mNET;           // Melissa
    IPhysicEngine*          mPhysicEngine;  // Robert
    IGraphicEngine*         mGraphicEngine; // BigJack отрисовка сцены
    IManagerObjectCommon*   mMOC;           // GameLib
    IManagerTime*           mMTime;         // GameLib
    TComponentClient()
    {
      //mReplay = NULL;      // HDD
      //mSound = NULL;       // Sound 
      mGUI           = NULL; // MyGUI
      mNET           = NULL; // Melissa
      mPhysicEngine  = NULL; // Robert
      mGraphicEngine = NULL; // BigJack отрисовка сцены
      mMOC           = NULL; // GameLib
      mMTime         = NULL; // GameLib
    }
  };
protected:
  // компоненты
  TComponentClient mComponent;
  
  typedef void (*TInitLogFunc)(char*); 
  TInitLogFunc mFuncInitLogger;
public:

  IClientDeveloperTool();
  virtual ~IClientDeveloperTool();

  virtual void Init(TComponentClient* pComponent) = 0;

  virtual std::string GetTitleWindow() = 0;

  virtual bool HandleEvent(TEvent* pEvent) = 0;// если необходимо прервать работу движка - вернуть false

  virtual void Calc() = 0;
  virtual IMakerObjectCommon* GetMakerObjectCommon() = 0;

  virtual void SetInitLogFunc(TInitLogFunc pFunc){mFuncInitLogger=pFunc;}

  // доступ к компонентам
  TComponentClient* GetComponent(){return &mComponent;}


  static IClientDeveloperTool* GetSingleton();
};

//extern IClientDeveloperTool* g_ClientDeveloperTool;

#endif
