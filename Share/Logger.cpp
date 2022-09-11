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


#include "Logger.h"
#include <boost/foreach.hpp>

using namespace std;

class TLoggerOneObject : public TLogger
{
};
//-----------------------------------------------------------------------
TLoggerOneObject g_Logger;
TLogger* GetLogger()
{
  return (TLogger*)&g_Logger;
}
//-----------------------------------------------------------------------
void TLogger::InitLogger(TSaveOnHDD* saver, const char* sName)
{
  if(saver->IsOpen()) return;
  char nameLogFile[260];
  sprintf(nameLogFile,".\\%s%s.log",sName,sPrefix.data());
  saver->ReOpen(nameLogFile);
}
//-----------------------------------------------------------------------
void TLogger::Register(const char* nameLogger)
{
  TSaveOnHDD* pSoHDD = new TSaveOnHDD;
  mMapNamePtr.insert(TMapStrPtr::value_type(nameLogger,pSoHDD));
  if(sPrefix.length())
  {
    InitLogger(pSoHDD,nameLogger);
  }
}
//-----------------------------------------------------------------------
void TLogger::Init(char* prefix)
{
  sPrefix = prefix;
  
  BOOST_FOREACH( TMapStrPtr::value_type& bit, mMapNamePtr )
    InitLogger(bit.second,bit.first.data());
}
//-----------------------------------------------------------------------
TSaveOnHDD* TLogger::Get(const char* nameLog)
{
  TMapStrPtr::iterator fit = mMapNamePtr.find(nameLog);
  if(mMapNamePtr.end()!=fit)
    return fit->second;

  return NULL;
}
//-----------------------------------------------------------------------
void TLogger::Done()
{
  TMapStrPtr::iterator bit = mMapNamePtr.begin();
  TMapStrPtr::iterator eit = mMapNamePtr.end();
  while(bit!=eit)
  {  
    delete bit->second;
    bit++;
  }
  mMapNamePtr.clear();
}
//-----------------------------------------------------------------------
TLogger::~TLogger()
{
  Done();
}
//-----------------------------------------------------------------------
