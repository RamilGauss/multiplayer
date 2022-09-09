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

#include <windows.h>
#include <vector>
#include "MakerXML.h"
#include "SortByAlphabetic.h"
#include "ShaderStack.h"
#include "ManagerResourceDX.h"
#include "..\GameLib\EventWinApi.h"
#include <map>

using namespace std;


INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
{
  //TMakerXML m;
  //IXML* pXML = m.New();
  //pXML->Load("d:\\joint.xml");

  //float v4[4];
  //bool res;
  //string content;
  //int cntSection = pXML->GetCountSection();
  //res = pXML->EnterSection("root",0);// <-
  //  res = pXML->EnterSection("world",0);
  //    res = pXML->ReadFloat4("row",0,&v4[0]);
  //  pXML->LeaveSection();
  //res = pXML->LeaveSection();// ->
  //pXML->Save();

  //int size;
  //TShaderStack ss;
  //double d = 1234;
  //void* pD = (void*)&d;
  //ss.Push("double",pD,sizeof(double));
  //double* pDouble = (double*)ss.GetData(0,size);
  //string str = ss.GetName(0);
  //ss.Done();

  //TManagerResourceDX m;
  //m.Load(L"d:\\MyProjects\\multiPlayer\\shader\\shader.fx");

  //char ss[11][256];
  //strcpy(ss[0],"Hahaha");
  //char** pp = (char**)ss;
  //char* p = pp[0];
  //string str = string(&p[0]);

  map<TEventWinApi,int,CompareEventWinApi> mMapKey;

  //TEventWinApi(0,0,0);
  mMapKey.insert(map<TEventWinApi,int,CompareEventWinApi>::value_type(TEventWinApi(0,0,0),0));
  mMapKey.insert(map<TEventWinApi,int,CompareEventWinApi>::value_type(TEventWinApi(0,0,1),0));
  mMapKey.insert(map<TEventWinApi,int,CompareEventWinApi>::value_type(TEventWinApi(0,1,0),0));
  mMapKey.insert(map<TEventWinApi,int,CompareEventWinApi>::value_type(TEventWinApi(1,1,0),0));
  mMapKey.insert(map<TEventWinApi,int,CompareEventWinApi>::value_type(TEventWinApi(0,0,0),0));

  return 0;
}
