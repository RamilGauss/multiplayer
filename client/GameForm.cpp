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
#include "ApplicationProtocolMainPacket.h"
#include "ApplicationProtocolPacketStream.h"
#include "LayerBetween_QT_DirectX.h"
#include "Bufferizator2Thread.h"

//-------------------------------------------------------------------------------------------
GameForm::GameForm(QWidget *parent)
: TBaseGUI(parent)
{
  pCallBackExitDirectX = NULL;
}
//---------------------------------------------------------------------------------------------
GameForm::~GameForm()
{
}
//---------------------------------------------------------------------------------------------
void GameForm::closeEvent(QCloseEvent* )
{
  sl_Exit();
}
//---------------------------------------------------------------------------------------------
void GameForm::sl_Exit()
{
  _exit(0);
}
//---------------------------------------------------------------------------------------------
void GameForm::Translate(unsigned short type, char*pData, int size)
{
  switch(type)
  {
//    case APPL_TYPE_A_IN_FIGHT:
//    {
//      break;
//    }
//    case APPL_TYPE_A_END_FIGHT:
//    {
      //char* dataPacket = (char*)data + sizeof(TIP_Port);
      //int sizePacket   = size - sizeof(TIP_Port);
      //pGameForm->AddListTank(dataPacket,sizePacket);
      //QEvent * event = new QEvent(QEVENT_A_LIST_TANKS);
      //QCoreApplication::postEvent( pGameForm, event );
//      break;
//    }
    default:;
  }
}
//---------------------------------------------------------------------------------------------
void GameForm::showGUI()
{
  startDX();
}
//---------------------------------------------------------------------------------------------
void GameForm::hideGUI()
{
  stopDX();
}
//---------------------------------------------------------------------------------------------
void GameForm::startDX()
{
  g_LB_QT_DX.StartDirectX(pCallBackExitDirectX);
}
//---------------------------------------------------------------------------------------------
void GameForm::stopDX()
{
  g_LB_QT_DX.StopDirectX();
}
//---------------------------------------------------------------------------------------------

