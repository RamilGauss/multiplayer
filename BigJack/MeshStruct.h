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


#ifndef MeshStructH
#define MeshStructH

#include "Struct3D.h"

// бинарный формат хранения Mesh
// только вершины (координаты, нормали, текстурные координаты) и индексы
namespace nsMeshStruct
{
  // Vertex format
#ifdef WIN32
#pragma pack(push, 1)
#endif
  struct VERTEX
  {
    nsStruct3D::TVector3 position;
    nsStruct3D::TVector3 normal;
    nsStruct3D::TVector2 texcoord;

    bool operator == ( const VERTEX& v) const
    {
      if((position!=v.position)||
         (normal  !=v.normal  )||
         (texcoord!=v.texcoord))
         return false;
      return true;
    }
    bool operator != ( const VERTEX& v) const
    {
      if((position!=v.position)||
         (normal  !=v.normal  )||
         (texcoord!=v.texcoord))
        return true;
      return false;
    }
    bool operator < ( const VERTEX& v) const
    {
      if(position < v.position) return true;// высший разряд
      else if(position > v.position) return false;
      //-------------------------------------------
      if(normal < v.normal) return true;
      else if(normal > v.normal) return false;
      //-------------------------------------------
      if(texcoord < v.texcoord) // низший разряд
        return true;

      return false;
    }
    bool operator > ( const VERTEX& v) const
    {
      if(position > v.position) return true;// высший разряд
      else if(position < v.position) return false;
      //-------------------------------------------
      if(normal > v.normal) return true;
      else if(normal < v.normal) return false;
      //-------------------------------------------
      if(texcoord > v.texcoord) // низший разряд
        return true;

      return false;
    }

  }_PACKED;
#ifdef WIN32
#pragma pack(pop)
#endif
}


#endif
