/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
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

#include <string.h>

#include "ClientDeveloperTool_ViewerModel.h"
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
#include "IXML.h"
#include "Client.h"

using namespace std;
using namespace nsStruct3D;
using namespace nsEvent;
using namespace nsMelissa;

using namespace nsKey;

#define LOG_TIME_LOAD_EDITOR_MODEL

#define DELTA_MOVE 0.2f //0.4f
#define DELTA_ROTATE 0.01f
#define ASPECT_MOUSE_X 0.002f
#define ASPECT_MOUSE_Y 0.002f

#define UP 1000000

TClientDeveloperTool_ViewerModel::TClientDeveloperTool_ViewerModel()
{
  flgDragCamera = false;

  mClientMain      = NULL;
  mGameRoomPrepare = NULL;
  mWaitForm        = NULL;

  mMakerObjectCommon = new TMakerObjectCommon;
  
  mIndexCurObj = 0;
}
//------------------------------------------------------------------------------------
TClientDeveloperTool_ViewerModel::~TClientDeveloperTool_ViewerModel()
{
  delete mMakerObjectCommon;
}
//------------------------------------------------------------------------------------
void TClientDeveloperTool_ViewerModel::MouseEvent(TMouseEvent* pEvent)
{
  switch(pEvent->state)
  {
    case nsEvent::eButtonUp:
      flgDragCamera = false;
      break;
    case nsEvent::eButtonDown:
      flgDragCamera = true;
      mOldX = pEvent->x;
      mOldY = pEvent->y;
      break;
    case nsEvent::eButtonDblClick:

      break;
    case nsEvent::eWheel:
      mComponent.mControlCamera->AddDistObj(float(pEvent->delta_wheel)/100.0f);
      break;
    case nsEvent::eMove:
      if(flgDragCamera)
      {
        float dRight = float(pEvent->x - mOldX)*ASPECT_MOUSE_X;
        float dDown  = float(pEvent->y - mOldY)*ASPECT_MOUSE_Y;
        mComponent.mControlCamera->RotateRight(dRight);
        mComponent.mControlCamera->RotateDown(dDown);
        
        mOldX = pEvent->x;
        mOldY = pEvent->y;
      }
			break;
  }
}
//------------------------------------------------------------------------------------
void TClientDeveloperTool_ViewerModel::KeyEvent(TKeyEvent* pEvent)
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
    case e_Numpad7:// вращаться влево 7
      mComponent.mControlCamera->Roll(DELTA_ROTATE);
      break;
    case e_Numpad9:// вращаться вправо 9
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
      IBaseObject* pBO = mComponent.mMOC->Get(mIndexCurObj);
      mComponent.mControlCamera->Link(pBO,IControlCamera::eCoord);
      mIndexCurObj++;
      mIndexCurObj %= mComponent.mMOC->GetCountObject();
      break;
    }
    case e_N: 
    {
      mComponent.mControlCamera->Unlink();
      break;
    }
    case e_O: 
    {
      break;
    }
  }
}
//--------------------------------------------------------------------
void TClientDeveloperTool_ViewerModel::PrepareForRender()
{

}
//------------------------------------------------------------------------------------
IMakerObjectCommon* TClientDeveloperTool_ViewerModel::GetMakerObjectCommon()
{
  return mMakerObjectCommon;
}
//------------------------------------------------------------------------------------
string TClientDeveloperTool_ViewerModel::GetTitleWindow()
{
  return "Просмоторщик моделей";
}
//------------------------------------------------------------------------------------
void TClientDeveloperTool_ViewerModel::Init(TComponentClient* pComponent, const char* arg )
{
  InitLog();
  mComponent = *pComponent; 

  int cnt[3] = {3,3,3};
#ifdef LOG_TIME_LOAD_EDITOR_MODEL
  unsigned int start = ht_GetMSCount();
#endif 
  CreateObjects(cnt[0],cnt[1],cnt[2]);
#ifdef LOG_TIME_LOAD_EDITOR_MODEL
  start = ht_GetMSCount() - start;
  float v = start/float(cnt[0]*cnt[1]*cnt[2]);
  mFuncGetLogger()->Get("Form")->WriteF_time("ViewerModel: Время загрузки объектов t=%u мс,v=%f мс/об.\n",start,v);
#endif
  // GUI
  mClientMain      = new TClientMain;
  mGameRoomPrepare = new TGameRoomPrepare;
  mWaitForm        = new TWaitForm;
  
  mComponent.mGUI->Add(string("mClientMain"),mClientMain);
  mComponent.mGUI->Add(string("mGameRoomPrepare"),mGameRoomPrepare);
  mComponent.mGUI->Add(string("mWaitForm"),mWaitForm);
  // показать форму
  mClientMain->Show();
  //mGameRoomPrepare->Show();
  //mWaitForm->Show();
  // подстроиться
  mComponent.mGUI->Resize();
  // HotKey
  bool resLoadMSM = mComponent.mMStateMachine->Load("../game_param/ViewerModel.xml", mIDkey);
  BL_ASSERT(resLoadMSM);
}
//------------------------------------------------------------------------------------
void TClientDeveloperTool_ViewerModel::CreateObjects(int cntK,int cntJ,int cntI)
{
  TMatrix16 w;
  SetMatrixIdentity(&w);
  float sizeK = 4,sizeJ = 12, sizeI = 5;

  w._43 = -((cntK+1)*sizeK)/2;
  for(int k = 0 ; k < cntK ; k ++)
  {
    w._42 = -((cntJ+1)*sizeJ)/2;
    w._43 += sizeK;
    for(int j = 0 ; j < cntJ ; j ++)
    {
      w._41  = -((cntI+1)*sizeI)/2;
      w._42 += sizeJ;
      for(int i = 0 ; i < cntI ; i ++)
      {
        IBaseObjectCommon* pBOC = mComponent.mMOC->CreateObject(0);

        pBOC->SetVelocity(0.1f);
        w._41 += sizeI;
        pBOC->SetWorld(&w);
        if((i==cntI-1)&&(j==cntJ-1)&&(k==cntK-1))
          pBOC->SetAlphaTransparency(0.5f);
        mComponent.mGraphicEngine->AddObject(pBOC);
      }
    }
  }
}
//---------------------------------------------------------------------------------------------
void TClientDeveloperTool_ViewerModel::Done()
{
  delete mClientMain;
  mClientMain = NULL;
  delete mGameRoomPrepare;
  mGameRoomPrepare = NULL;
  delete mWaitForm;
  mWaitForm = NULL;
}        
//---------------------------------------------------------------------------------------------
void TClientDeveloperTool_ViewerModel::InitLog()
{
  if(mFuncGetLogger)
  {
    mFuncGetLogger()->Register("Form");// для логирования внутренних событий
    mFuncGetLogger()->Init("ViewerModel");
  }
}
//---------------------------------------------------------------------------------------------
string TClientDeveloperTool_ViewerModel::GetPathXMLFile()
{
  return "resources.xml";
}
//---------------------------------------------------------------------------------------------
