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


#ifndef MapXML_FieldH
#define MapXML_FieldH

#include <map>
#include <string>
#include "IXML.h"
#include "TypeDef.h"

#ifdef WIN32
#pragma warning(disable:4251)
#endif

class SHARE_EI TMapXML_Field
{
  typedef std::multimap<std::string,std::string> TMultiMapStrStr;

  TMultiMapStrStr mMap;

  IXML* mXML;
    
public:

  TMapXML_Field();
  ~TMapXML_Field();
  
  bool Load(const char* sAbsPath);

	const char* GetSecond(const char* name, unsigned int index = 0);
  unsigned int GetCount(const char* name);

protected:
};

#endif