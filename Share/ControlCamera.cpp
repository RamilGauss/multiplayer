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
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 

#include "ControlCamera.h"
#include "MakerCamera.h"
#include "IBaseObject.h"
#include "HiTimer.h"
#include "Logger.h"

using namespace nsStruct3D;

TControlCamera::TControlCamera()
{
  TMakerCamera makerCamera;
  mCamera  = makerCamera.New();
  mLinkObj = NULL;
  mDist    = 0.0f;
  mMask    = eNotLinked;
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
  return mLinkObj;
}
//---------------------------------------------------------------------------------------
void TControlCamera::Link(IBaseObject* pObject, TypeLinked mask)
{
  mLinkObj = pObject;
  if(pObject)
    mMask    = mask;
}
//---------------------------------------------------------------------------------------
void TControlCamera::Unlink()
{
  Link(NULL,eNotLinked);
}
//---------------------------------------------------------------------------------------
//ICamera* TControlCamera::GetCamera()
//{
  //return mCamera;
//}
//---------------------------------------------------------------------------------------
//void TControlCamera::SetView(TMatrix16* view)
//{
//  mCamera->SetView(view);
//}
////---------------------------------------------------------------------------------------
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
  mCamera->SetPosition(pPos);
}
//---------------------------------------------------------------------------------------
void TControlCamera::MoveInDirection(float dist, TVector3* pDir)
{
  mCamera->MoveInDirection(dist,pDir);
}
//---------------------------------------------------------------------------------------
void TControlCamera::MoveForward(float dist)// вдоль осей камеры
{
  mCamera->MoveForward(dist);
}
//---------------------------------------------------------------------------------------
void TControlCamera::MoveRight(float dist)
{
  mCamera->MoveRight(dist);
}
//---------------------------------------------------------------------------------------
void TControlCamera::MoveUp(float dist)
{
  mCamera->MoveUp(dist);
}
//---------------------------------------------------------------------------------------
void TControlCamera::RotateDown(float angle)
{
  mCamera->RotateDown(angle);
}
//---------------------------------------------------------------------------------------
void TControlCamera::RotateRight(float angle)
{
  mCamera->RotateRight(angle);
}
//---------------------------------------------------------------------------------------
void TControlCamera::Roll(float angle)
{
  mCamera->Roll(angle);
}
//---------------------------------------------------------------------------------------
void TControlCamera::SetProjParams( float fFOV, float fAspect, float fNearPlane, float fFarPlane )
{
  mCamera->SetProjParams(fFOV, fAspect, fNearPlane, fFarPlane);
}
//----------------------------------------------------------------------------------------
void TControlCamera::SetOrient(TVector3* up, bool use)
{
  mCamera->SetOrient(up,use);
}
//----------------------------------------------------------------------------------------
void TControlCamera::ClearRotate()
{
  mCamera->ClearRotate();
}
//----------------------------------------------------------------------------------------
void TControlCamera::SetPositionLookAt(TVector3* pPosLookAt)
{
  mCamera->SetPositionLookAt(pPosLookAt);
}
//----------------------------------------------------------------------------------------
void TControlCamera::SetSpeedForward(float v)
{
  InitLastTime();
  mSpeedShift.x = v;
}
//----------------------------------------------------------------------------------------
void TControlCamera::SetSpeedRight(float v)
{
  InitLastTime();
  mSpeedShift.y = v;
}
//----------------------------------------------------------------------------------------
void TControlCamera::SetSpeedUp(float v)
{
  InitLastTime();
  mSpeedShift.z = v;
}
//----------------------------------------------------------------------------------------
void TControlCamera::ApplySpeed()
{
  if(mSpeedShift==TVector3()) 
    return;

  guint32 now = ht_GetMSCount();
  guint32 dt = now - mLastTime; // сколько прошло времени
  if(dt==0) 
    return;
  // новая позиция
  TVector3 v = mSpeedShift*float(dt)/1000.0f;//у.е./с * мс/1000
  MoveForward(mSpeedShift.x);
  MoveRight(mSpeedShift.y);
  MoveUp(mSpeedShift.z);
  mLastTime = now;
}
//----------------------------------------------------------------------------------------
void TControlCamera::InitLastTime()
{
  if(mSpeedShift==TVector3()) mLastTime = ht_GetMSCount();
}
//----------------------------------------------------------------------------------------
void TControlCamera::GetFromLinkObj()
{
  if(mLinkObj==NULL||mMask==eNotLinked) return;

  // прежде чем менять позицию применить все операции вращения 
  // (что бы не было подергивания. т.к. камера работает по принципу микро изменений)
  mCamera->UpdateForRender();

  // взять данные с объекта координаты и ориентацию по маске
  const TMatrix16* pM_Obj = mLinkObj->GetMatrixForCamera();

  const TMatrix16* pMCam = mCamera->GetView();

  TVector3 v;
  v.x = pM_Obj->_41;
  v.y = pM_Obj->_42;
  v.z = pM_Obj->_43;
  SetPosition(&v);

  if(mMask==eOrientAndCoord)
  {
    TMatrix16 m;
    SetMatrixInverse(&m,NULL,pM_Obj);
    
    TVector3 r = *((TVector3*)pM_Obj->m[0]);
    TVector3 l = *((TVector3*)pM_Obj->m[1]);
    TVector3 u = *((TVector3*)pM_Obj->m[2]);

    r *= -1;// 
    l *= -1;// 
    mCamera->SetDir(&r,&u,&l);
  }
  // отодвинуться от объекта
  MoveForward(mDist);
}
//----------------------------------------------------------------------------------------
void TControlCamera::SetDir(TVector3* right, TVector3* up, TVector3* lookat)
{
  mCamera->SetDir(lookat, up, right);
}
//----------------------------------------------------------------------------------------
void TControlCamera::UpdateForRender()
{
  // применить изменения координат с учетом времени и скорости 
  ApplySpeed();
  GetFromLinkObj();

  mCamera->UpdateForRender();
}
//----------------------------------------------------------------------------------------
