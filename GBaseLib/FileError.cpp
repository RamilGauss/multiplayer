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


#include "FileError.h"
#include <string.h>

//-----------------------------------------------------------------------------
TFileError::TFileError( const char *_name )
{
  errSource = NULL;
  if( _name )
    SetFileName( _name );
  else
    name[0] = '\0';
}
//-----------------------------------------------------------------------------
void TFileError::SetFileName( const char *_name )
{
  if( _name ) strcpy( name, _name );
  else        name[0] = '\0';
}
//-----------------------------------------------------------------------------
void TFileError::SetError( guint8 group, guint32 code, const char* file, int line )
{
  if( !errSource && name[0] ) errSource =  err_RegisterSource( name ); 
  err_SetError( group, code, file, line, errSource );
}
//-----------------------------------------------------------------------------
void TFileError::SetErrorStr( const char* cp, const char* text, const char* file, int line )
{
  if( !errSource && name[0] ) errSource = err_RegisterSource( name );
  errStr_Set( cp, text, file, line, errSource );
}
//-----------------------------------------------------------------------------
void TFileError::SetErrorSTD( guint32 code, const char* file, int line )
{
  if( !errSource && name[0] ) errSource =  err_RegisterSource( name );
  errSTD_Set( code, file, line, errSource );
}
//-----------------------------------------------------------------------------
