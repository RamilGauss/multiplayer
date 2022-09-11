/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
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
