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

#ifndef MELISSA_NAVIGATE_SESSION_H
#define MELISSA_NAVIGATE_SESSION_H

#include <map>
#include <set>

#include "ShareMisc.h"

namespace nsMelissa
{
	class TSession;
  class TNavigateSession
  {
    typedef std::map<unsigned int,TSession*> TMapUintPtr;
    typedef TMapUintPtr::iterator TMapUintPtrIt;
    typedef TMapUintPtr::value_type TMapUintPtrVT;
    
    typedef std::map<TIP_Port,TSession*> TMapIP_Ptr;
    typedef TMapIP_Ptr::iterator TMapIP_PtrIt;
    typedef TMapIP_Ptr::value_type TMapIP_PtrItVT;

    typedef std::set<TSession*> TSetPtr;
    typedef TSetPtr::iterator TSetPtrIt;

    TMapUintPtr mMapID_Session;
    TMapIP_Ptr  mMapIP_Session;
    TSetPtr     mSetSession;

  public:
    TNavigateSession();
    ~TNavigateSession();
		
    void Add(TSession* pSession);
    void Delete(TSession* pSession);
		TSession* FindSessionByIP(TIP_Port& ip_port);
		TSession* FindSessionByID(unsigned int id);

		bool IsExist(TSession* pSession);

    std::set<TSession*>::iterator Begin();
    std::set<TSession*>::iterator End();

  protected:
  };
}

#endif