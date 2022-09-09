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

#include "ControlCamera.h"
#include "MakerCamera.h"

TControlCamera::TControlCamera()
{
  TMakerCamera makerCamera;
  mCamera  = makerCamera.New();
  mLinkObj = NULL;
}
//---------------------------------------------------------------------------------------
TControlCamera::~TControlCamera()
{
  TMakerCamera makerCamera;
  makerCamera.Delete(mCamera);
}
//---------------------------------------------------------------------------------------
IBaseObject* TControlCamera::GetLinkedObject()
{
  return NULL;
}
//---------------------------------------------------------------------------------------
void TControlCamera::Link(IBaseObject* pObject, int mask)
{
  mLinkObj = pObject;
}
//---------------------------------------------------------------------------------------
void TControlCamera::Unlink()
{
  Link(NULL,0);
}
//---------------------------------------------------------------------------------------
ICamera* TControlCamera::GetCamera()
{
  return mCamera;
}
//---------------------------------------------------------------------------------------
void TControlCamera::SetView(nsStruct3D::TMatrix16* view)
{
  mCamera->SetView(view);
}
//---------------------------------------------------------------------------------------
void TControlCamera::SetProj(nsStruct3D::TMatrix16* proj)
{
  mCamera->SetProj(proj);
}
//---------------------------------------------------------------------------------------
const nsStruct3D::TMatrix16* TControlCamera::GetView()const
{
  return mCamera->GetView();
}
//---------------------------------------------------------------------------------------
const nsStruct3D::TMatrix16* TControlCamera::GetProj()const
{
  return mCamera->GetProj();
}
//---------------------------------------------------------------------------------------
const nsStruct3D::TVector3*  TControlCamera::GetEyePt()const
{
  return mCamera->GetEyePt();
}
//---------------------------------------------------------------------------------------
float TControlCamera::GetDistObj()
{
  return mDist;
}
//---------------------------------------------------------------------------------------
void TControlCamera::SetDistObj(float v)
{
  mDist = v;
}
//---------------------------------------------------------------------------------------
void TControlCamera::AddDistObj(float dV)
{
  mDist += dV;
}
//---------------------------------------------------------------------------------------
void TControlCamera::SetPosition(nsStruct3D::TVector3* pPos)
{

}
//---------------------------------------------------------------------------------------
void TControlCamera::MovePosition(float dist, nsStruct3D::TVector3* pDir)
{

}
//---------------------------------------------------------------------------------------
void TControlCamera::MoveForward(float dist)// вдоль осей камеры
{

}
//---------------------------------------------------------------------------------------
void TControlCamera::MoveRight(float dist)
{

}
//---------------------------------------------------------------------------------------
void TControlCamera::MoveUp(float dist)
{

}
//---------------------------------------------------------------------------------------
void TControlCamera::RotateDown(float angle)
{

}
//---------------------------------------------------------------------------------------
void TControlCamera::RotateRight(float angle)
{

}
//---------------------------------------------------------------------------------------
void TControlCamera::Roll(float angle)
{

}
//---------------------------------------------------------------------------------------
void TControlCamera::SetRotate(nsStruct3D::TVector3* pAngles)
{

}
//---------------------------------------------------------------------------------------