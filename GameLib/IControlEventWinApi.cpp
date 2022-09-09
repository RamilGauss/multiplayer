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


#include "IControlEventWinApi.h"
#include "MakerXML.h"
#include "file_operation.h"

using namespace std;

const char* sDirGameParam      = "game_param";

const char* SectionNode        = "node";
const char* SectionName_uMsg   = "msg";
const char* SectionName_Key    = "key";

IControlEventWinApi::IControlEventWinApi()
{  
  TMakerXML maker;
  mXML = maker.New();
}
//--------------------------------------------------------------------------------
IControlEventWinApi::~IControlEventWinApi()
{
  delete mXML;
}
//--------------------------------------------------------------------------------
bool IControlEventWinApi::LoadSettings(const char* name_file)
{
  CHECK_RET(FindPath(name_file))

  CHECK_RET(mXML->Load(strPath.data()))
  string root = mXML->GetNameSection(0);

  CHECK_RET(mXML->EnterSection(root.data(),0))

  int cnt = mXML->GetCountSection();
  for(int i = 0 ; i < cnt ; i++)
    CHECK_RET(AddNode(i))

  return true;
}
//--------------------------------------------------------------------------------
int IControlEventWinApi::GetValByKey(UINT uMsg)
{
  TMAP::iterator fit = mMapKey.find(uMsg);
  if(fit==mMapKey.end())
    return -1;

  return fit->second;
}
//--------------------------------------------------------------------------------
bool IControlEventWinApi::AddNode(int i)
{
  int msg,key;    
  CHECK_RET(mXML->EnterSection(SectionNode,i))

  CHECK_RET(mXML->ReadInt(SectionName_uMsg,0,msg))
  CHECK_RET(mXML->ReadInt(SectionName_Key, 0,key))

  mXML->LeaveSection();

  mMapKey.insert(TMAP::value_type(msg,key));
  return true;
}
//--------------------------------------------------------------------------------
bool IControlEventWinApi::FindPath(const char* name_file)
{
  CHECK_RET(GetCurrentPath(strPath))
  char sAbs[300];
  CHECK_RET(FindAbsPath((char*)strPath.data(),sAbs,sizeof(sAbs)-1))
  // подняться из текущей директории на уровень выше
  UpPath(sAbs);
  // в папке sDirGameParam находится файлы настроек
  strPath = sAbs;
  strPath += "\\";
  strPath += sDirGameParam;
  strPath += "\\";
  strPath += name_file;

  return true;
}
//--------------------------------------------------------------------------------