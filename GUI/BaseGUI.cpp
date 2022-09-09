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


#include "BaseGUI.h"
#include "MyGUI_DeprecatedTypes.h"



void TBaseGUI::Show()
{
  if(mMainWidget==nullptr)
  {
    initialise(GetNameLayout(), (MyGUI::Widget*)GetParent());
    Activate();
    InitTabJump();// научить окно прыгать по табуляции между детьми
    RegisterOnKeyEvent();
  }

  mMainWidget->setVisible(true);
}
//--------------------------------------------------------------
void TBaseGUI::Close()
{
  mMainWidget->setVisible(false);
}
//--------------------------------------------------------------
void TBaseGUI::InitTabJump()
{
  mVectorChild_Tab.clear();
  SetupTabChild();
  int cnt = mVectorChild_Tab.size();
  for(int i = 0 ; i < cnt ; i++ )
  {
    MyGUI::Widget* pChild = mVectorChild_Tab.at(i);
    pChild->eventKeyButtonPressed += MyGUI::newDelegate(this,&TBaseGUI::sl_TabJump);
  }
}
//--------------------------------------------------------------
void TBaseGUI::sl_TabJump(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char)
{
  if(mVectorChild_Tab.size()==0) return;
  if(_key!=MyGUI::KeyCode::Tab) return;

  MyGUI::VectorWidgetPtr::iterator fit = 
    std::find(mVectorChild_Tab.begin(),mVectorChild_Tab.end(),_sender);

  if(mVectorChild_Tab.end()!=fit)
  {
    int curFocus = fit-mVectorChild_Tab.begin();
    curFocus++;
    int nextFocus = curFocus >= (int)mVectorChild_Tab.size() ?  0 : curFocus;
    MyGUI::Widget* pChild = mVectorChild_Tab.at(nextFocus);
    MyGUI::InputManager::getInstance().setKeyFocusWidget(pChild);
  }
}
//--------------------------------------------------------------
void TBaseGUI::RegisterOnKeyEvent()
{
  mMainWidget->eventKeyButtonPressed += MyGUI::newDelegate(this,&TBaseGUI::sl_KeyEvent);
  int cnt = mMainWidget->getChildCount();
  for(int i = 0 ; i < cnt ; i++ )
  {
    MyGUI::Widget* pChild = mMainWidget->getChildAt(i);
    pChild->eventKeyButtonPressed += MyGUI::newDelegate(this,&TBaseGUI::sl_KeyEvent);
  }
}
//--------------------------------------------------------------
void TBaseGUI::sl_KeyEvent(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char)
{
  KeyEvent(_sender, _key, _char);
}
//--------------------------------------------------------------
void TBaseGUI::Shutdown()
{
  shutdown();
}
//--------------------------------------------------------------
