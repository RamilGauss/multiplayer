/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
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



#ifndef LogFileH
#define LogFileH

#include "ILogConsole.h"
#include "BL_IOStreamFile.h"

class GBASELIB_EI TLogFile : public ILogConsole
{
public:
  TLogFile( const char* nameFile, bool enable = true );
	virtual ~TLogFile();

public: // ILogConsole
	virtual const char* name() const;
	virtual void write(const char* str, int len = -1, bool eol = true );

private:
	TIOStreamFile od;
  char* mName;
  bool mEnable;
  bool mCheckOpenFile;
  
  bool enable();
};
#endif // !defined(EA_C954E270_5042_4aad_B28A_46F9438FEBA6__INCLUDED_)
