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


#include "ClientMain.h"
#include "NetSystem.h"
#include "ClientTank.h"
#include "ApplicationProtocolDef.h"
#include "WaitForm.h"
#include <QMessageBox>
#include <QSettings>
#include <QTimer>
#include "ApplicationProtocolPacketAnswer.h"
#include "GameRoomPrepare.h"
#include "ApplicationProtocolPacketStream.h"
#include "GameForm.h"


//-------------------------------------------------------------------------------------------
TWaitForm::TWaitForm(QWidget *parent)
: TBaseGUI(parent)
{
  mWait   = 0;
  mFight  = 0;
  mCommon = 0;

  ui.setupUi(this);

  setWindowTitle(QString(tr("Ожидание, бой %1х%1 ")).arg(COUNT_COMMAND_IN_FIGHT)+tr(STR_VERSION_CLIENT));

  connect(ui.bExitQueue,SIGNAL(clicked()),this,SLOT(sl_ExitQueue()));

  // таймер для отображения времени, проведенного в ожидании, в формат: чч:мм:сс
  mTimer = new QTimer(this);
  connect(mTimer, SIGNAL(timeout()), this, SLOT(sl_Timer()));
}
//---------------------------------------------------------------------------------------------
TWaitForm::~TWaitForm()
{
}
//---------------------------------------------------------------------------------------------
void TWaitForm::sl_ExitQueue()
{
  // отправить запрос на выход из ожидания
  pClient->SendRequestExitFromWait();
}
//---------------------------------------------------------------------------------------------
void TWaitForm::closeEvent(QCloseEvent* )
{
  _exit(0);
}
//---------------------------------------------------------------------------------------------
void TWaitForm::Translate(unsigned short typePacket, char* pData, int size)
{
  switch(typePacket)
  {
    case APPL_TYPE_S_WAIT:
    {
      TS_Wait S_Wait;
      S_Wait.setData(pData,size);
      SetCntClient(S_Wait.getWait(),S_Wait.getFight(),S_Wait.getCommon());
      break;
    }
    //  case QEVENT_A_EXIT_WAIT:
  //    ExitQueueInRoom();
  //    break;
  //  case QEVENT_A_IN_FIGHT:
  //    AnalizCode_A_In_Fight();
  //    break;
    default:;
  }
}
//---------------------------------------------------------------------------------------------
void TWaitForm::sl_Timer()
{
  RefreshInfoStream();

  guint32 delta_ms = ht_GetMSCount()-mStartWait_ms;
  delta_ms /= 1000;
  int hour   = delta_ms/3600;
  int minute = delta_ms/60-hour*60;
  int second = delta_ms%60;
  char str[1000];
  sprintf(str,"%02d:%02d:%02d",hour,minute,second);
  QString str_time = tr("Время ожидания: ")+QString(str);

  ui.lTimeWait->setText(str_time);
}
//---------------------------------------------------------------------------------------------
void TWaitForm::SetCntClient(unsigned int wait,unsigned int fight,unsigned int common)
{
  mWait   = wait;
  mFight  = fight;
  mCommon = common;
}
//---------------------------------------------------------------------------------------------
void TWaitForm::RefreshInfoStream()
{
  QString str;
  str = QString(tr("Кол-во ожидающих: %1")).arg(mWait);
  ui.lCntWait->setText(str);
  str = QString(tr("Общее кол-во: %1")).arg(mCommon);
  ui.lCntCommon->setText(str);
  str = QString(tr("В бою: %1")).arg(mFight);
  ui.lFight->setText(str);
}
//---------------------------------------------------------------------------------------------
void TWaitForm::showGUI()
{
  mTimer->start(eTimeRefresh);
  mStartWait_ms = ht_GetMSCount();
  TBaseGUI::showGUI();
}
//---------------------------------------------------------------------------------------------
void TWaitForm::hideGUI()
{
  mTimer->stop();
  TBaseGUI::hideGUI();
}
//---------------------------------------------------------------------------------------------
