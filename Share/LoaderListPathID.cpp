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
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 

#include "LoaderListPathID.h"
#include "BL_Debug.h"
#include "glib/gmem.h"
#include "MakerXML.h"


using namespace std;

const char* Section_ID          = "id";
const char* Section_path        = "path";
const char* Section_ID_behavior = "id_behavior";

TLoaderListPathID::TLoaderListPathID()
{
  sCurrentPath[0] = '\0';
  TMakerXML makerXML;
  mXML = makerXML.New();
}
//--------------------------------------------------------------------------------------
TLoaderListPathID::~TLoaderListPathID()
{
  delete mXML;
}
//--------------------------------------------------------------------------------------
bool TLoaderListPathID::Load(char* sAbsPath, map<unsigned int,string>* pMap)
{
  bool res = true;
  CHECK_RET(mXML->Load(sAbsPath))
  if(!FindCurrentPath(sAbsPath))
    res = false;
  else
  {
    pMap->clear();
    // кол-во записей
    string sRoot = mXML->GetNameSection(0);
    if(sRoot.length()==0) return false;
    CHECK_RET(mXML->EnterSection(sRoot.data(),0))
    //--------------------------------------
    int cnt = mXML->GetCountSection();
    if(cnt==0)
      res = false;
    else
    {
      for(int i = 0 ; i < cnt ; i++ )
      {
        if(LoadPartPath(i,pMap)==false)
        {
          res = false;
          break;
        }
      }
    }
    //--------------------------------------
    mXML->LeaveSection();
  }

  return res;
}
//--------------------------------------------------------------------------------------
bool TLoaderListPathID::LoadPartPath(int i, map<unsigned int,string>* pMap)
{
  string sPart = mXML->GetNameSection(i);
  CHECK_RET(mXML->EnterSection(sPart.data(),i))
  //------------------------------------------------
  unsigned int id;
  CHECK_RET(mXML->ReadUint(Section_ID,0,id))
  string sPath = mXML->ReadSection(Section_path,0);
  if(sPath.length()==0) return false;
  //----------------------------------
  char buffer[400];
  strcpy(buffer,sCurrentPath);
  strcat(buffer,sPath.data());
  strcat(buffer,"\\");
  pMap->insert(map<unsigned int,string>::value_type(id,string(buffer)));
  //------------------------------------------------
  mXML->LeaveSection();

  return true;
}
//--------------------------------------------------------------------------------------
bool TLoaderListPathID::FindCurrentPath(char* sPath)
{
  int len = strlen(sPath);
  for(int i = len-1 ; i > 0 ; i--)
  {
    if((sPath[i]=='\\')||(sPath[i]=='/'))
    {
      strncpy(sCurrentPath,sPath,i+1);
      sCurrentPath[i+1] = '\0';
      return true;
    }
  }
  return false;
}
//--------------------------------------------------------------------------------------
bool TLoaderListPathID::LoadBehavior(char* sAbsPath, std::map<unsigned int,unsigned int>* pMapID_model_ID_Behavior)
{
  bool res = true;
  CHECK_RET(mXML->Load(sAbsPath))
  if(!FindCurrentPath(sAbsPath))
    res = false;
  else
  {
    pMapID_model_ID_Behavior->clear();
    // кол-во записей
    string sRoot = mXML->GetNameSection(0);
    if(sRoot.length()==0) return false;
    CHECK_RET(mXML->EnterSection(sRoot.data(),0))
    //--------------------------------------
    int cnt = mXML->GetCountSection();
    if(cnt==0)
      res = false;
    else
    {
      for(int i = 0 ; i < cnt ; i++ )
      {
        if(LoadPartID_Behavior(i,pMapID_model_ID_Behavior)==false)
        {
          res = false;
          break;
        }
      }
    }
    //--------------------------------------
    mXML->LeaveSection();
  }

  return res;
}
//--------------------------------------------------------------------------------------
bool TLoaderListPathID::LoadPartID_Behavior(int i, map<unsigned int,unsigned int>* pMap)
{
  string sPart = mXML->GetNameSection(i);
  CHECK_RET(mXML->EnterSection(sPart.data(),i))
  //------------------------------------------------
  unsigned int id,id_behavior;
  CHECK_RET(mXML->ReadUint(Section_ID,0,id))
  CHECK_RET(mXML->ReadUint(Section_ID_behavior,0,id_behavior))
  pMap->insert(map<unsigned int,unsigned int>::value_type(id,id_behavior));
  //------------------------------------------------
  mXML->LeaveSection();

  return true;
}
//--------------------------------------------------------------------------------------
 