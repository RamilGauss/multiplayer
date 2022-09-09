/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
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
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 

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
#include <string.h>

using namespace std;
using namespace nsStruct3D;
using namespace nsEvent;

#define LOG_TIME_LOAD_EDITOR_MODEL

TClientDeveloperTool_ViewerModel::TClientDeveloperTool_ViewerModel()
{
  mClientMain      = NULL;
  mGameRoomPrepare = NULL;
  mWaitForm        = NULL;

  mMakerObjectCommon = new TMakerObjectCommon;
}
//------------------------------------------------------------------------------------
TClientDeveloperTool_ViewerModel::~TClientDeveloperTool_ViewerModel()
{
  delete mMakerObjectCommon;
}
//------------------------------------------------------------------------------------
bool TClientDeveloperTool_ViewerModel::MouseEvent(TMouseEvent* pEvent)
{
  int a = 0;
  switch(pEvent->state)
  {
    case nsEvent::eButtonUp:
      a = 1;
      break;
    case nsEvent::eButtonDown:
      a = 2;
      break;
    case nsEvent::eButtonDblClick:
      a = 3;
      break;
    case nsEvent::eWheel:
      a = 4;
      break;
  }
  return true;
}
//------------------------------------------------------------------------------------
bool TClientDeveloperTool_ViewerModel::KeyEvent(TKeyEvent* pEvent)
{
  return true;
}
//--------------------------------------------------------------------
void TClientDeveloperTool_ViewerModel::Calc()
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
void TClientDeveloperTool_ViewerModel::Init(TComponentClient* pComponent)
{
  InitLog();
  mComponent = *pComponent; 

  int cnt[3] = {3,3,3};
#ifdef LOG_TIME_LOAD_EDITOR_MODEL
  guint32 start = ht_GetMSCount();
#endif 
  CreateObjects(cnt[0],cnt[1],cnt[2]);
#ifdef LOG_TIME_LOAD_EDITOR_MODEL
  start = ht_GetMSCount() - start;
  float v = start/float(cnt[0]*cnt[1]*cnt[2]);
  mFuncGetLogger()->Get("Form")->WriteF_time("ViewerModel: Время загрузки объектов t=%u мс,v=%f мс/об.\n",start,v);
#endif

  mClientMain      = new TClientMain;
  mGameRoomPrepare = new TGameRoomPrepare;
  mWaitForm        = new TWaitForm;
  
  mComponent.mGUI->Add(std::string("mClientMain"),mClientMain);
  mComponent.mGUI->Add(std::string("mGameRoomPrepare"),mGameRoomPrepare);
  mComponent.mGUI->Add(std::string("mWaitForm"),mWaitForm);
  // показать форму
  mClientMain->Show();
  //mGameRoomPrepare->Show();
  //mWaitForm->Show();
  // подстроиться
  mComponent.mGUI->Resize();
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
        mComponent.mGraphicEngine->AddObject((IBaseObjectGE*)pBOC);
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
    mFuncGetLogger()->Init("ViewerModel");
}
//---------------------------------------------------------------------------------------------
