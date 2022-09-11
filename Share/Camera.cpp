/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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
#define _USE_MATH_DEFINES

#include <cmath>

#include "Camera.h"
#include "BL_Debug.h"
#include <math.h>
#include "HiTimer.h"

using namespace nsStruct3D;

TCamera::TCamera():
defFOV(float(M_PI) / 4.0f),
defAspect(1.0f),
defNearPlan(1.0f),
defFarPlan(1000.0f),
defDotProductMin(0.000001f)
{
  InitVector();
  mPosition = TVector3(0.0f,0.0f,0.0f);

  flgUseOrient  = true;
  mNormal2Earth = mUp;

  mRotateRight = mRotateLookAt = mRotateUp = 0.0f;
  SetMatrixIdentity(&mView);

  SetProjParams( defFOV, defAspect, defNearPlan, defFarPlan);
}
//----------------------------------------------------------------------------------------
TCamera::~TCamera()
{

}
//----------------------------------------------------------------------------------------
void TCamera::SetView(TMatrix16* view)
{
  MATRIX16_EQUAL_M_P(mView,view);
  TMatrix16 mInv;
  SetMatrixInverse(&mInv,NULL,&mView);
  mRight    = TVector3(mInv._11,mInv._12,mInv._13);
  mUp       = TVector3(mInv._21,mInv._22,mInv._23);
  mLookAt   = TVector3(mInv._31,mInv._32,mInv._33);
  mPosition = TVector3(mInv._41,mInv._42,mInv._43);
}
//----------------------------------------------------------------------------------------
void TCamera::SetProj(TMatrix16* proj)
{
  MATRIX16_EQUAL_M_P(mProj,proj);
}
//----------------------------------------------------------------------------------------
// ������ ��������� �����������
const TMatrix16* TCamera::GetView()
{
  return &mView;
}
//----------------------------------------------------------------------------------------
const TMatrix16* TCamera::GetProj()
{
  return &mProj;
}
//----------------------------------------------------------------------------------------
const TVector3* TCamera::GetEyePt()
{
  return &mPosition;
}
//----------------------------------------------------------------------------------------
// ���������
void TCamera::SetPosition(TVector3* pPos)
{
  mPosition = *pPos;
  mChangedView = true;
}
//----------------------------------------------------------------------------------------
void TCamera::MoveInDirection(float dist, TVector3* pDir)
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
  if(flgUseOrient)
  {
    angle *= 2;
    float a;
    TQuaternion axis;
    SetQuaternionRotationAxis(&axis,&mRight,angle);// ������ ���� ��� ����� ������������ ��������
    //-----------------------------------------
    TQuaternion rot = TQuaternion(mLookAt.x,mLookAt.y,mLookAt.z,0);// ���� ������ ����� ���������
    SetQuaternionMultiply(&rot,&rot,&axis);// �������
    SetQuaternionToAxisAngle(&rot,&mLookAt,&a);// �������� ����� ������ �, ��� �������� �������, ����
    //-----------------------------------------
    rot = TQuaternion(mUp.x,mUp.y,mUp.z,0);
    SetQuaternionMultiply(&rot,&rot,&axis);
    SetQuaternionToAxisAngle(&rot,&mUp,&a);
  }
  else
    mRotateRight += angle;
  mChangedView = true;
}
//----------------------------------------------------------------------------------------
void TCamera::RotateRight(float angle)
{
  if(flgUseOrient)
  {
    angle *= 2;
    float a;
    TQuaternion axis;
    SetQuaternionRotationAxis(&axis,&mNormal2Earth,angle);
    //-----------------------------------------
    TQuaternion rot = TQuaternion(mLookAt.x,mLookAt.y,mLookAt.z,0);
    SetQuaternionMultiply(&rot,&rot,&axis);
    SetQuaternionToAxisAngle(&rot,&mLookAt,&a);
    //-----------------------------------------
    rot = TQuaternion(mUp.x,mUp.y,mUp.z,0);
    SetQuaternionMultiply(&rot,&rot,&axis);
    SetQuaternionToAxisAngle(&rot,&mUp,&a);
    //-----------------------------------------
    rot = TQuaternion(mRight.x,mRight.y,mRight.z,0);
    SetQuaternionMultiply(&rot,&rot,&axis);
    SetQuaternionToAxisAngle(&rot,&mRight,&a);
  }
  else
    mRotateUp += angle;
  mChangedView = true;
}
//----------------------------------------------------------------------------------------
void TCamera::Roll(float angle)
{
  if(flgUseOrient)
  {/*�������*/}
  else
  {
    mRotateLookAt += angle;
    mChangedView = true;
  }
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
    if(fabs(SetVec3Dot(&mUp,&mRight))>defDotProductMin)
    {
      // ���� ��� �� ���������������
      SetVec3Cross(&mUp,&mLookAt,&mRight);
    }

    // ����������� ���� �������
    SetVec3Normalize(&mRight, &mRight);
    SetVec3Normalize(&mUp,    &mUp);
    SetVec3Normalize(&mLookAt,&mLookAt);

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
  mNormal2Earth = *up;
  flgUseOrient  = use;
  if(flgUseOrient)
    NormalByEarth();// ��������������� Roll
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
  if(flgUseOrient)
    NormalByEarth();// ��������������� Roll

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

  if(flgUseOrient)
    NormalByEarth();// ��������������� Roll
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
  if(fabs(dotRN) < defDotProductMin) return;
  // ��������������
  //  ���������, ���������� � lookat
  TPlane planeRoll;
  SetPlaneFromPointNormal(&planeRoll,&mPosition,&mLookAt);
  // ���������, ���������� � normal2earth
  TPlane planeN2E;
  SetPlaneFromPointNormal(&planeN2E,&mPosition,&mNormal2Earth);

  TLine lineRoll_N2E;
  lineRoll_N2E.FindAndSetIntersect(&planeRoll,&planeN2E);
  // ����� 2 ������� ��� ������� ��������� ������ � ������������ (� 2 �������)
  TVector3 v1,v2;
  lineRoll_N2E.FindVector(&v1,&v2,true);// ����� �������� ����� �������������

  SetVec3Cross(&mUp,&mLookAt,&v1);
  float dotUpN2E = SetVec3Dot(&mUp,&mNormal2Earth);
  if(dotUpN2E<0)//  ���� ��������� ������������ ������������, �� ������� �� ������
  {
    SetVec3Cross(&mUp,&mLookAt,&v2);
    mRight = v2;
  }
  else
    mRight = v1;

  SetVec3Normalize(&mRight,&mRight);
  SetVec3Normalize(&mUp,&mUp);
}
//----------------------------------------------------------------------------------------
void TCamera::SetDir(TVector3* right, TVector3* up, TVector3* lookat)
{
  mLookAt = *lookat;
  mRight  = *right;
  mUp     = *up;
  mChangedView = true;
}
//----------------------------------------------------------------------------------------
void TCamera::Update()
{
  UpdateView();  
}
//----------------------------------------------------------------------------------------
