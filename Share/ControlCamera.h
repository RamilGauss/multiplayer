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

#ifndef ControlCameraH
#define ControlCameraH

#include "IControlCamera.h"

// Not Thread Safe!

class TControlCamera : public IControlCamera
{
  ICamera* mCamera;

  IBaseObject* mLinkObj;

  float mDist;
  
  TypeLinked mMask;

  // при изменении скорости
  nsStruct3D::TVector3 mSpeedShift;// x - forward, y - right, z - up
  guint32              mLastTime;// время изменения, мс

public:
  TControlCamera();
  virtual ~TControlCamera();

  virtual IBaseObject* GetLinkedObject();
  virtual void Link(IBaseObject* pObject, TypeLinked mask);
  virtual void Unlink();

  //virtual ICamera* GetCamera();

  // смещение от eye обзора. Например, от 3 лица вид координаты точка больше 0
  // от 1 лица координаты равны нулю
  // влияет на вращение камеры (задает центр вращения)
  virtual float GetDistObj();
  virtual void  SetDistObj(float v);
  virtual void  AddDistObj(float dV);

  // для свободной камеры
  virtual void SetSpeedForward(float v);
  virtual void SetSpeedRight(float v);
  virtual void SetSpeedUp(float v);
  //---------------------------------------------------
  // интерфейс камеры
  virtual void Update();
  // выдать результат манипуляций
  virtual const nsStruct3D::TMatrix16* GetView();
  virtual const nsStruct3D::TMatrix16* GetProj();
  virtual const nsStruct3D::TVector3*  GetEyePt();

  // базовая настройка
  virtual void SetView(nsStruct3D::TMatrix16* view);
  virtual void SetProj(nsStruct3D::TMatrix16* proj);
  virtual void SetProjParams( float fFOV, float fAspect, float fNearPlane, float fFarPlane );
  virtual void SetOrient(nsStruct3D::TVector3* up, bool use = true);

  // положение
  virtual void SetPositionLookAt(nsStruct3D::TVector3* pPosLookAt);
  virtual void SetPosition(nsStruct3D::TVector3* pPos);
  virtual void MoveInDirection(float dist, nsStruct3D::TVector3* pDir);
  virtual void MoveForward(float dist);// вдоль осей камеры
  virtual void MoveRight(float dist);
  virtual void MoveUp(float dist);

  // сбросить вращение
  virtual void ClearRotate();
  // вращать 
  virtual void RotateDown(float angle);
  virtual void RotateRight(float angle);
  virtual void Roll(float angle);

  virtual void SetDir(nsStruct3D::TVector3* right, nsStruct3D::TVector3* up, nsStruct3D::TVector3* lookat);

protected:
  void ApplySpeed();
  void InitLastTime();
  void GetFromLinkObj();
};

#endif