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


#include "GameRoomPrepare.h"
#include "ApplicationProtocolDef.h"
#include "GlobalParamsTank.h"
#include "BL_Debug.h"
#include "ApplicationProtocolPacketStream.h"
#include <QMessageBox>
#include <QTimer>
#include "ApplicationProtocolPacketAnswer.h"
#include "TypeTank.h"
#include "ClientTank.h"


//-------------------------------------------------------------------------------------------
TGameRoomPrepare::TGameRoomPrepare(QWidget *parent)
: TBaseGUI(parent)
{
  mTimer = new QTimer(this);
  connect(mTimer, SIGNAL(timeout()), this, SLOT(sl_RefreshCntClient()));

  mCntCommon = 0;
	mCntInFight = 0;

  ui.setupUi(this);
  setWindowTitle(tr("Комната ")+tr(STR_VERSION_CLIENT));

  connect(ui.cbTank,SIGNAL(activated(int)), this, SLOT(sl_ChangeCurTank()));
  connect(ui.bFight,SIGNAL(clicked()), this, SLOT(sl_InFight()));
	connect(ui.bExit,  SIGNAL(clicked()), this, SLOT(sl_Exit()));
}
//---------------------------------------------------------------------------------------------
TGameRoomPrepare::~TGameRoomPrepare()
{
}
//---------------------------------------------------------------------------------------------
void TGameRoomPrepare::SetCntClient(unsigned int common,unsigned int fight)
{
	mCntCommon = common;
	mCntInFight = fight;
}
//---------------------------------------------------------------------------------------------
void TGameRoomPrepare::sl_RefreshCntClient()
{
	QString str;
	str = QString(tr("В бою: %1")).arg(mCntInFight);
	ui.lInFight->setText(str);
	
	str = QString(tr("Всего: %1")).arg(mCntCommon);
	ui.lAll->setText(str);
}
//---------------------------------------------------------------------------------------------
void TGameRoomPrepare::closeEvent(QCloseEvent* )
{
	sl_Exit();
}
//---------------------------------------------------------------------------------------------
void TGameRoomPrepare::sl_Exit()
{
	_exit(0);
}
//---------------------------------------------------------------------------------------------
void TGameRoomPrepare::SendRequestListTank()
{
  pClient->SendRequestListTank();
}
//---------------------------------------------------------------------------------------------
void TGameRoomPrepare::ViewListTanks()
{
  for(int i = 0 ; i < mPacketListTank.getCnt() ; i++)
  {
    unsigned short typeTank = mPacketListTank.getTypeTank(i);
    switch(typeTank)
    {
      case nsTank_ID::eID_KingTiger:
        if(mPacketListTank.getFlgBlockTank(i))
          ui.cbTank->addItem(tr("KingTiger заблокирован"));
        else
           ui.cbTank->addItem(tr("KingTiger"));
          break;

      //  case nsTank_ID::eID_Tiger:
				//if(mPacketListTank.getFlgBlockTank(i))
    //      ui.cbTank->addItem(tr("Tiger заблокирован"));
				//else
	   //     ui.cbTank->addItem(tr("Tiger"));
    //    break;
    //  case nsTank_ID::eID_IS:
				//if(mPacketListTank.getFlgBlockTank(i))
				//	ui.cbTank->addItem(tr("ИС заблокирован"));
				//else
				//	ui.cbTank->addItem(tr("ИС"));
    //    break;
      default:BL_FIX_BUG();
    }
  }
}
//---------------------------------------------------------------------------------------------
void TGameRoomPrepare::AddListTank(char* data, int size)
{
  mPacketListTank.setData(data,size);
}
//---------------------------------------------------------------------------------------------
void TGameRoomPrepare::sl_ChangeCurTank()
{
  int i = ui.cbTank->currentIndex();
  if(i==-1) return;
  pClient->SetCurrentTank(i);
}
//---------------------------------------------------------------------------------------------
void TGameRoomPrepare::sl_InFight()
{
  pClient->SendRequestInFight();
}
//---------------------------------------------------------------------------------------------
void TGameRoomPrepare::Translate(unsigned short type, char*pData, int size)
{
  switch(type)
  {
		case APPL_TYPE_S_GARAGE:
		{
			TS_Garage S_Garage;
			S_Garage.setData(pData,size);
			SetCntClient(S_Garage.getCommon(),S_Garage.getFight());
			break;
		}
		case APPL_TYPE_A_GET_LIST_TANK:
		{
			AddListTank(pData, size);
      ViewListTanks();
			break;
		}
  	default:;
  }
}
//---------------------------------------------------------------------------------------------
void TGameRoomPrepare::showGUI()
{
  ui.cbTank->clear();
  sl_RefreshCntClient();

  // запуск таймера для обновления списка стрима
  mTimer->start(eTimeRefreshCntClient);

  SendRequestListTank();

  TBaseGUI::showGUI();
}
//---------------------------------------------------------------------------------------------
void TGameRoomPrepare::hideGUI()
{
  mTimer->stop();
  TBaseGUI::hideGUI();
}
//---------------------------------------------------------------------------------------------