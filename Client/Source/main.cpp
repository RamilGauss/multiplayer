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
the "Tanks" Source Code.  If not, please request a copy in writing from id Software at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 

#include "DXUT.h"
#include <QtGui/QApplication>

#include <QtCore/QTextCodec>
#include <QWindowsStyle>
#include "HiTimer.h"
#include "NetSystem.h"
#include "ErrorReg.h"
#include <glib/gthread.h>
#include "ManagerGUIClient.h"
#include "GameRoomPrepare.h"
#include "ClientMain.h"
#include "WaitForm.h"
#include "GameForm.h"
#include "ClientTank.h"
#include "ManagerGUI.h"
#include "ManagerObjectCommonClient.h"


int main(int argc, char *argv[])
{
  g_thread_init( NULL );
  err_Init();
  errSTR_Init();
  errSTD_Init();
  errSDK_Init();
  ht_Init();
  ns_Init();

  QTextCodec::setCodecForTr(QTextCodec::codecForName("CP1251"));

  QApplication a(argc, argv);
  //---------------------------------------
  // что бы не смущать стек - создадим все объекты в Heap-е
  TManagerGUIClient* pManagerGUI = new TManagerGUIClient;
  TClientTank*       pClientTank = new TClientTank;// транспорт
  // менеджер двигателей
  TManagerObjectCommon* pManagerObjectCommon = new TManagerObjectCommonClient;

  pManagerGUI->startGUI(pClientTank,pManagerObjectCommon);
  // формы:
  // обычные 
  TGameRoomPrepare* pGameRoomPrepare = new TGameRoomPrepare;
  TWaitForm*        pWaitForm        = new TWaitForm;
  TClientMain*      pClientMain      = new TClientMain;
  // DirectX формы
  TGameForm*        pGameForm = new TGameForm;
  pManagerGUI->AddFormInList(pClientMain,     "clientMain");// первая форма
  pManagerGUI->AddFormInList(pWaitForm,       "waitForm");
  pManagerGUI->AddFormInList(pGameRoomPrepare,"gameRoomPrepare");
  pManagerGUI->AddFormInList(pGameForm,       "gameForm");

  if(argc==4)
  {
    char* sNick = argv[1];
    char*   sIP = argv[2]; 
    char*  port = argv[3]; 
    pClientMain->Connect(sNick,sIP,port);
  }
	a.exec();

  // что бы я не делал - линковщик не видит деструктор класса, определенного в исполняемом файле из под либины
  // убить всех: (блин почему это не C#? всех бы убил сборщик мусора)
  delete pGameRoomPrepare;
  delete pWaitForm;
  delete pClientMain;

  delete pGameForm;

  delete pManagerGUI;
  delete pClientTank;
  delete pManagerObjectCommon;


	return 0;
}
