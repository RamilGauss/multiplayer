/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
2011, 2012, 2013
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

#ifndef ClientGameH
#define ClientGameH

#include <vector>
#include <string>
#include <map>

#include "IGame.h"
#include "WrapperMakerTransport.h"

/*
1. ������ ��������� ������� ���-�� ����������� (� ������������ �� ����� ���������������).
2. ���� ���������� ��������, �������� � ���������������� ������� ����� ������������.
3. �������� ������� "�������� �������" SrcEvent � "�����������" DstEvent.

*/
struct TDescThread;

class TClientGame : public IGame
{
protected:
  friend struct TDescThread;

  IClientDeveloperTool::TComponentClient mCClient;

  friend void* ThreadModule(void* p);
	
	typedef bool(TClientGame::*FuncHandleEvent)();

  typedef std::vector<FuncHandleEvent> TVectorFunc;
  typedef std::vector<TDescThread>     TVectorDT;
  TVectorFunc mMainThreadVecModule;
  TVectorDT   mOtherThreadVecModule;

  enum{
    eSleepNE      = 30,
    eWaitFeedBack = 30,
  };
public:
  TClientGame();
  virtual ~TClientGame();

  virtual void Work(int variant_use, const char* sNameDLL, const char* arg = NULL);// ������ ������
  
protected:
  bool Init(int variant_use, const char* sNameDLL, const char* arg = NULL);
  void Done();

  void MakeVectorModule();
  bool MakeEventFromModule();
  
  bool HandleGraphicEngineEvent();
  bool HandleNetEngineEvent();

  void HandleEventByDeveloper();
  void CollectEvent();

  void PrepareForRender();
  void Render();

  void HandleEvent(nsEvent::TEvent* pEvent);

  void StartThreadModule();
  void StopThreadModule();
};
//---------------------------------------------------------
struct TDescThread
{
  TClientGame*                 pClientGame;
  TClientGame::FuncHandleEvent pFunc;
  int             sleep_ms;
  volatile bool   flgActive;
  volatile bool   flgNeedStop;
  TDescThread()
  {
    pClientGame = NULL;
    pFunc       = NULL;
    sleep_ms    = 20;
    flgActive   = false;
    flgNeedStop = false;
  }
  void Work();
};
#endif
