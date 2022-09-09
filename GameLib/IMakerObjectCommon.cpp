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

#include "IMakerObjectCommon.h"
#include <stddef.h>

#include "LoaderListPathID.h"
#include "GlobalParams.h"
#include "file_operation.h"
#include "BL_Debug.h"
#include "IBaseObjectCommon.h"
#include "MapXML_Field.h"
#include "StorePathResources.h"

using namespace std;

IMakerObjectCommon::IMakerObjectCommon()
{
  flgNeedInit = true;
}
//------------------------------------------------------------------------
IMakerObjectCommon::~IMakerObjectCommon()
{

}
//------------------------------------------------------------------------
IBaseObjectCommon* IMakerObjectCommon::New(unsigned int id_model)
{
  if(flgNeedInit) Init();

  unsigned int id_behavior = GetID_ModelByID_Behavior(id_model);
  IBaseObjectCommon* pObject = NewByID_Behavior(id_behavior);
  pObject->SetID_Model(id_model);
  return pObject;
}
//------------------------------------------------------------------------
unsigned int IMakerObjectCommon::GetID_ModelByID_Behavior(unsigned int id_model)
{
  TMapUintUint::iterator fit = mMapID.find(id_model);
  if(fit == mMapID.end())
  {
    BL_FIX_BUG();
    return 0;
  }
  return fit->second;
}
//------------------------------------------------------------------------
void IMakerObjectCommon::Init()
{
  TLoaderListPathID loader;
  char sAbsPath[300];
  FindAbsPath((char*)GetStorePathResources()->GetSecond("model"),sAbsPath,sizeof(sAbsPath));
  if(loader.LoadBehavior(sAbsPath,&mMapID)==false)
    BL_FIX_BUG();

  flgNeedInit = false;
}
//------------------------------------------------------------------------
