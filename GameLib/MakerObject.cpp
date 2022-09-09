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

#include "MakerObject.h"
#include <stddef.h>

#include "namespace_ID_BEHAVIOR.h"
#include "TankTower.h"
#include "LoaderListPathID.h"
#include "GlobalParamsTank.h"
#include "file_operation.h"
#include "BL_Debug.h"

using namespace nsID_BEHAVIOR;
using namespace std;

TMakerObject::TMakerObject()
{
  TLoaderListPathID loader;
  char sAbsPath[300];
  FindAbsPath(PATH_LIST_MODELS,sAbsPath,sizeof(sAbsPath));
  if(loader.LoadBehavior(sAbsPath,&mMapID)==false)
    BL_FIX_BUG();
}
//------------------------------------------------------------------------
TMakerObject::~TMakerObject()
{

}
//------------------------------------------------------------------------
IBaseObjectCommon* TMakerObject::New(unsigned int id_model)
{
  unsigned int id_behavior = GetID_ModelByID_Behavior(id_model);
  IBaseObjectCommon* pObject = NewByID_Behavior(id_behavior);
  pObject->SetID_Model(id_model);
  return pObject;
}
//------------------------------------------------------------------------
IBaseObjectCommon* TMakerObject::NewByID_Behavior(unsigned int id_behavior)
{
  IBaseObjectCommon* pObject = NULL;
  switch(id_behavior)
  {
    case ID_TANK_TOWER:
      pObject = new TTankTower();
      break;
    default:BL_FIX_BUG();
  }
  return pObject;
}
//------------------------------------------------------------------------
unsigned int TMakerObject::GetID_ModelByID_Behavior(unsigned int id_model)
{
  map<unsigned int,unsigned int>::iterator fit = mMapID.find(id_model);
  if(fit == mMapID.end())
    return ID_NULL;
  return fit->second;
}
//------------------------------------------------------------------------
void TMakerObject::Delete(IBaseObjectCommon* pObject)
{
  delete pObject;
}
//------------------------------------------------------------------------
