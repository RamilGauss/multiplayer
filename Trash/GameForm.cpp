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


#include "GameForm.h"
#include "GCS.h"
#include "BL_Debug.h"
#include "NET_LevelClientServer.h"
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

//-------------------------------------------------------------------------------------------
TGameForm::TGameForm(QWidget *parent)
: TBaseGUI_DX(parent)
{
  setWindowTitle(tr(STR_VERSION_CLIENT));
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
void TGameForm::Translate(unsigned short type, char*pData, int size)
{
  if(pGame)
    pGame->Translate(type, pData, size);
}
//---------------------------------------------------------------------------------------------
