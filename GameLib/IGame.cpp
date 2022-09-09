/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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

#include "IGame.h"
#include <stddef.h>
#include "DeveloperTool_DLL.h"

#ifdef WIN32
  #include <windows.h>
  #include "BL_Debug.h"
  HMODULE hModule = NULL;
#else // MacOS �����

#endif


IGame::IGame()
{
  mGetClientDeveloperTool = NULL;
  mGetServerDeveloperTool = NULL;
  mFreeDeveloperTool      = NULL;

  mClientDeveloperTool = NULL;
  mServerDeveloperTool = NULL;
}
//----------------------------------------------------------------------
IGame::~IGame()
{
  if(mFreeDeveloperTool)
  {
    if(mFreeDeveloperTool) 
      mFreeDeveloperTool(mClientDeveloperTool);
    if(mFreeDeveloperTool) 
      mFreeDeveloperTool(mServerDeveloperTool);
  }

  FreeLib();
}
//----------------------------------------------------------------------
bool IGame::LoadDLL(const char* sNameDLL)
{
  if(mClientDeveloperTool!=NULL||
     mServerDeveloperTool!=NULL)
  {
    BL_FIX_BUG();// ���� ���, ����� ����� ������ �����������
    return true;
  }
#ifdef WIN32
  hModule = LoadLibraryA(sNameDLL);
  if(hModule==NULL)
  {
    BL_FIX_BUG();
    return false;
  }

  mFreeDeveloperTool = (FreeDeveloperTool)GetProcAddress(hModule, StrFreeDeveloperTool);
  if(mFreeDeveloperTool==NULL)
  {
    BL_FIX_BUG();
    return false;
  }
  mGetClientDeveloperTool = (GetClientDeveloperTool)GetProcAddress(hModule, StrGetClientDeveloperTool);
  mGetServerDeveloperTool = (GetServerDeveloperTool)GetProcAddress(hModule, StrGetServerDeveloperTool);

#endif

  if(mGetClientDeveloperTool)
    mClientDeveloperTool = mGetClientDeveloperTool();
  if(mGetServerDeveloperTool)
    mServerDeveloperTool = mGetServerDeveloperTool();

  return true;
}
//----------------------------------------------------------------------
void IGame::FreeLib()
{
  bool res = false;
#ifdef WIN32
  res = (bool)FreeLibrary(hModule);
#else
#endif
  BL_ASSERT(res);
}
//----------------------------------------------------------------------
