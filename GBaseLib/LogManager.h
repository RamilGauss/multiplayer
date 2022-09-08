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


#ifndef LogManagerH
#define LogManagerH

#include "AutoCreateVar.h"
#include "BL_Log.h"
#include "ILogConsole.h"
#include "LogStd.h"
#include "GCS.h"
#include <list>

class GBASELIB_EI TLogManager : protected GCS
{
  TLogStdErr mLogStdErr;
  TLogStdOut mLogStdOut;
public:
	TLogManager();
	virtual ~TLogManager();

	void Register(ILogConsole* console);
	void Unregister(ILogConsole* console);

  void Link( TBL_Log* writer );
  void Link( TBL_Log* writer, const char* consoleName );
  void Unlink( TBL_Log* writer );

private:
  typedef std::list<ILogConsole*> TListConsole;
  typedef std::list<TLogWriter*>  TListWriter;                 

  #ifdef _MSC_VER
    # pragma warning(push)
    # pragma warning( disable : 4251 )
      TListConsole  mConsoles;
      TListWriter   mWriters;
    # pragma warning(pop)
  #else
    TListConsole  mConsoles;
    TListWriter   mWriters;
  #endif

  ILogConsole* Find( const char* name );
  ILogConsole* CheckConsole(const char* consoleName);
  void setGroupComment();
  void parseDescConsole( TBL_Log* writer, const char* descConsole );

};

extern GBASELIB_EI TAutoCreateVarT<TLogManager> LogManager;

#endif // !defined(EA_B762BF23_D62E_41d5_9EDB_DDC4145FEB5D__INCLUDED_)
