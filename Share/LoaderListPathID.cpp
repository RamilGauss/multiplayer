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

#include "LoaderListPathID.h"
#include "BL_Debug.h"
#include "glib/gmem.h"


using namespace std;

TLoaderListPathID::TLoaderListPathID()
{
  sCurrentPath[0] = '\0';
}
//--------------------------------------------------------------------------------------
TLoaderListPathID::~TLoaderListPathID()
{
}
//--------------------------------------------------------------------------------------
bool TLoaderListPathID::Load(char* sPath, map<unsigned int,string>* pMap)
{
  bool res = true;
  iniReader.Open(sPath);
  if(!FindCurrentPath(sPath))
    res = false;
  else
  {
    pMap->clear();
    // кол-во записей
    int cnt = iniReader.GetInteger("MAIN","cnt",0);
    if(cnt==0)
      res = false;
    else
      for(int i = 0 ; i < cnt ; i++ )
      {
        if(LoadPartPath(i,pMap)==false)
        {
          res = false;
          break;
        }
      }
  }

  iniReader.Close();
  return res;
}
//--------------------------------------------------------------------------------------
bool TLoaderListPathID::LoadPartPath(int i, map<unsigned int,string>* pMap)
{
  char strNumPart[20];
  sprintf(strNumPart,"PART%d",i);

  unsigned int id = iniReader.GetInteger(strNumPart,"id_model",0);
  char* sPath = iniReader.GetValue(strNumPart,"path");
  if(sPath)
  {
    char buffer[400];
    strcpy(buffer,sCurrentPath);
    strcat(buffer,sPath);
    strcat(buffer,"\\");
    pMap->insert(map<unsigned int,string>::value_type(id,string(buffer)));
    
    g_free(sPath);
    sPath = NULL;
  }
  else
    return false;

  return true;
}
//--------------------------------------------------------------------------------------
bool TLoaderListPathID::FindCurrentPath(char* sPath)
{
  int len = strlen(sPath);
  for(int i = len-1 ; i > 0 ; i--)
  {
    if(sPath[i]=='\\')
    {
      strncpy(sCurrentPath,sPath,i+1);
      sCurrentPath[i+1] = '\0';
      return true;
    }
  }
  return false;
}
//--------------------------------------------------------------------------------------
bool TLoaderListPathID::LoadBehavior(char* sPath, std::map<unsigned int,unsigned int>* pMapID_model_ID_Behavior)
{
  bool res = true;
  iniReader.Open(sPath);
  if(!FindCurrentPath(sPath))
    res = false;
  else
  {
    pMapID_model_ID_Behavior->clear();
    // кол-во записей
    int cnt = iniReader.GetInteger("MAIN","cnt",0);
    if(cnt==0)
      res = false;
    else
      for(int i = 0 ; i < cnt ; i++ )
      {
        if(LoadPartID_Behavior(i,pMapID_model_ID_Behavior)==false)
        {
          res = false;
          break;
        }
      }
  }

  iniReader.Close();
  return res;
}
//--------------------------------------------------------------------------------------
bool TLoaderListPathID::LoadPartID_Behavior(int i, map<unsigned int,unsigned int>* pMap)
{
  char strNumPart[20];
  sprintf(strNumPart,"PART%d",i);

  unsigned int id_model = iniReader.GetInteger(strNumPart,"id_model",0);
  unsigned int id_behavior = iniReader.GetInteger(strNumPart,"id_behavior",0);
  pMap->insert(map<unsigned int,unsigned int>::value_type(id_model,id_behavior));

  return true;
}
//--------------------------------------------------------------------------------------
 