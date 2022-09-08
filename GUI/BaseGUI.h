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

#ifndef BaseGUIH
#define BaseGUIH

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"


#define ASSIGN_WIDGET(name) \
assignWidget(name,#name);

class TBaseGUI :
	public wraps::BaseLayout
{
protected:
  MyGUI::VectorWidgetPtr mVectorChild_Tab;

public:
  TBaseGUI(){};
  virtual ~TBaseGUI(){};

  virtual void Show();
  virtual void Hide();

  void Shutdown();

protected:
  virtual void Activate() = 0;
  virtual const char* GetNameLayout() = 0;
  virtual void* GetParent() = 0;

  virtual void SetupTabChild(){};

  virtual void KeyEvent(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char){}

private:
  void InitTabJump();
  void RegisterOnKeyEvent();

  void sl_TabJump(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char);
  void sl_KeyEvent(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char);
  
};

#endif // BaseGUI
