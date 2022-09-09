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

#include "LoaderObjectCommon.h"
#include <stddef.h>
#include "LoaderListPathID.h"
#include "file_operation.h"
#include "GlobalParams.h"
#include "Logger.h"
#include "BL_Debug.h"

using namespace std;

const char* sNameSettingMap = "main.xml";

TLoaderMap::TLoaderMap()
{
  LoadListMap();
}
//-------------------------------------------------------------
TLoaderMap::~TLoaderMap()
{
  Done();
}
//-------------------------------------------------------------
IBaseObjectCommon* TLoaderMap::LoadObject(unsigned int id_model)
{
  return mMakerObjectCommon->New(id_model);
}
//-------------------------------------------------------------
void TLoaderMap::LoadMap(unsigned int id_map)
{
  string sPath = mMapID[id_map];
  if(sPath.length()==0) return;

  Done();
  // собственно загрузка
}
//-------------------------------------------------------------
void TLoaderMap::LoadListMap()
{
  // открыть файл, содержащий список путей к картам и запомнить в ассоциативный список
  TLoaderListPathID loader;
  char sAbsPath[300];
  FindAbsPath(PATH_LIST_MAP,sAbsPath,sizeof(sAbsPath));
  if(loader.Load(sAbsPath,&mMapID)==false)
    BL_FIX_BUG();

  PrepareMapPath();
}
//-------------------------------------------------------------
int TLoaderMap::GetCountObject()
{
  return mVectorObject.size();
}
//-------------------------------------------------------------
IBaseObjectCommon* TLoaderMap::Get(int i)
{
  return mVectorObject[i];
}
//-------------------------------------------------------------
void TLoaderMap::Done()
{
  mVectorObject.clear();
}
//-------------------------------------------------------------
void TLoaderMap::PrepareMapPath()
{
  TMapIDName::iterator bit = mMapID.begin();
  TMapIDName::iterator eit = mMapID.end();
  
  while(bit!=eit)
  {
    bit->second += sNameSettingMap;
    bit++;
  }
}
//-------------------------------------------------------------
void TLoaderMap::Delete(IBaseObjectCommon* pObject)
{
  mMakerObjectCommon->Delete(pObject);
}
//-------------------------------------------------------------
void TLoaderMap::SetMakerObjectCommon(IMakerObjectCommon* pMakerObjectCommon)
{
  mMakerObjectCommon = pMakerObjectCommon;
}
//-------------------------------------------------------------
