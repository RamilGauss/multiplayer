/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
√удаков –амиль —ергеевич 
2011, 2012
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
#define _USE_MATH_DEFINES

#include <cmath>



#include "ClientDeveloperTool_TestTask.h"
#include "MakerObjectCommon.h"
#include "../GameLib/IClientDeveloperTool.h"
#include "../GameLib/IManagerObjectCommon.h"
#include "../BigJack/IGraphicEngine.h"
#include "../GameLib/IBaseObjectCommon.h"
#include "../GUI/IGUI.h"
#include "Logger.h"
#include "HiTimer.h"
#include "../GameLib/NameSrcEventID.h"
#include "../GameLib/IManagerStateMachine.h"
#include "IControlCamera.h"
#include <string.h>
#include "IXML.h"
#include "SphericalDecart.h"

using namespace std;
using namespace nsStruct3D;
using namespace nsEvent;

using namespace nsKey;

#define LOG_TIME_LOAD_EDITOR_MODEL

#define DELTA_MOVE 0.2f //0.4f
#define DELTA_ROTATE 0.01f
#define ASPECT_MOUSE_X 0.002f
#define ASPECT_MOUSE_Y 0.002f

#define ASPECT_LIGHT_X 0.01f
#define ASPECT_LIGHT_Y 0.01f


#define UP 1000000

TClientDeveloperTool_TestTask::TClientDeveloperTool_TestTask()
{
  flgDragCamera = false;
	flgDragLight = false;

  mForm = NULL;

  mMakerObjectCommon = new TMakerObjectCommon;
  
	pSphere = NULL;
	mUseBind = false;
}
//------------------------------------------------------------------------------------
TClientDeveloperTool_TestTask::~TClientDeveloperTool_TestTask()
{
  delete mMakerObjectCommon;
}
//------------------------------------------------------------------------------------
bool TClientDeveloperTool_TestTask::MouseEvent(TMouseEvent* pEvent)
{
  switch(pEvent->state)
  {
    case nsEvent::eButtonUp:
			if(pEvent->button==eLClick)// камера
			{
				flgDragCamera = false;
			}
			else if(pEvent->button==eRClick)// освещение
			{
				flgDragLight = false;
			}
      break;
    case nsEvent::eButtonDown:
			if(pEvent->button==eLClick)// камера
			{
				flgDragCamera = true;
				mOldX_Camera = pEvent->x;
				mOldY_Camera = pEvent->y;
			}
			else if(pEvent->button==eRClick)// освещение
			{
				flgDragLight = true;
				mOldX_Light = pEvent->x;
				mOldY_Light = pEvent->y;		
				vLight = *(mComponent.mGraphicEngine->GetLightPosition(0));
			}
      break;
    case nsEvent::eButtonDblClick:
      break;
    case nsEvent::eWheel:
      mComponent.mControlCamera->AddDistObj(float(pEvent->delta_wheel)/100.0f);
      break;
    case nsEvent::eMove:
      if(flgDragCamera)
      {
        float dRight = float(pEvent->x - mOldX_Camera)*ASPECT_MOUSE_X;
        float dDown  = float(pEvent->y - mOldY_Camera)*ASPECT_MOUSE_Y;
        mComponent.mControlCamera->RotateRight(dRight);
        mComponent.mControlCamera->RotateDown(dDown);
        
        mOldX_Camera = pEvent->x;
        mOldY_Camera = pEvent->y;
      }
			if(flgDragLight)
			{
				float dPitch = float(pEvent->y - mOldY_Light)*ASPECT_LIGHT_X;// theta
				float dYaw   = float(pEvent->x - mOldX_Light)*ASPECT_LIGHT_Y;// он же fi
				TSphericalDecart::TSpheric s;
				TSphericalDecart sd;
				sd.Decart2Shperic(vLight,s);// из декартовых в сферические
				s.fi += dYaw;
				s.theta += dPitch;
				TVector3 vNew;
				sd.Shperic2Decart(s,vNew);// обратно
				mComponent.mGraphicEngine->SetLightPosition(0,&vNew);
				mForm->SetPitch(s.theta);
				mForm->SetYaw(s.fi);
			}
  }
  return true;
}
//------------------------------------------------------------------------------------
bool TClientDeveloperTool_TestTask::KeyEvent(TKeyEvent* pEvent)
{
  unsigned int v;
  bool res = mComponent.mMStateMachine->GetValue(mIDkey,pEvent->key,v);
  v |= (pEvent->pressed?0:UP);
  if(res)
  switch(v)
  {
    // клавиатура
    case e_W://W
      mComponent.mControlCamera->SetSpeedForward(DELTA_MOVE);
      break;
    case e_W|UP://W
      mComponent.mControlCamera->SetSpeedForward(0);
      break;
    case e_S://S
      mComponent.mControlCamera->SetSpeedForward(-DELTA_MOVE);
      break;
    case e_S|UP://S
      mComponent.mControlCamera->SetSpeedForward(0);
      break;
    case e_A://A
      mComponent.mControlCamera->SetSpeedRight(-DELTA_MOVE);
      break;
    case e_A|UP://A
      mComponent.mControlCamera->SetSpeedRight(0);
      break;
    case e_D://D
      mComponent.mControlCamera->SetSpeedRight(DELTA_MOVE);
      break;
    case e_D|UP://D
      mComponent.mControlCamera->SetSpeedRight(0);
      break;
    case e_Q://Q
      mComponent.mControlCamera->SetSpeedUp(DELTA_MOVE);
      break;
    case e_Q|UP://Q
      mComponent.mControlCamera->SetSpeedUp(0);
      break;
    case e_E://E
      mComponent.mControlCamera->SetSpeedUp(-DELTA_MOVE);
      break;
    case e_E|UP://E
      mComponent.mControlCamera->SetSpeedUp(0);
      break;
    // мышь
    case e_Numpad4:// влево 4
      mComponent.mControlCamera->RotateRight(-DELTA_ROTATE);
      break;
    case e_Numpad6:// вправо 6
      mComponent.mControlCamera->RotateRight(DELTA_ROTATE);
      break;
    case e_Numpad8:// вверх 8
      mComponent.mControlCamera->RotateDown(-DELTA_ROTATE);
      break;
    case e_Numpad2:// вниз 2
      mComponent.mControlCamera->RotateDown(DELTA_ROTATE);
      break;
    case e_Numpad7:// вращатьс€ влево 7
      mComponent.mControlCamera->Roll(DELTA_ROTATE);
      break;
    case e_Numpad9:// вращатьс€ вправо 9
      mComponent.mControlCamera->Roll(-DELTA_ROTATE);
      break;
    case e_C:// сбросить ориентацию камеры C
      mComponent.mControlCamera->ClearRotate();
      break;
    case e_V:// сбросить ориентацию камеры V
    {
      TVector3 v;
      v.x = 0.0f;
      v.y = 0.0f;
      v.z = 0.0f;
      mComponent.mControlCamera->SetPositionLookAt(&v);
      break;
    }
    case e_B: // B 
    {
			BindObj();
      break;
    }
    case e_N: 
    {
      break;
    }
    case e_O: 
    {
      break;
    }
  }
  return true;
}
//--------------------------------------------------------------------
void TClientDeveloperTool_TestTask::PrepareForRender()
{

}
//------------------------------------------------------------------------------------
IMakerObjectCommon* TClientDeveloperTool_TestTask::GetMakerObjectCommon()
{
  return mMakerObjectCommon;
}
//------------------------------------------------------------------------------------
string TClientDeveloperTool_TestTask::GetTitleWindow()
{
  return "“естовое задание";
}
//------------------------------------------------------------------------------------
void TClientDeveloperTool_TestTask::Init(TComponentClient* pComponent, const char* arg )
{
  InitLog();
  mComponent = *pComponent; 

  CreateObjects();

  mForm = new TForm;
 
  mComponent.mGUI->Add(string("mForm"),mForm);
  // показать форму
  mForm->Show();
  // подстроитьс€
  mComponent.mGUI->Resize();

  bool resLoadMSM = mComponent.mMStateMachine->Load("../game_param/TestTask.xml", mIDkey);
  BL_ASSERT(resLoadMSM);

	mComponent.mControlCamera->RotateDown(float(M_PI/4));
	mComponent.mControlCamera->SetDistObj(-5.0f);
	BindObj();
	InitForm();
}
//------------------------------------------------------------------------------------
void TClientDeveloperTool_TestTask::CreateObjects()
{
  TMatrix16 w;
  SetMatrixIdentity(&w);
  // биль€рдный шар
	SetMatrixRotateX(&w,float(M_PI/2));
  w._43 += 1;//приподн€ть шар над сукном
	pSphere = mComponent.mMOC->CreateObject(1);
  pSphere->SetWorld(&w);
  mComponent.mGraphicEngine->AddObject(pSphere);
  // плоскость
  SetMatrixIdentity(&w);
  IBaseObjectCommon* pPlane = mComponent.mMOC->CreateObject(2);
  pPlane->SetWorld(&w);
  mComponent.mGraphicEngine->AddObject(pPlane);
}
//---------------------------------------------------------------------------------------------
void TClientDeveloperTool_TestTask::Done()
{
  delete mForm;
  mForm = NULL;
}        
//---------------------------------------------------------------------------------------------
void TClientDeveloperTool_TestTask::InitLog()
{
  if(mFuncGetLogger)
    mFuncGetLogger()->Init("TestTask");
}
//---------------------------------------------------------------------------------------------
string TClientDeveloperTool_TestTask::GetPathXMLFile()
{
  return "resources.xml";
}
//---------------------------------------------------------------------------------------------
void TClientDeveloperTool_TestTask::BindObj()
{
	if(!mUseBind)
		mComponent.mControlCamera->Link(pSphere,IControlCamera::eCoord);
	else
		mComponent.mControlCamera->Unlink();
	mUseBind = !mUseBind;
}
//---------------------------------------------------------------------------------------------
void TClientDeveloperTool_TestTask::InitForm()
{
	TVector3 v = *(mComponent.mGraphicEngine->GetLightPosition(0));
	TSphericalDecart::TSpheric s;
	TSphericalDecart sd;
	sd.Decart2Shperic(v,s);// из декартовых в сферические
	
	mForm->SetPitch(s.theta);
	mForm->SetYaw(s.fi);
	mForm->SetDist(s.r);
}
//---------------------------------------------------------------------------------------------
