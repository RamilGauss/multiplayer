/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012
===========================================================================
                        Common Information
"Tanks" GPL Source Code

This file is part of the "Tanks" GPL Source Code.

"Tanks" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"Tanks" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "Tanks" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "Tanks" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "Tanks" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 

#include "ManagerObjectCommonEditorModel.h"
#include "namespace_ID_BEHAVIOR.h"
#include "BaseObjectCommon.h"
#include "Logger.h"
#include <map>
#include "HiTimer.h"
#include "ManagerObjectCommon.h"

using namespace nsID_BEHAVIOR;
using namespace std;

#define LOG_TIME_LOAD_EDITOR_MODEL

const char* strPathGESetting  = "EditorModelGE.xml";
const char* strPathNETSetting = "EditorModelNET.xml";
const char* strPathGUISetting = "EditorModelGUI.xml";

//-------------------------------------------------------------------
TManagerObjectCommonEditorModel::TManagerObjectCommonEditorModel()
:TManagerObjectCommon()
{
  flgNeedLoadModel = true;

  BL_ASSERT(mControlEventGE.LoadSettings(strPathGESetting));
  BL_ASSERT(mControlEventNET.LoadSettings(strPathNETSetting));
  BL_ASSERT(mControlEventGUI.LoadSettings(strPathGUISetting));

  mManagerEventWinApi.Register(&mControlEventGE);
  mManagerEventWinApi.Register(&mControlEventNET);
  mManagerEventWinApi.Register(&mControlEventGUI);

  InitLogger("Gauss");
}
//-------------------------------------------------------------------
TManagerObjectCommonEditorModel::~TManagerObjectCommonEditorModel()
{
  mManagerEventWinApi.Unregister(&mControlEventGE);
  mManagerEventWinApi.Unregister(&mControlEventNET);
  mManagerEventWinApi.Unregister(&mControlEventGUI);
}
//-------------------------------------------------------------------
void TManagerObjectCommonEditorModel::Work()
{
  float time_ms = GetTimeWork();
  mMDX_Scene->Work(time_ms);
}
//--------------------------------------------------------------------
void TManagerObjectCommonEditorModel::OnMsg( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

}
//--------------------------------------------------------------------------------------------------------
void TManagerObjectCommonEditorModel::OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
  LoadModel();
}
//--------------------------------------------------------------------------------------------------------
void TManagerObjectCommonEditorModel::LoadModel()
{
  if(flgNeedLoadModel)
    flgNeedLoadModel = false;
  else
    return;

  TMakerObject maker;
  //TBaseObjectCommon* pObject = mLoaderObject.LoadObject(0);//####

  //AddObject(pObject);//###

  //map<string,int> mapUse;
  //mapUse.insert(map<string,int>::value_type(string("Hull"),    0));
  //mapUse.insert(map<string,int>::value_type(string("Turret"),  0));
  //mapUse.insert(map<string,int>::value_type(string("Gun"),     0));
  //mapUse.insert(map<string,int>::value_type(string("ChassisL"),0));
  //mapUse.insert(map<string,int>::value_type(string("ChassisR"),0));
  //mapUse.insert(map<string,int>::value_type(string("TrackL"),  0));
  //mapUse.insert(map<string,int>::value_type(string("TrackR"),  0));

  //pObject->GetDefaultMapUse(&mapUse);
  //pObject->SetMapUse(&mapUse);
  int cnt[3] = {3,3,3};
#ifdef LOG_TIME_LOAD_EDITOR_MODEL
  guint32 start = ht_GetMSCount();
#endif 
  CreateObject(cnt[0],cnt[1],cnt[2]);
#ifdef LOG_TIME_LOAD_EDITOR_MODEL
  start = ht_GetMSCount() - start;
  float v = start/float(cnt[0]*cnt[1]*cnt[2]);
  GlobalLoggerForm.WriteF_time("EditorModel: Время загрузки объектов t=%u,v=%f\n",start,v);
#endif
}
//--------------------------------------------------------------------------------------------------------
float TManagerObjectCommonEditorModel::GetTimeWork()
{
  return 0.0f;
}
//---------------------------------------------------------------------------------------------
void TManagerObjectCommonEditorModel::CreateObject(int cntK,int cntJ,int cntI)
{
  TMakerObject maker;
  D3DXMATRIXA16 w;
  D3DXMatrixIdentity(&w);
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
        TBaseObjectCommon* pObject = mLoaderObject.LoadObject(0);
        pObject->SetVelocity(0.1f);
        w._41 += sizeI;
        pObject->SetWorld(w);
        if((i==cntI-1)&&(j==cntJ-1)&&(k==cntK-1))
          pObject->SetAlphaTransparency(0.5f);
        AddObject(pObject);
      }
    }
  }
}
//---------------------------------------------------------------------------------------------
