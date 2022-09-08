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


#include "LogFile.h"
#include <glib.h>
#include <string.h>

//-----------------------------------------------------------------------------
TLogFile::TLogFile(const char* nameFile, bool enable )
{
  mEnable = enable;
  mCheckOpenFile = false;
  mName = g_strdup( nameFile );
}
//-----------------------------------------------------------------------------
TLogFile::~TLogFile()
{
  g_free( mName );
}
//-----------------------------------------------------------------------------
bool TLogFile::enable()
{
  bool res;
  if( !mEnable ) res = false;
  else
  {
    if( mCheckOpenFile ) res = od.isOpen();
    else
    {
      lock();
      if( mCheckOpenFile ) mEnable = od.isOpen();
      else
      {
        mEnable = od.create( mName, false, true, true );
        mCheckOpenFile = true;
      }
      unlock();
      res = mEnable;
    }
  }
  return res;
}
//-----------------------------------------------------------------------------
void TLogFile::write(const char* str, int len, bool eol )
{
  lock();
  if( enable() )
  {    
    if( eol )
      od.writeStringEOL( str, len );
    else
    {
      if( len == -1 )
        len = str ? (int)strlen( str ) : 0;
      od.put( str, len );
    }
    od.flush();    
  }
  unlock();  
}
//-----------------------------------------------------------------------------
const char* TLogFile::name() const 
{
	return mName;
}
//-----------------------------------------------------------------------------
