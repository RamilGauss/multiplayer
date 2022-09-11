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

#include "CmdParam.h"

using namespace std;

TCmdParam::TCmdParam()
{

}
//-------------------------------------------------------------------------------
TCmdParam::~TCmdParam()
{

}
//-------------------------------------------------------------------------------
void TCmdParam::SetArg(vector<string>& vecArgv)
{
  mMapKey.clear();
  mSetFoundKey.clear();

  int cnt = vecArgv.size();
  for( int iKey = 0 ; iKey < cnt ; iKey++)
  {
    bool resFoundKey = FindInDefKey( vecArgv[iKey] );
    if(resFoundKey)
    {
      mSetFoundKey.insert(vecArgv[iKey]);
      // ищем значения после ключа
      int iValue = iKey + 1;
      while(iValue < cnt)
      {
        resFoundKey = FindInDefKey( vecArgv[iValue] );
        if(resFoundKey==false)
          mMapKey.insert(TMultiMapStrStr::value_type(vecArgv[iKey],vecArgv[iValue]));
        else
          break;
        iValue++;
      }
      iKey = iValue - 1;
    }
  }
}
//-------------------------------------------------------------------------------
int TCmdParam::GetCountValueByKey(string& sKey)
{
  return mMapKey.count(sKey);
}
//-------------------------------------------------------------------------------
bool TCmdParam::GetByKey(string& sKey, int index, string& sOut)
{
  TMultiMapStrStrIt fit = mMapKey.lower_bound(sKey);
  if(fit==mMapKey.end()) return false;
  for(int i = 0 ; i < index ; i++)
    fit++;
  sOut = fit->second;
  return true;
}
//-------------------------------------------------------------------------------
bool TCmdParam::IsKey(string& sKey)
{
  return (mSetFoundKey.find(sKey)!=mSetFoundKey.end());
}
//-------------------------------------------------------------------------------
void TCmdParam::SetDefKey(vector<string>& vecKey)
{
  mSetDefKey.clear();
  mSetDefKey.insert(vecKey.begin(),vecKey.end());
}
//-------------------------------------------------------------------------------
bool TCmdParam::FindInDefKey( string& sKey )
{
  return (mSetDefKey.find(sKey)!=mSetDefKey.end());
}
//-------------------------------------------------------------------------------

