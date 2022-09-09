/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
2011, 2012
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
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 

#ifndef ClientGameH
#define ClientGameH

#include "IClientDeveloperTool.h"
#include "DstEvent.h"
#include "IGame.h"

/*
1. ������ ��������� ������� ���-�� ����������� (� ������������ �� ����� ���������������).
2. ���� ���������� ��������, �������� � ���������������� ������� ����� ������������.
3. �������� ������� "�������� �������" SrcEvent � "�����������" DstEvent.

*/

class IControlCamera;
class IGUI;
class TNET_LevelClientServer;
class IPhysicEngine;
class IGraphicEngine; // ��������� �����
class IManagerObjectCommon;
class IManagerTime;

class TClientGame : public IGame, public TDstEvent
{
protected:
  volatile bool flgNeedStop;
  volatile bool flgActive;

  //IReplay*              mReplay;        // HDD
  //ISoundEngine*         mSound;         // Sound 
  //IManagerStateMachine*   mManagerStateMachine;// �������� ��������� ��� MapHotKey

  IControlCamera*         mControlCamera; // ��� �������� ������
  IGUI*                   mGUI;           // GUI, MyGUI! - �� �������� ���������� �������
  TNET_LevelClientServer* mNET;           // Melissa
  IPhysicEngine*          mPhysicEngine;  // Robert
  IGraphicEngine*         mGraphicEngine; // BigJack ��������� �����
  IManagerObjectCommon*   mMOC;           // GameLib
  IManagerTime*           mMTime;         // GameLib

public:
  TClientGame();
  virtual ~TClientGame();

  void Work(const char* sNameDLL);// ������ ������
  
protected:
  bool Init(const char* sNameDLL);
  void Done();

  bool HandleGraphicEngineEvent();
  bool HandleExternalEvent();
  void CollectEvent();
  void Calc();
  void Render();

  bool HandleEvent(nsEvent::TEvent* pEvent);


  void InitLog();
};

#endif
