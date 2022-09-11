
#ifndef FormH
#define FormH

#include "BaseGUI.h"
#include <MyGUI.h>

class IGraphicEngine;
class TControlLight;

class TForm :
  public TBaseGUI
{
	IGraphicEngine* pGE;

  TControlLight* pCL;

  MyGUI::Button*   cbViewLight;
  MyGUI::ComboBox* cbCountLight;
  MyGUI::ComboBox* cbControlLight;
	MyGUI::Button* cbPostEffectMirror;

public:
  TForm();
  virtual ~TForm();

  void SetControlLight(TControlLight* pcl);

  int GetCurrentLight();

protected:
  virtual void Activate();
  virtual void* GetParent();
  virtual const char* GetNameLayout();
  virtual void SetupTabChild();

  virtual void KeyEvent(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char);

protected:// slots

  void sl_View(MyGUI::Widget* _sender);
  void sl_Count(MyGUI::ComboBox* _sender, size_t _index);
  void sl_Control(MyGUI::ComboBox* _sender, size_t _index);
	void sl_PostEffectMirror(MyGUI::Widget* _sender);

protected:

  void SetCountLight(int v);
  void SetControlLight(int v);// индекс
  void SetViewLight(bool v);
	void SetPostEffectMirror(bool v);

  void SetAllLightDisable();
};

#endif 
