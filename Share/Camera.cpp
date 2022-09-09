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

#include "Camera.h"

TCamera::TCamera()
{

}
//----------------------------------------------------------------------------------------
TCamera::~TCamera()
{

}
//----------------------------------------------------------------------------------------
void TCamera::SetView(nsStruct3D::TMatrix16* view)
{
  MATRIX16_EQUAL_M_P(mView,view);
}
//----------------------------------------------------------------------------------------
void TCamera::SetProj(nsStruct3D::TMatrix16* proj)
{
  MATRIX16_EQUAL_M_P(mProj,proj);
}
//----------------------------------------------------------------------------------------
// выдать результат манипуляций
const nsStruct3D::TMatrix16* TCamera::GetView()const
{
  return &mView;
}
//----------------------------------------------------------------------------------------
const nsStruct3D::TMatrix16* TCamera::GetProj()const
{
  return &mProj;
}
//----------------------------------------------------------------------------------------
const nsStruct3D::TVector3* TCamera::GetEyePt()const
{
  return NULL;
}
//----------------------------------------------------------------------------------------
// положение
void TCamera::SetPosition(nsStruct3D::TVector3* pPos)
{

}
//----------------------------------------------------------------------------------------
void TCamera::MovePosition(float dist, nsStruct3D::TVector3* pDir)
{

}
//----------------------------------------------------------------------------------------
void TCamera::MoveForward(float dist)
{

}
//----------------------------------------------------------------------------------------
void TCamera::MoveRight(float dist)
{

}
//----------------------------------------------------------------------------------------
void TCamera::MoveUp(float dist)
{

}
//----------------------------------------------------------------------------------------
// вращать 
void TCamera::RotateDown(float angle)
{

}
//----------------------------------------------------------------------------------------
void TCamera::RotateRight(float angle)
{

}
//----------------------------------------------------------------------------------------
void TCamera::Roll(float angle)
{

}
//----------------------------------------------------------------------------------------
void TCamera::SetRotate(nsStruct3D::TVector3* pAngles)
{

}
//----------------------------------------------------------------------------------------
