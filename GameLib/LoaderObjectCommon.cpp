/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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
#include "GlobalParamsTank.h"
#include "Logger.h"
#include "BL_Debug.h"

using namespace std;

const char* sNameSettingMap = "main.xml";

TLoaderObjectCommon::TLoaderObjectCommon()
{
  LoadListMap();
}
//-------------------------------------------------------------
TLoaderObjectCommon::~TLoaderObjectCommon()
{
  Done();
}
//-------------------------------------------------------------
IBaseObjectCommon* TLoaderObjectCommon::LoadObject(unsigned int id_model)
{
  return mMakerObject.New(id_model);
}
//-------------------------------------------------------------
void TLoaderObjectCommon::LoadMap(unsigned int id_map)
{
  string sPath = mMapID[id_map];
  if(sPath.length()==0) return;

  Done();
  // ���������� ��������
}
//-------------------------------------------------------------
void TLoaderObjectCommon::LoadListMap()
{
  // ������� ����, ���������� ������ ����� � ������ � ��������� � ������������� ������
  TLoaderListPathID loader;
  char sAbsPath[300];
  FindAbsPath(PATH_LIST_MAP,sAbsPath,sizeof(sAbsPath));
  if(loader.Load(sAbsPath,&mMapID)==false)
    BL_FIX_BUG();

  PrepareMapPath();
}
//-------------------------------------------------------------
int TLoaderObjectCommon::GetCountObject()
{
  return mVectorObject.size();
}
//-------------------------------------------------------------
IBaseObjectCommon* TLoaderObjectCommon::Get(int i)
{
  return mVectorObject[i];
}
//-------------------------------------------------------------
void TLoaderObjectCommon::Done()
{
  mVectorObject.clear();
}
//-------------------------------------------------------------
void TLoaderObjectCommon::PrepareMapPath()
{
  map<unsigned int,string>::iterator bit = mMapID.begin();
  map<unsigned int,string>::iterator eit = mMapID.end();
  
  while(bit!=eit)
  {
    bit->second += sNameSettingMap;
    bit++;
  }
}
//-------------------------------------------------------------
void TLoaderObjectCommon::Delete(IBaseObjectCommon* pObject)
{
  mMakerObject.Delete(pObject);
}
//-------------------------------------------------------------