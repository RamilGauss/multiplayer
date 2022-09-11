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


#include "ErrorReg.h"
#include <windows.h>

guint8 egSDK;
//------------------------------------------------------------------------------
static int GetStr( size_t code, char* buf, int len )
{
  len = ::FormatMessage(
      FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL,
      (DWORD)code,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
      buf,
      len,
      NULL );
  if( len )
  {
    if( len > 2 )
      while( (buf[len-1] == 0x0D) || (buf[len-1] == 0x0A) )
        buf[--len] = 0;
  }
  return len;
}
//------------------------------------------------------------------------------
static TDefErrorGroup deg = {
  GetStr,
  NULL,
  NULL
};
//------------------------------------------------------------------------------
void errSDK_Set( guint32 code, const char* file, int line, void* source )
{
  err_SetError( egSDK, code, file, line, source );
}
//------------------------------------------------------------------------------
guint32 errSDK_push()
{
  return GetLastError();
}
//------------------------------------------------------------------------------
void errSDK_pop( guint32 errCode )
{
  SetLastError( errCode );
}
//------------------------------------------------------------------------------
bool errSDK_Init(void)
{
  ERR_REGGROUP( egSDK, deg )
}
//------------------------------------------------------------------------------

