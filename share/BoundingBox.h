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


#ifndef BoundingBoxH
#define BoundingBoxH

#include "TypeDef.h"
#include "Struct3D.h"


#if defined( WIN32 )
#pragma pack(push, 1)
#endif


class SHARE_EI TBoundingBox
{
protected:
  nsStruct3D::TVector3 mMin;
  nsStruct3D::TVector3 mMax;

public:

  TBoundingBox();
  ~TBoundingBox();

  void SetBound(nsStruct3D::TVector3& min,nsStruct3D::TVector3& max);
  void AddBound(nsStruct3D::TVector3& min,nsStruct3D::TVector3& max);

  nsStruct3D::TVector3& Min(){return mMin;};
  nsStruct3D::TVector3& Max(){return mMax;};

  bool IsCorrect();

  bool Contains(nsStruct3D::TVector3& point);
  bool Collision(TBoundingBox& bb);
  float VolumeCollision(TBoundingBox& bb);// �.�. � ����
};

#if defined( WIN32 )
#pragma pack(pop)
#endif


#endif