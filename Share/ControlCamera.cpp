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

#include "ControlCamera.h"
#include "MakerCamera.h"

using namespace nsStruct3D;

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
void TControlCamera::SetView(TMatrix16* view)
{
  mCamera->SetView(view);
}
//---------------------------------------------------------------------------------------
void TControlCamera::SetProj(TMatrix16* proj)
{
  mCamera->SetProj(proj);
}
//---------------------------------------------------------------------------------------
const TMatrix16* TControlCamera::GetView()
{
  return mCamera->GetView();
}
//---------------------------------------------------------------------------------------
const TMatrix16* TControlCamera::GetProj()
{
  return mCamera->GetProj();
}
//---------------------------------------------------------------------------------------
const TVector3*  TControlCamera::GetEyePt()
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
void TControlCamera::SetPosition(TVector3* pPos)
{

}
//---------------------------------------------------------------------------------------
void TControlCamera::MoveInDirection(float dist, TVector3* pDir)
{

}
//---------------------------------------------------------------------------------------
void TControlCamera::MoveForward(float dist)// ����� ���� ������
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
void TControlCamera::SetProjParams( float fFOV, float fAspect, float fNearPlane, float fFarPlane )
{

}
//----------------------------------------------------------------------------------------
void TControlCamera::SetOrient(TVector3* up, bool use)
{
  mCamera->SetOrient(up,use);
}
//----------------------------------------------------------------------------------------
void TControlCamera::ClearRotate()
{

}
//----------------------------------------------------------------------------------------
void TControlCamera::SetPositionLookAt(TVector3* pPosLookAt)
{

}
//----------------------------------------------------------------------------------------


