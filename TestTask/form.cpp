#include "Form.h"

#include "Precompiled.h"
#include <atlconv.h>
#include "..\GameLib\IClientDeveloperTool.h"
#include "IGraphicEngine.h"
#include "SphericalDecart.h"

using namespace nsStruct3D;

TForm::TForm()
{
	pGE = IClientDeveloperTool::GetSingleton()->GetComponent()->mGraphicEngine;
}
//------------------------------------------------------
TForm::~TForm()
{

}
//-------------------------------------------------------------------------------------
void TForm::Activate()
{
	ASSIGN_WIDGET(ebPitch)
	ASSIGN_WIDGET(ebYaw)
	ASSIGN_WIDGET(ebDist)
}
//-------------------------------------------------------------------------------------
void TForm::sl_Enter(MyGUI::Widget* _sender)
{
  if(_sender==ebPitch)
	{
		TVector3 posLight = *(pGE->GetLightPosition(0));

		TSphericalDecart sd;
		TSphericalDecart::TSpheric s;
		sd.Decart2Shperic(posLight,s);

	  USES_CONVERSION;
	  std::string sA = W2A((LPCWSTR)ebPitch->getOnlyText().data());
		float v = atof(sA.data());
		s.theta = v;
		sd.Shperic2Decart(s,posLight);

		pGE->SetLightPosition(0,&posLight);
	}
	else if(_sender==ebYaw)
	{
		TVector3 posLight = *(pGE->GetLightPosition(0));

		TSphericalDecart sd;
		TSphericalDecart::TSpheric s;
		sd.Decart2Shperic(posLight,s);

		USES_CONVERSION;
		std::string sA = W2A((LPCWSTR)ebPitch->getOnlyText().data());
		float v = atof(sA.data());
		s.fi = v;
		sd.Shperic2Decart(s,posLight);

		pGE->SetLightPosition(0,&posLight);
	}
	else if(_sender==ebDist)
	{
		TVector3 posLight = *(pGE->GetLightPosition(0));

		TSphericalDecart sd;
		TSphericalDecart::TSpheric s;
		sd.Decart2Shperic(posLight,s);

		USES_CONVERSION;
		std::string sA = W2A((LPCWSTR)ebPitch->getOnlyText().data());
		float v = atof(sA.data());
		s.r = v;
		sd.Shperic2Decart(s,posLight);

		pGE->SetLightPosition(0,&posLight);
	}
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
  mVectorChild_Tab.push_back(ebPitch);
  mVectorChild_Tab.push_back(ebYaw);
  mVectorChild_Tab.push_back(ebDist);
}
//-------------------------------------------------------------------------------------
void TForm::KeyEvent(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char)
{
  switch(_key.getValue())
  {
    case MyGUI::KeyCode::Return:
      sl_Enter(_sender);
      break;
    default:;
  }
}
//-------------------------------------------------------------------------------------
void TForm::SetPitch(float v)
{
	char s[100];
	sprintf(s,"%2.2f",v);
	ebPitch->setCaption(s);
}
//-------------------------------------------------------------------------------------
void TForm::SetYaw(float v)
{
	char s[100];
	sprintf(s,"%2.2f",v);
	ebYaw->setCaption(s);
}
//-------------------------------------------------------------------------------------
void TForm::SetDist(float v)
{
	char s[100];
	sprintf(s,"%3.2f",v);
	ebDist->setCaption(s);
}
//-------------------------------------------------------------------------------------

