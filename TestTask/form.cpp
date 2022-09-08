#include "Form.h"

#include "Precompiled.h"
#include <atlconv.h>
#include "..\GameLib\IClientDeveloperTool.h"
#include "IGraphicEngine.h"
#include "ControlLight.h"
#include "BL_Debug.h"

using namespace nsStruct3D;

TForm::TForm()
{
	pGE = IClientDeveloperTool::GetSingleton()->GetComponent()->mGraphicEngine;
  pCL = NULL;
}
//------------------------------------------------------
TForm::~TForm()
{

}
//-------------------------------------------------------------------------------------
void TForm::Activate()
{
  ASSIGN_WIDGET(cbViewLight);
  ASSIGN_WIDGET(cbCountLight);
  ASSIGN_WIDGET(cbControlLight);
	ASSIGN_WIDGET(cbPostEffectMirror);

  cbViewLight->eventMouseButtonClick += MyGUI::newDelegate(this, &TForm::sl_View);
  cbCountLight->eventComboChangePosition += MyGUI::newDelegate(this, &TForm::sl_Count);
  cbControlLight->eventComboChangePosition += MyGUI::newDelegate(this, &TForm::sl_Control);
  cbPostEffectMirror->eventMouseButtonClick += MyGUI::newDelegate(this, &TForm::sl_PostEffectMirror);
}
//-------------------------------------------------------------------------------------
const char* TForm::GetNameLayout()
{
  return "FormTestTask.layout";
}
//-------------------------------------------------------------------------------------
void* TForm::GetParent()
{
  return nullptr;
}
//-------------------------------------------------------------------------------------
void TForm::SetupTabChild()
{
  mVectorChild_Tab.push_back(cbViewLight);
  mVectorChild_Tab.push_back(cbCountLight);
  mVectorChild_Tab.push_back(cbControlLight);
}
//-------------------------------------------------------------------------------------
void TForm::KeyEvent(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char)
{

}
//-------------------------------------------------------------------------------------
void TForm::SetCountLight(int v)
{
  cbCountLight->setIndexSelected(v-1);
}
//-------------------------------------------------------------------------------------
void TForm::SetControlLight(int v)
{
  cbControlLight->setIndexSelected(v);
}
//-------------------------------------------------------------------------------------
void TForm::SetViewLight(bool v)
{
	cbViewLight->setStateSelected(v);
}
//-------------------------------------------------------------------------------------
void TForm::SetPostEffectMirror(bool v)
{
	cbPostEffectMirror->setStateSelected(v);
}
//-------------------------------------------------------------------------------------
void TForm::sl_View(MyGUI::Widget* _sender)
{
  cbViewLight->setStateSelected(!cbViewLight->getStateSelected());
  pCL->SetViewObject(cbViewLight->getStateSelected());
}
//-------------------------------------------------------------------------------------
void TForm::sl_Count(MyGUI::ComboBox* _sender, size_t _index)
{
  BL_ASSERT(pCL);
  SetAllLightDisable();
  for(size_t i = 0 ; i < _index+1 ; i++ )
    pCL->SetLightEnable(i,true);
}
//-------------------------------------------------------------------------------------
void TForm::sl_Control(MyGUI::ComboBox* _sender, size_t _index)
{
  BL_ASSERT(pCL);
}
//-------------------------------------------------------------------------------------
void TForm::SetControlLight(TControlLight* pcl)
{
  pCL = pcl;
  //---------------------------------------------
  cbCountLight->removeAllItems();
  cbControlLight->removeAllItems();
  int cnt = pCL->GetCountAllLight();
  for(int i = 0 ; i < cnt ; i++)
  {
    char s[10];
    sprintf(s,"%d",i+1);
    cbCountLight->addItem(s);
    cbControlLight->addItem(s);
  }
  //---------------------------------------------
  SetCountLight(1);
  SetControlLight(0);
  SetViewLight(true);
	SetPostEffectMirror(pGE->GetPostEffectMirror());
}
//-------------------------------------------------------------------------------------
int TForm::GetCurrentLight()
{
  return cbControlLight->getIndexSelected();
}
//-------------------------------------------------------------------------------------
void TForm::SetAllLightDisable()
{
  int cnt = pCL->GetCountAllLight();
  for(int i = 0 ; i < cnt ; i++)
    pCL->SetLightEnable(i,false);
}
//-------------------------------------------------------------------------------------
void TForm::sl_PostEffectMirror(MyGUI::Widget* _sender)
{
  cbPostEffectMirror->setStateSelected(!cbPostEffectMirror->getStateSelected());
	pGE->SetPostEffectMirror(cbPostEffectMirror->getStateSelected());
}
//-------------------------------------------------------------------------------------