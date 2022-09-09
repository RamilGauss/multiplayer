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


#include "EditorModel.h"

#include "GCS.h"
#include "BL_Debug.h"
#include "GlobalParamsTank.h"
#include <QMessageBox>
#include <QTimer>
#include <QBitmap>
#include <QMouseEvent>

#define WIN32_LEAN_AND_MEAN 
#include <d3d9.h> 
#include <d3dx9.h> 
#include "ManagerObjectCommonEditorModel.h"
#include "Logger.h"
#include "HiTimer.h"
#include "MakerObject.h"
#include "namespace_ID_BEHAVIOR.h"
#include "BaseObjectCommon.h"

using namespace nsID_BEHAVIOR;


TEditorModel::TEditorModel(QWidget *parent)
: TBaseGUI_DX(parent)
{
  setWindowTitle(tr(STR_VERSION_EDITOR));
}
//---------------------------------------------------------------------------------------------
TEditorModel::~TEditorModel()
{

}
//---------------------------------------------------------------------------------------------
void TEditorModel::Translate(unsigned short type, char*pData, int size)
{
}
//---------------------------------------------------------------------------------------------
//void TEditorModel::VisualEvent(QPaintEvent* pEvent)
//{
//  guint32 iTime = ht_GetMSCount();
//  float fElapsedTime = 0;
//  pGame->VisualEvent(iTime, fElapsedTime);
//}
//---------------------------------------------------------------------------------------------
void TEditorModel::mousePressEvent ( QMouseEvent * event ) 
{
  //mBeginPoint = event->pos();
}
////---------------------------------------------------------------------------------------------
//void TEditorModel::keyPressEvent( QKeyEvent * event )
//{
//  switch(event->key())
//  {
//    case Qt::Key_O:
//    {
//      // открыть модель
//      OpenModelPath();
//      break;
//    }
//    case Qt::Key_W:
//    {
//      break;
//    }
//    case Qt::Key_S:
//    {
//      break;
//    }
//    case Qt::Key_A:
//    {
//      break;
//    }
//    case Qt::Key_D:
//    {
//      break;
//    }
//    default:;
//  }
//}
//--------------------------------------------------------------------------------------------------------
//void TEditorModel::OpenModelPath()
//{
//  //### пока так
//  TMakerBehavior maker;
//  TBaseObjectCommon* pObject = maker.New(ID_TANK_TOWER);
//  pObject->SetID_Model(0);
//  pGame->AddObject(pObject);
//  //###
//}
//--------------------------------------------------------------------------------------------------------
bool TEditorModel::event ( QEvent * event )  
{
  return false;
}
//--------------------------------------------------------------------------------------------------------