/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
??????? ?????? ????????? 
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


#ifndef GameRoomPrepareH
#define GameRoomPrepareH

#include <QWidget>
#include "ui_GameRoomPrepare.h"
#include "ApplicationProtocolPacketAnswer.h"
#include "ManagerGUIClient.h"

class QTimer;

class TGameRoomPrepare : public TBaseGUI
{
  Q_OBJECT

	enum{
		eTimeRefreshCntClient = 1000,
	};

	volatile unsigned int mCntCommon;
	volatile unsigned int mCntInFight;

  TA_Get_List_Tank mPacketListTank;

  QTimer* mTimer;
  
public:
  TGameRoomPrepare(QWidget *parent = NULL);
  ~TGameRoomPrepare();

  virtual void Translate(unsigned short type, char*pData, int size);
  virtual void showGUI();
  virtual void hideGUI();

protected:
	void SetCntClient(unsigned int common,unsigned int fight);

	virtual void closeEvent(QCloseEvent* );

  void SendRequestListTank();
  void ViewListTanks();
  void AddListTank(char* data, int size);

protected slots:
	void sl_RefreshCntClient();
	void sl_Exit();
  void sl_ChangeCurTank();
  void sl_InFight();

private:
  Ui::GameRoomPrepareClass ui;
};

#endif
