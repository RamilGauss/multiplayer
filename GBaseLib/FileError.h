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


#ifndef FileErrorH
#define FileErrorH

#include "ErrorReg.h"
//DECLARE_CLASS( FileError )

///////////////////////////////////////////////////////////////////////////
/////////////////// Класс регистрации ошибок файла  ///////////////////////
///////////////////////////////////////////////////////////////////////////

class GBASELIB_EI TFileError
{
protected:
  char name[MAX_PATH];
  void *errSource;
public:
  TFileError( const char *_name = NULL );

  void SetFileName( const char *_name );
  const char* GetFileName() const { return name; }

  void SetErrSource( void* _errSource )
  { errSource = _errSource; }

  void SetError( guint8 group, guint32 code, const char* file, int line );
  void SetErrorStr( const char* cp, const char* text, const char* file, int line );
  void SetErrorSTD( guint32 code, const char* file, int line );
};

//-----------------------------------------------------------------------------
#define ODFfile_SetError( group, code )        \
  SetError( group, code, __FILE__, __LINE__ )

#define ODFfile_SetErrorStr( cp, text )        \
  SetErrorStr( cp, text, __FILE__, __LINE__ )

#define ODFfile_SetErrorFileSystem()           \
{ if( errno ) SetErrorSTD( errno, __FILE__, __LINE__ ); }
//-----------------------------------------------------------------------------

#endif // FileErrorH
