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


#include "GameForm.h"
#include "GCS.h"
#include "BL_Debug.h"
#include "ClientTank.h"
#include "GlobalParamsTank.h"
#include <QMessageBox>
#include <QTimer>
#include <QBitmap>
#include <QMouseEvent>
#include "ApplicationProtocolMainPacket.h"
#include "ApplicationProtocolPacketStream.h"

#define WIN32_LEAN_AND_MEAN 
#include <d3d9.h> 
#include <d3dx9.h> 
#include "ApplicationProtocolPacketAnswer.h"
#include "ManagerObjectCommonClient.h"
#include "Logger.h"



//-------------------------------------------------------------------------------------------
TGameForm::TGameForm(QWidget *parent)
: TBaseGUI_DX(parent)
{
  setMouseTracking(true);
}
//---------------------------------------------------------------------------------------------
TGameForm::~TGameForm()
{
}
//---------------------------------------------------------------------------------------------
void TGameForm::closeEvent(QCloseEvent* )
{
  sl_Exit();
}
//---------------------------------------------------------------------------------------------
void TGameForm::sl_Exit()
{
  _exit(0);
}
//---------------------------------------------------------------------------------------------
void TGameForm::SetupEvent()
{
  BL_ASSERT(pGame);
  pGame->Setup(mDevice);
  ((TManagerObjectCommonClient*)pGame)->SetTransport(pClient);
}
//---------------------------------------------------------------------------------------------
void TGameForm::Translate(unsigned short type, char*pData, int size)
{
  switch(type)
  {
    case APPL_TYPE_A_IN_FIGHT:
    {
      GlobalLoggerForm.WriteF_time("Получен пакет вход в бой.\n");
      // создать поток загрузки карты
      // когда загрузка закончится отослать запрос на корректирующий пакет
      pGame->SetPacketA_In_Fight(pData,size);
      break;
    }
    case APPL_TYPE_G_S_LOAD_MAP:
    {
      break;
    }
    case APPL_TYPE_G_S_COUNT_DOWN:
    {
      // отображать отсчет на 
      break;
    }
    case APPL_TYPE_G_S_FIGHT_COORD_BULLET:
    {
      pGame->RefreshFromServer();
      break;
    }
    default:;
  }
}
//---------------------------------------------------------------------------------------------
void TGameForm::VisualEvent(QPaintEvent* pEvent)
{
  guint32 iTime = ht_GetMSCount();
  float fElapsedTime = 0;
  pGame->VisualEvent(iTime, fElapsedTime);
}
//---------------------------------------------------------------------------------------------
void TGameForm::mouseMoveEvent ( QMouseEvent * event ) 
{
  // настроить ориентацию камеры (или может прицела)
  // отправить на сервер эту информацию

//  if(mState!=eFight) return;
//  guint32 now_ms = ht_GetMSCount();
//  if(now_ms>mLastTimeSendMouseStream+eIntervalMouseStream)
//  {
//
//    D3DXVECTOR3 view = *mCamera.GetLookAtPt();
//    //view.y
//    if(mMouseHandler)
//      mMouseHandler->Work(fTime, fElapsedTime, pUserContext);
//
//    mLastTimeSendMouseStream = now_ms;
//  }

  QPoint point(width()/2,height()/2);
  QPoint pointDelta =  event->pos() - point;
  if(event->modifiers()!=Qt::ControlModifier)
  {
    point = mapToGlobal(point);
    QCursor::setPos(point);
  }
}
//--------------------------------------------------------------------------------------------------------
void TGameForm::showGUI()
{
  TBaseGUI_DX::show();

  // скрыть курсор
  QBitmap bitmap;
  QBitmap mask;
  QCursor cursor(bitmap,mask);
  cursor.setShape(Qt::BitmapCursor);
  setCursor( cursor );
}
//--------------------------------------------------------------------------------------------------------
void TGameForm::hideGUI()
{
  TBaseGUI_DX::hide();
  unsetCursor();
}
//--------------------------------------------------------------------------------------------------------
void TGameForm::keyPressEvent( QKeyEvent * event )
{
  switch(event->key())
  {
    case Qt::Key_Escape:
    {
      pClient->SendRequestExitFromFight();
      break;
    }
    default:;
  }
}
//--------------------------------------------------------------------------------------------------------
