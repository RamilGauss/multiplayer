/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
√удаков –амиль —ергеевич 
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
#include "TankTower.h"
#include "namespace_ID_BEHAVIOR.h"

using namespace nsID_BEHAVIOR;

TGameForm* pGameForm = NULL;

void CallbackLoadMapEndEvent(void* /*pData*/, int /*size*/)
{
  BL_ASSERT(pGameForm);
  pGameForm->LoadMapEndEvent();
}
//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
TGameForm::TGameForm(QWidget *parent)
: TBaseGUI_DX(parent)
{
  pGameForm = this;

  setMouseTracking(true);
}
//---------------------------------------------------------------------------------------------
TGameForm::~TGameForm()
{
  pGame->Unregister(CallbackLoadMapEndEvent);
  pGameForm = NULL;
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
  pGame->CreateDevice3DEvent(mDevice);
  
  pGame->Register(CallbackLoadMapEndEvent);
}
//---------------------------------------------------------------------------------------------
void TGameForm::Translate(unsigned short type, char*pData, int size)
{
  switch(type)
  {
    case APPL_TYPE_A_IN_FIGHT:
    {
      ApplPacketA_In_Fight(pData,size);
      break;
    }
    case APPL_TYPE_G_A_CORRECT_PACKET_STATE_TANK:
    {
      ApplPacketA_Correct_Packet_State_Tank();
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
      //pGame->RefreshFromServer();
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

    pGame->SetCameraDelta(pointDelta.x(),pointDelta.y());
  }
}
//--------------------------------------------------------------------------------------------------------
void TGameForm::showGUI()
{
  TBaseGUI_DX::showGUI();

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
  TBaseGUI_DX::hideGUI();
  unsetCursor();
  pGame->StopLoadMap();// стоп поток! синхронно
  pGame->Done();// очистить объекты ???
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
void TGameForm::PrepareTank(TTankTower* pTank, int i)
{
  char* pData = (char*)mPacketInFight.getPointerTankProperty(i);
  int size = mPacketInFight.getSizeTankProperty(i);
  pTank->SetProperty(pData,size);
}
//--------------------------------------------------------------------
void TGameForm::AddTankInCommonList()
{
  TMakerBehavior mMaker;
  int cntTank = mPacketInFight.getCountTank();
  for(int i = 0 ; i < cntTank ; i++ )
  {
    // 
    TBaseObjectCommon* pObject = mMaker.New(ID_TANK_TOWER);
    PrepareTank((TTankTower*)pObject,i);

    pGame->AddObject(pObject);
  }
}
//--------------------------------------------------------------------
void TGameForm::LoadMapEndEvent()
{
  AddTankInCommonList();
  SetWaitTrue();
  pClient->SendRequestCorrectPacket();// отсылка запроса
}
//--------------------------------------------------------------------
void TGameForm::ApplPacketA_In_Fight(char* pData, int size)
{
  GlobalLoggerForm.WriteF_time("ѕолучен пакет вход в бой.\n");
  // создать поток загрузки карты
  // когда загрузка закончитс€ отослать запрос на корректирующий пакет

  // к сожалению поток загрузки карты прервать нельз€,
  // ждите
  mPacketInFight.setData(pData,size);
  pGame->LoadMap(mPacketInFight.getCodeMap());
}
//--------------------------------------------------------------------
void TGameForm::ApplPacketA_Correct_Packet_State_Tank()
{
  GlobalLoggerForm.WriteF_time("ѕолучен корректирующий пакет, состо€ние танков.\n");
  if(IsWaitCorrectPacketTank()==false) return;
  // применить содержимое пакета

  pGame->EndLoadMap();// окончание загрузки

  SetWaitFalse();
}
//--------------------------------------------------------------------
