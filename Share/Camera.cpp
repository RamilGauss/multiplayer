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
#define _USE_MATH_DEFINES

#include <cmath>

#include "Camera.h"
#include "BL_Debug.h"
#include <math.h>

using namespace nsStruct3D;

TCamera::TCamera()
{
  InitVector();
  mPosition = TVector3(0.0f,0.0f,0.0f);

  flgUseOrient = true;
  mNormal2Earth      = mUp;

  mRotateRight = mRotateLookAt = mRotateUp = 0.0f;
  SetMatrixIdentity(&mView);

  SetProjParams( float(M_PI) / 4.0f, 1.0f, 1.0f, 1000.0f);
}
//----------------------------------------------------------------------------------------
TCamera::~TCamera()
{

}
//----------------------------------------------------------------------------------------
void TCamera::SetView(nsStruct3D::TMatrix16* view)
{
  mChangedView = false; // �������� ��� ��� ������ �� �����
  MATRIX16_EQUAL_M_P(mView,view);
}
//----------------------------------------------------------------------------------------
void TCamera::SetProj(nsStruct3D::TMatrix16* proj)
{
  MATRIX16_EQUAL_M_P(mProj,proj);
}
//----------------------------------------------------------------------------------------
// ������ ��������� �����������
const nsStruct3D::TMatrix16* TCamera::GetView()
{
  UpdateView();
  return &mView;
}
//----------------------------------------------------------------------------------------
const nsStruct3D::TMatrix16* TCamera::GetProj()
{
  return &mProj;
}
//----------------------------------------------------------------------------------------
const nsStruct3D::TVector3* TCamera::GetEyePt()
{
  UpdateView();
  return &mPosition;
}
//----------------------------------------------------------------------------------------
// ���������
void TCamera::SetPosition(nsStruct3D::TVector3* pPos)
{
  mPosition = *pPos;
  mChangedView = true;
}
//----------------------------------------------------------------------------------------
void TCamera::MoveInDirection(float dist, nsStruct3D::TVector3* pDir)
{
  mPosition += (*pDir)*dist;
  mChangedView = true;
}
//----------------------------------------------------------------------------------------
void TCamera::MoveForward(float dist)
{
  mPosition += mLookAt*dist;
  mChangedView = true;
}
//----------------------------------------------------------------------------------------
void TCamera::MoveRight(float dist)
{
  mPosition += mRight*dist;
  mChangedView = true;
}
//----------------------------------------------------------------------------------------
void TCamera::MoveUp(float dist)
{
  mPosition += mUp*dist;
  mChangedView = true;
}
//----------------------------------------------------------------------------------------
// ������� 
void TCamera::RotateDown(float angle)
{
  mRotateRight += angle;
  mChangedView = true;
}
//----------------------------------------------------------------------------------------
void TCamera::RotateRight(float angle)
{
  mRotateUp += angle;
  mChangedView = true;
}
//----------------------------------------------------------------------------------------
void TCamera::Roll(float angle)
{
  mRotateLookAt += angle;
  mChangedView = true;
}
//----------------------------------------------------------------------------------------
void TCamera::UpdateView()
{
  if(mChangedView)
  {
    // ������� ��� ��������� ������������� ������������ ����
    TMatrix16 MatTotal;
    TMatrix16 MatRotateRight;
    TMatrix16 MatRotateUp;
    TMatrix16 MatRotateLookAt;

    // �������� ������� ��� ������� ���� ��������
    SetMatrixRotationAxis(&MatRotateRight,
                          &mRight,mRotateRight);
    SetMatrixRotationAxis(&MatRotateUp,
                          &mUp,mRotateUp);
    SetMatrixRotationAxis(&MatRotateLookAt,
                          &mLookAt,mRotateLookAt);
    // ��������� ������������� � ����� �������
    SetMatrixMultiply(&MatTotal,&MatRotateUp,
                      &MatRotateRight);
    SetMatrixMultiply(&MatTotal,&MatRotateLookAt,
                      &MatTotal);
    // �������������� ��� ������� �����������  ����� ������� � ���������
    // ������������ ��������
    SetVec3TransformCoord(&mRight,&mRight,&MatTotal);
    SetVec3TransformCoord(&mUp,&mUp,&MatTotal);

    SetVec3Cross(&mLookAt,&mRight,&mUp);

    // ��������� ������������������ ��������
    if(fabs(SetVec3Dot(&mUp,&mRight))>0.01f)
    {
      // ���� ��� �� ���������������
      SetVec3Cross(&mUp,&mLookAt,&mRight);
    }

    // ����������� ���� �������
    SetVec3Normalize(&mRight, &mRight);
    SetVec3Normalize(&mUp,    &mUp);
    SetVec3Normalize(&mLookAt,&mLookAt);

    // �������� � ������������ � �������� ������� � �����.
    if(flgUseOrient)
      NormalByEarth();

    // ��������� ������ ��� ������� ������
    float fView41, fView42, fView43;
    fView41 = -SetVec3Dot(&mRight,  &mPosition);
    fView42 = -SetVec3Dot(&mUp,     &mPosition);
    fView43 = -SetVec3Dot(&mLookAt, &mPosition);
    // ��������� ������� ������
    mView = TMatrix16( mRight.x, mUp.x,   mLookAt.x, 0.0f,
                       mRight.y, mUp.y,   mLookAt.y, 0.0f,
                       mRight.z, mUp.z,   mLookAt.z, 0.0f,
                       fView41,  fView42, fView43,   1.0f);

    mChangedView = false;
    mRotateRight = mRotateLookAt = mRotateUp = 0.0f;
  }
}
//----------------------------------------------------------------------------------------
void TCamera::SetProjParams( float fFOV, float fAspect, float fNearPlane, float fFarPlane )
{
  // Set attributes for the projection matrix
  mfFOV       = fFOV;
  mfAspect    = fAspect;
  mfNearPlane = fNearPlane;
  mfFarPlane  = fFarPlane;

  SetMatrixPerspectiveFovLH( &mProj, fFOV, fAspect, fNearPlane, fFarPlane );
}
//----------------------------------------------------------------------------------------
void TCamera::SetOrient(TVector3* up, bool use)
{
  mNormal2Earth      = *up;
  flgUseOrient = use;
}
//----------------------------------------------------------------------------------------
void TCamera::ClearRotate()
{
  InitVector();
  mChangedView = true;
}
//----------------------------------------------------------------------------------------
void TCamera::SetPositionLookAt(TVector3* pPosLookAt)
{
  ClearRotate();// ������� ����������
  mLookAt = mPosition - *pPosLookAt;
  if(mLookAt==TVector3(0,0,0))
    mLookAt=TVector3(0,1.0f,0);
  else
    SetVec3Normalize(&mLookAt,&mLookAt);

  if(mPosition.y>=0)
    SetVec3Cross(&mUp,&mLookAt,&mRight);
  else
    SetVec3Cross(&mUp,&mRight,&mLookAt);

  SetVec3Cross(&mRight,&mUp,&mLookAt);

  SetVec3Normalize(&mRight,&mRight);
  SetVec3Normalize(&mUp,&mUp);
}
//----------------------------------------------------------------------------------------
void TCamera::InitVector()
{
  mLookAt   = TVector3(0.0f,1.0f,0.0f);
  mRight    = TVector3(1.0f,0.0f,0.0f);
  mUp       = TVector3(0.0f,0.0f,1.0f);

  mRotateRight = mRotateLookAt = mRotateUp = 0.0f;
}
//----------------------------------------------------------------------------------------
void TCamera::NormalByEarth()
{
  // mRight ������ ���� ����������� � mNormal2Earth
  // ���� ���, �� ���������� ���� ����������� � ����������������� mRight � mUp
  float dotRN = SetVec3Dot(&mRight,&mNormal2Earth);
  if(fabs(dotRN)<0.001f) return;
  // ��������������
  //  ��������� ��� right
  TPlane planeRoll;
  SetPlaneFromPointNormal(&planeRoll,&mPosition,&mLookAt);
  // ��������� normal2earth
  TPlane planeN2E;
  SetPlaneFromPointNormal(&planeN2E,&mPosition,&mNormal2Earth);

  TLine lineRoll_N2E;
  lineRoll_N2E.FindAndSetIntersect(&planeRoll,&planeN2E);
  // ����� 2 ����� ��� 2 ��������, ��������� �� mPosition
  TVector3 v1,v2;
  lineRoll_N2E.FindVector(&v1,&v2,true);

  SetVec3Cross(&mUp,&mLookAt,&v1);
  float dotUpN2E = SetVec3Dot(&mUp,&mNormal2Earth);
  if(dotUpN2E<0)//  ���� ��������� ������������ ������������, �� ������� �� ������
  {
    SetVec3Cross(&mUp,&mLookAt,&v2);
    mRight = v2;
  }
  else
    mRight = v1;
  // ��������� ������ ����� �� ��������� ������������ mUp - ��� ������ ���� ������ ����

  SetVec3Normalize(&mRight,&mRight);
  SetVec3Normalize(&mUp,&mUp);
}
//----------------------------------------------------------------------------------------
