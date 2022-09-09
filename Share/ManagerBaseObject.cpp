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
the "Tanks" Source Code.  If not, please request a copy in writing from id Software at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 

#include "ManagerBaseObject.h"
#include "GlobalParamsTank.h"
#include "file_operation.h"
#include "LoaderListPathID.h"
#include "BaseObject.h"
#include "BL_Debug.h"

using namespace std;

TManagerBaseObject::TManagerBaseObject()
{
  LoadListPath();
}
//-------------------------------------------------------------------------------------------
TManagerBaseObject::~TManagerBaseObject()
{
  Done();
}
//-------------------------------------------------------------------------------------------
void TManagerBaseObject::AddObject(TBaseObject* pObject)
{
  TTreeJoint::TLoadedJoint* pLoaded = NULL;
  unsigned int id = pObject->GetID_Model();
  map<unsigned int, TTreeJoint::TLoadedJoint*>::iterator fit = mMapID_Tree.find(id);
  map<unsigned int, TTreeJoint::TLoadedJoint*>::iterator eit = mMapID_Tree.end();
  if(fit!=eit)
    pLoaded = (TTreeJoint::TLoadedJoint*)fit->second;
  else
  {
    map<unsigned int,string>::iterator it = mMapID_Path.find(id);
    if(mMapID_Path.end()==it) {BL_FIX_BUG();return;}
    if(mLoaderTree.Load((char*)it->second.data())==false) {BL_FIX_BUG();return;}
    pLoaded = mLoaderTree.TakeTree();

    map<unsigned int, TTreeJoint::TLoadedJoint*>::value_type val(id,pLoaded);
    mMapID_Tree.insert(val);
  }
  pObject->SetTree(pLoaded);
}
//-------------------------------------------------------------------------------------------
void TManagerBaseObject::Done()
{
  map<unsigned int, TTreeJoint::TLoadedJoint*>::iterator bit = mMapID_Tree.begin();
  map<unsigned int, TTreeJoint::TLoadedJoint*>::iterator eit = mMapID_Tree.end();
  while(bit!=eit)
  {
    delete bit->second;
    bit++;
  }
  mMapID_Tree.clear();
}
//-------------------------------------------------------------------------------------------
bool TManagerBaseObject::LoadListPath()
{
  TLoaderListPathID loader;
  char sAbsPath[300];
  FindAbsPath(PATH_LIST_MODELS,sAbsPath,sizeof(sAbsPath));
  if(loader.Load(sAbsPath,&mMapID_Path)==false)
  {
    BL_FIX_BUG();
    return false;
  }

  PrepareForTreeJoint();

  return true;
}
//--------------------------------------------------------------------------------------
void TManagerBaseObject::PrepareForTreeJoint()
{
  std::map<unsigned int, std::string>::iterator bit = mMapID_Path.begin();
  std::map<unsigned int, std::string>::iterator eit = mMapID_Path.end();
  while(bit!=eit)
  {
    (*bit).second += "joint.ini";
    bit++;
  }
}
//--------------------------------------------------------------------------------------
