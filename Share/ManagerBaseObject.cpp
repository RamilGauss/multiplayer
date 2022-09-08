/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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

#include "ManagerBaseObject.h"
#include "GlobalParams.h"
#include "file_operation.h"
#include "LoaderListPathID.h"
#include "IBaseObject.h"
#include "BL_Debug.h"
#include "StorePathResources.h"
#include "MapXML_Field.h"

using namespace std;

const char* NameJointFile = "joint.xml";

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
void TManagerBaseObject::AddObject(IBaseObject* pObject)
{
  SetTree(pObject);
}
//-------------------------------------------------------------------------------------------
void TManagerBaseObject::SetTree(IBaseObject* pObject)
{
  TTreeJoint::TLoadedJoint* pLoaded = NULL;
  unsigned int id = pObject->GetID_Model();
  TMapID_Tree::iterator fit = mMapID_Tree.find(id);
  TMapID_Tree::iterator eit = mMapID_Tree.end();
  if(fit!=eit)
    pLoaded = (TTreeJoint::TLoadedJoint*)fit->second;
  else
  {
    TMapID_Path::iterator it = mMapID_Path.find(id);
    if(mMapID_Path.end()==it) {BL_FIX_BUG();return;}
    if(mLoaderTree.Load((char*)it->second.data())==false) {BL_FIX_BUG();return;}
    pLoaded = mLoaderTree.TakeTree();

    TMapID_Tree::value_type val(id,pLoaded);
    mMapID_Tree.insert(val);
  }
  pObject->SetTree(pLoaded);
}
//-------------------------------------------------------------------------------------------
void TManagerBaseObject::Done()
{
  TMapID_Tree::iterator bit = mMapID_Tree.begin();
  TMapID_Tree::iterator eit = mMapID_Tree.end();
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
  FindAbsPath((char*)GetStorePathResources()->GetSecond("model"),
              sAbsPath,sizeof(sAbsPath));
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
  TMapID_Path::iterator bit = mMapID_Path.begin();
  TMapID_Path::iterator eit = mMapID_Path.end();
  while(bit!=eit)
  {
    (*bit).second += NameJointFile;
    bit++;
  }
}
//--------------------------------------------------------------------------------------
