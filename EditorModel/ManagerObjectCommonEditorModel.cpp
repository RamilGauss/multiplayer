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

using namespace nsID_BEHAVIOR;
using namespace std;

//-------------------------------------------------------------------
TManagerObjectCommonEditorModel::TManagerObjectCommonEditorModel()
:TManagerObjectCommon()
{
  flgNeedLoadModel = true;

  InitLogger("Gauss");
}
//-------------------------------------------------------------------
TManagerObjectCommonEditorModel::~TManagerObjectCommonEditorModel()
{

}
//-------------------------------------------------------------------
void TManagerObjectCommonEditorModel::Work()
{
  float time_ms = GetTimeWork();
  mMDX_Scene.Work(time_ms);
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
  TBaseObjectCommon* pObject = mLoaderObject.LoadObject(0);

  AddObject(pObject);
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
}
//--------------------------------------------------------------------------------------------------------
float TManagerObjectCommonEditorModel::GetTimeWork()
{
  return 0.0f;
}
//---------------------------------------------------------------------------------------------