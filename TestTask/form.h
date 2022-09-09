
#ifndef FormH
#define FormH

#include "BaseGUI.h"
#include <MyGUI.h>

class IGraphicEngine;

class TForm :
  public TBaseGUI
{
	IGraphicEngine* pGE;
public:
  TForm();
  virtual ~TForm();

	void SetPitch(float v);
	void SetYaw(float v);
	void SetDist(float v);

protected:
  virtual void Activate();
  virtual void* GetParent();
  virtual const char* GetNameLayout();
  virtual void SetupTabChild();

  virtual void KeyEvent(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char);

protected:

  void sl_Enter(MyGUI::Widget* _sender);

  MyGUI::EditBox* ebPitch;
  MyGUI::EditBox* ebYaw; 
  MyGUI::EditBox* ebDist;

};

#endif 
