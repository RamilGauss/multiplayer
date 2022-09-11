/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
2011, 2012, 2013
===========================================================================
                        Common Information
"TornadoEngine" GPL Source Code

This file is part of the "TornadoEngine" GPL Source Code.

"TornadoEngine" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"TornadoEngine" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "TornadoEngine" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "TornadoEngine" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "TornadoEngine" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 

#include "GameRoomPrepare.h"

#include "Precompiled.h"
#include <atlconv.h>


TGameRoomPrepare::TGameRoomPrepare()
{

}
//------------------------------------------------------
TGameRoomPrepare::~TGameRoomPrepare()
{

}
//-------------------------------------------------------------------------------------
void TGameRoomPrepare::Activate()
{
  //assignWidget(bEnter,"bEnter");
  //assignWidget(bExit, "bExit");

  //assignWidget(ebIP,    "ebIP");
  //assignWidget(ebPort,  "ebPort");
  //assignWidget(ebLogin, "ebLogin");

  //bEnter->eventMouseButtonClick += MyGUI::newDelegate(this, &TClientMain::sl_Enter);
  //bExit ->eventMouseButtonClick += MyGUI::newDelegate(this, &TClientMain::sl_Exit);
}
//-------------------------------------------------------------------------------------
//void TClientMain::sl_Enter(MyGUI::Widget* _sender)
//{
//
//}
////-------------------------------------------------------------------------------------
//void TClientMain::sl_Exit(MyGUI::Widget* _sender)
//{
//  Close();
//}
//-------------------------------------------------------------------------------------
//void TClientMain::sl_IP(MyGUI::EditBox* _sender)
//{
//  USES_CONVERSION;
//  std::string sA = W2A((LPCWSTR)_sender->getOnlyText().data());
//  int a = 0;
//}
//-------------------------------------------------------------------------------------
const char* TGameRoomPrepare::GetNameLayout()
{
  return "GameRoomPrepare.layout";
}
//-------------------------------------------------------------------------------------
void* TGameRoomPrepare::GetParent()
{
  return nullptr;
}
//-------------------------------------------------------------------------------------
void TGameRoomPrepare::SetupTabChild()
{
  //mVectorChild_Tab.push_back(ebIP);
  //mVectorChild_Tab.push_back(ebLogin);
  //mVectorChild_Tab.push_back(ebPort);
}
//-------------------------------------------------------------------------------------
void TGameRoomPrepare::KeyEvent(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char)
{
  switch(_key.getValue())
  {
    case MyGUI::KeyCode::Return:
      //sl_Enter(_sender);
      break;
    default:;
  }
}
//-------------------------------------------------------------------------------------
