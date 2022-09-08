/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
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


#include "MakerXML.h"
#include "MapXML_Field.h"

using namespace std;

TMapXML_Field::TMapXML_Field()
{
  TMakerXML makerXML;
  mXML = makerXML.New();
}
//--------------------------------------------------------------------------------------
TMapXML_Field::~TMapXML_Field()
{
  TMakerXML makerXML;
  makerXML.Delete(mXML);
  mXML = NULL;
}
//--------------------------------------------------------------------------------------
bool TMapXML_Field::Load(const char* sAbsPath)
{
  bool res = true;
  CHECK_RET(mXML->Load(sAbsPath))
  mMap.clear();
  string sRoot = mXML->GetNameSection(0);
  if(sRoot.length()==0) return false;
  CHECK_RET(mXML->EnterSection(sRoot.data(),0))
  int cntChild = mXML->GetCountSection();
  for(int i = 0 ; i < cntChild ; i++)
  {
    string sChild = mXML->GetNameSection(i);
    string sPath = mXML->ReadSection(i);
    mMap.insert(TMultiMapStrStr::value_type(sChild,sPath));
  }

  CHECK_RET(mXML->LeaveSection());
  return res;
}
//--------------------------------------------------------------------------------------
const char* TMapXML_Field::GetSecond(const char* name, unsigned int index)
{
  unsigned int cnt = GetCount(name);
  if(cnt<index+1) return NULL;// перебор

  TMultiMapStrStr::iterator fit = mMap.lower_bound(name);
  for(unsigned int i = 0 ; i < index ; i++)
    fit++;
  return fit->second.data();
}
//--------------------------------------------------------------------------------------
unsigned int TMapXML_Field::GetCount(const char* name)
{
  return mMap.count(name);
}
//--------------------------------------------------------------------------------------
