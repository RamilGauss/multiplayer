/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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
#include "..\GBaseLib\HiTimer.h"


TEditorModel::TEditorModel(QWidget *parent)
: TBaseGUI_DX(parent)
{
  setMouseTracking(true);
}
//---------------------------------------------------------------------------------------------
TEditorModel::~TEditorModel()
{

}
//---------------------------------------------------------------------------------------------
void TEditorModel::SetupEvent()
{
  BL_ASSERT(pGame);
  pGame->CreateDevice3DEvent(mDevice);
}
//---------------------------------------------------------------------------------------------
void TEditorModel::Translate(unsigned short type, char*pData, int size)
{
}
//---------------------------------------------------------------------------------------------
void TEditorModel::VisualEvent(QPaintEvent* pEvent)
{
  guint32 iTime = ht_GetMSCount();
  float fElapsedTime = 0;
  pGame->VisualEvent(iTime, fElapsedTime);
}
//---------------------------------------------------------------------------------------------
void TEditorModel::mousePressEvent ( QMouseEvent * event ) 
{
  mBeginPoint = event->pos();
}
//---------------------------------------------------------------------------------------------
void TEditorModel::mouseMoveEvent ( QMouseEvent * event ) 
{
  // ��������� ���������� ������ 
  
  switch(event->button())
  {
    case Qt::LeftButton:
    {
      QPoint pointDelta =  event->pos() - mBeginPoint;
      pGame->SetCameraDelta(pointDelta.x(),pointDelta.y());
      break;
    }
    default:;
  }
}
//--------------------------------------------------------------------------------------------------------
void TEditorModel::showGUI()
{
  TBaseGUI_DX::show();

  // ������ ������
  QBitmap bitmap;
  QBitmap mask;
  QCursor cursor(bitmap,mask);
  cursor.setShape(Qt::BitmapCursor);
  setCursor( cursor );
}
//--------------------------------------------------------------------------------------------------------
void TEditorModel::hideGUI()
{
  TBaseGUI_DX::hideGUI();
  unsetCursor();
  pGame->StopLoadMap();// ���� �����! ���������
  pGame->Done();// �������� ������� ???
}
//--------------------------------------------------------------------------------------------------------
void TEditorModel::keyPressEvent( QKeyEvent * event )
{
  switch(event->key())
  {
    case Qt::Key_O:
    {
      // ������� ������
      OpenModelPath();
      break;
    }
    case Qt::Key_W:
    {
      break;
    }
    case Qt::Key_S:
    {
      break;
    }
    case Qt::Key_A:
    {
      break;
    }
    case Qt::Key_D:
    {
      break;
    }
    default:;
  }
}
//--------------------------------------------------------------------------------------------------------
void TEditorModel::OpenModelPath()
{
  int a = 0;
}
//--------------------------------------------------------------------------------------------------------
