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

#include "file_operation.h"

#include <stdlib.h>
#include <stdio.h>
#include <string>

#include <boost/filesystem.hpp>

using namespace boost::filesystem;
//--------------------------------------------------------------------------------
bool FindAbsPath(char* sRelativePath, char* sAbsPath, int lenAbs)
{
  bool res = false;
  try
  {
    path p = sRelativePath;
    path canonicalPath = canonical(p);

    if(lenAbs<=int(canonicalPath.string().size()))
      return false;
    strcpy(sAbsPath, canonicalPath.string().data());
    res = true;
  }
  catch(std::exception e)
  {}
  return res;
}
//--------------------------------------------------------------------------------
void UpPath(char* path)
{
  int cnt = strlen(path);
  for(int i = cnt-1 ; i > 0 ; i--)
    if((path[i]=='\\')|| 
       (path[i]=='\\'))
    {
      path[i]='\0';
      return;
    }
}
//--------------------------------------------------------------------------------
bool GetCurrentPath(std::string &sPath)
{
  boost::system::error_code ec;
  path p = current_path(ec);
  if(ec)
    return false;
  sPath = p.string();
  return true;
}
//--------------------------------------------------------------------------------
