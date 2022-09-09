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

#include <QPaintEvent>
#include <QPainter>
#include <QFile>

#include "BaseGUI_DX.h"
#include "ManagerObjectCommon.h"
#include "Logger.h"

static TBaseGUI_DX* PtrBaseGUI_DX = NULL;
typedef LRESULT (CALLBACK *DeclWndProc)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

static DeclWndProc pWndProcGUI           = NULL;
static DeclWndProc pWndProcGraphicEngine = NULL;

// т.к. все происходит в одном потоке - мьютексы не нужны
LRESULT CALLBACK BaseGUI_DX_WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
  if(PtrBaseGUI_DX) 
    return (LRESULT)PtrBaseGUI_DX->WndProc(&hWnd, &uMsg, &wParam, &lParam);
  
  return DefWindowProc( hWnd, uMsg, wParam, lParam );
}
//---------------------------------------------------------------------------------------

TBaseGUI_DX::TBaseGUI_DX( QWidget* pParent) 
: TBaseGUI( pParent ) 
{ 
  PtrBaseGUI_DX = this;
  //###
  // эксперимент
  //HWND hwnd = winId();
  //TFilterWinApi::SetWndProc(&hwnd);
  //###
  pWndProcGUI = NULL;
  //setMinimumSize( 400, 400); 
  setAttribute( Qt::WA_OpaquePaintEvent, true);  
  setAttribute( Qt::WA_PaintOnScreen, true); 

  setMouseTracking(true);

  connect(&mTimer, SIGNAL(timeout()), this, SLOT(sl_Rendering()));
} 
//-------------------------------------------------------------------------
TBaseGUI_DX::~TBaseGUI_DX()
{
  if(pGame)
    pGame->Done();

  PtrBaseGUI_DX = NULL;
}
//-------------------------------------------------------------------------
void TBaseGUI_DX::SetupEvent()
{
  HWND hwnd = winId();
  // определить WndProc для окна
  pWndProcGUI = (DeclWndProc)GetWindowLongPtr(hwnd, GWLP_WNDPROC);   // обработчик Qt (был по крайней мере)
  pWndProcGraphicEngine = (DeclWndProc)pGame->GetWndProc_GraphicEngine();// обработчик графического ядра
  // инициализация движка игры
  pGame->Init(hwnd);
  // и вот только сейчас можно транслировать поток событий
  LONG_PTR res = SetWindowLongPtr(hwnd,GWLP_WNDPROC,(LONG_PTR)BaseGUI_DX_WndProc);// новый


  //setWindowState(windowState()^Qt::WindowFullScreen);//###
}
//---------------------------------------------------------
void TBaseGUI_DX::sl_Rendering()
{
  /*
  bool QtIsFullScreen = isFullScreen();
  bool DXIsFullScreen = pGame->IsFullScreen();

  if(QtIsFullScreen!=DXIsFullScreen)
  {
    GlobalLoggerForm.WriteF_time("toggle in full screen 0, state=0x%X\n",windowState());
    if(DXIsFullScreen)
      setWindowState(Qt::WindowFullScreen);
    else
      setWindowState(0);
    GlobalLoggerForm.WriteF_time("toggle in full screen 1, state=0x%X\n",windowState());
    GlobalLoggerForm.WriteF("--------------------------------------------------\n");
  }*/

  pGame->Work();
  
  //SetBITForQt();
  SaveRender();
  // перерисовать всех детей
  RenderChild();
  //repaint();

  //SetBITForQt();
}
//---------------------------------------------------------
void TBaseGUI_DX::showGUI()
{
  TBaseGUI::show();
  mTimer.start();
}
//---------------------------------------------------------------------------------------------
void TBaseGUI_DX::hideGUI()
{
  TBaseGUI::hide();
  mTimer.stop();
}
//---------------------------------------------------------------------------------------------
void* TBaseGUI_DX::WndProc( void* phWnd, void* puMsg, void* pwParam, void* plParam )
{
  HWND hWnd = *((HWND*)phWnd);
  UINT uMsg = *((UINT*) puMsg);
  WPARAM wParam = *((WPARAM*) pwParam);  
  LPARAM lParam = *((LPARAM*) plParam);

  if(hWnd!=winId())
  {
    BL_FIX_BUG();
    return 0;
  }

  LRESULT resultGE = 0;
  LRESULT resultGUI = 0;

  //###
  if(uMsg==WM_ENTERSIZEMOVE)
    GlobalLoggerForm.WriteF_time("uMsg==WM_ENTERSIZEMOVE\n");

  if(uMsg==WM_EXITSIZEMOVE)
    GlobalLoggerForm.WriteF_time("uMsg==WM_EXITSIZEMOVE\n");
  //###

  resultGE = pWndProcGraphicEngine(hWnd, uMsg, wParam, lParam);
  resultGUI = pWndProcGUI(hWnd, uMsg, wParam, lParam);

  return (void*)resultGE;
}
//---------------------------------------------------------------------------------------------
//void TBaseGUI_DX::resizeEvent ( QResizeEvent * event )
//{
//  //QRect r1 = event->rect();
//  QRect r2 = this->rect();
//  QPoint p = pos();
//  int a = 0;
//}
////---------------------------------------------------------------------------------------------
//void TBaseGUI_DX::paintEvent( QPaintEvent * event ) 
//{
  //pGame->Work();
  //QRect r1 = event->rect();
  //QRect r2 = this->rect();
  //QPoint p = pos();
  //int a = 0;
//}
//---------------------------------------------------------------------------------------------
void TBaseGUI_DX::RenderChild()
{

}
//---------------------------------------------------------------------------------------------
void TBaseGUI_DX::SaveRender()
{
  QPixmap pixmap(size());
  QPainter painter(&pixmap);
  render(&painter);
  //bool resSave = pixmap.save(tr("D:\\Hahaha.jpeg"));
}
//--------------------------------------------------------------------------------------
