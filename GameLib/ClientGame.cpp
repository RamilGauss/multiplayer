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
the "Tanks" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 

#include <stddef.h>
#include <string>

#include "BL_Debug.h"
#include "ClientGame.h"
#include "MakerPhysicEngine.h"
#include "MakerGraphicEngine.h"


using namespace std;

TClientGame::TClientGame()
{

}
//------------------------------------------------------------------------
TClientGame::~TClientGame()
{

}
//------------------------------------------------------------------------
void TClientGame::Work(const char* sNameDLL)// начало работы
{
  Init(sNameDLL);

  flgNeedStop = false;
  flgActive   = true;
  //------------------------------------------------------
  while(flgNeedStop==false)
  {
    // обработать событи€ окна
    HandleWindowEvent(GetHandleWindow());

    // обработать событи€
    HandleExternalEvent();
    // расчеты, необходимые дл€ рендера, в зависимости от времени предыдущего расчета
    Calc();
    Render();
  }
  //------------------------------------------------------
  flgActive = false;

  Done();
}
//------------------------------------------------------------------------
void TClientGame::Init(const char* sNameDLL)
{
  // создать двигатели и проинициализировать менеджеры
  TMakerGraphicEngine makerGraphicEngine;
  mGraphicEngine = makerGraphicEngine.New();
  mGraphicEngine->SetKeyMouseHandler(&mMKM);
  mGraphicEngine->Init();// создали окно
  //------------------------------------------
  TMakerPhysicEngine makerPhysicEngine;
  mPhysicEngine = makerPhysicEngine.New();
  //------------------------------------------
  TMakerNET_Engine makerNET;
  mNET = makerNET.New();
}
//------------------------------------------------------------------------
void TClientGame::Done()
{
  delete mGraphicEngine;
  mGraphicEngine = NULL;
}
//------------------------------------------------------------------------
TClientGame::tResHandleWindowEvent TClientGame::HandleWindowEvent()
{
  HWND hWnd = mGraphicEngine->GetHWND();

  // Now we're ready to receive and process Windows messages.
  bool bGotMsg;
  MSG msg;
  msg.message = WM_NULL;
  PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );

  while( WM_QUIT != msg.message )
  {
    // Use PeekMessage() so we can use idle time to render the scene. 
    bGotMsg = ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) != 0 );

    if( bGotMsg )
    {
      // Translate and dispatch the message
      if(0 == TranslateAccelerator( hWnd, hAccel, &msg ) )
      {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
      }
    }
    else
      return eNoWindowEvent;
  }
  return eQuit;
}
//------------------------------------------------------------------------
void TClientGame::HandleExternalEvent()
{

}
//------------------------------------------------------------------------
void TClientGame::Calc()
{

}
//------------------------------------------------------------------------
void TClientGame::Render()
{
  mGraphicEngine->Work(mMTime.GetTime());
}
//------------------------------------------------------------------------
void TClientGame::SetEvent(const char* sFrom, unsigned int key, void* pData, int sizeData)
{

}
//------------------------------------------------------------------------
HWND TClientGame::GetHandleWindow()
{
  return mGraphicEngine->GetHWND();
}
//------------------------------------------------------------------------



