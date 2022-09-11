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

#ifndef ShareMiscH
#define ShareMiscH

#include "TypeDef.h"

#define CHECK_RET(f) \
  if(f==false) \
  return false;
//--------------------------------------------------
#if defined( WIN32 )
#pragma pack(push, 1)
#endif

struct TIP_Port
{
  unsigned int ip;
  unsigned short port;
  TIP_Port(){}
  TIP_Port(unsigned int _ip, unsigned short _port)
  {
    ip   = _ip;
    port = _port;
  }
  void Set(unsigned int _ip, unsigned short _port)
  {
    ip   = _ip;
    port = _port;
  }
  bool operator < (const TIP_Port& right) const
  {
    if(ip < right.ip)
      return true;
    else
      if(ip == right.ip)
        if(port < right.port)
          return true;
    return false;
  }
};

#if defined( WIN32 )
#pragma pack(pop)
#endif



extern int SHARE_EI GetCountCoreCPU();

//--------------------------------------------------

#endif
