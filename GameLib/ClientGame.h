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

#ifndef ClientGameH
#define ClientGameH


#include "ManagerObjectCommon.h"
#include "IPhysicEngine.h"
#include "INET_Engine.h"


/*
    Краткая концепция
Постулат: Менеджер компонентов (МК) является игровым движком.
От него наследуется Клиент и Сервер. Это два движка.

Исходя из этого:
1. Должен содержать строгое кол-во компонентов (в соответствии со своим предназначением).
2. Ядро МК занимается рендером, расчетом и диспетчеризацией событий между компонентами
3. Вводится понятие "Источник событий" SrcEvent и "Поглотитель событий" DstEvent.
4. 
*/


class TClientGame : public IGame
{
protected:
  volatile bool flgNeedStop;
  volatile bool flgActive;



  //IAI*               mAI;
  //IReplay*           mReplay;
  //ISoundEngine*      mSound;
  INET_Engine*         mNET;
  IPhysicEngine*       mPhysicEngine;
  IGraphicEngine*      mGraphicEngine; // отрисовка сцены
  TManagerGUI          mMGUI;
  TManagerKeyMouse     mMKM;
  TManagerObjectCommon mMOC;
  TManagerTime         mMTime;


public:
  TClientGame();
  virtual ~TClientGame();

  void Work(const char* sNameDLL);// начало работы
  
  // диспетчеризация событий
  // решение принимается на основании скрипта (что-то типа машины состояния)
  void SetEvent(const char* sFrom, unsigned int key, void* pData, int sizeData);

protected:
  void Init(const char* sNameDLL);
  void Done();


  typedef enum{ eNoWindowEvent,
        eQuit,
      } tResHandleWindowEvent;


  tResHandleWindowEvent HandleWindowEvent();
  void HandleExternalEvent();
  void Calc();
  void Render();

};

#endif
