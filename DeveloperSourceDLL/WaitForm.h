#ifndef TWaitFormH
#define TWaitFormH

#include "BaseGUI.h"
#include <MyGUI.h>

class TWaitForm :
  public TBaseGUI
{

public:
  TWaitForm();
  virtual ~TWaitForm();

protected:
  virtual void Activate();
  virtual void* GetParent();
  virtual const char* GetNameLayout();
  virtual void SetupTabChild();

  virtual void KeyEvent(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char);
protected:

  //void sl_Enter(MyGUI::Widget* _sender);
  //void sl_Exit(MyGUI::Widget* _sender);

  //MyGUI::Button* bEnter;
  //MyGUI::Button* bExit;

  //MyGUI::EditBox* ebIP;
  //MyGUI::EditBox* ebPort; 
  //MyGUI::EditBox* ebLogin;

};

#endif 