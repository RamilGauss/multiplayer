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

#ifndef CameraH
#define CameraH

#include "ICamera.h"
#include <list>

// Not Thread Safe!

class TCamera : public ICamera
{
  bool flgUseOrient;
  nsStruct3D::TVector3 mNormal2Earth;

  nsStruct3D::TMatrix16 mView;
  nsStruct3D::TMatrix16 mProj;

  nsStruct3D::TVector3  mPosition;
  nsStruct3D::TVector3  mLookAt;
  nsStruct3D::TVector3  mRight;
  nsStruct3D::TVector3  mUp;

  bool mChangedView;

  float mRotateUp;
  float mRotateRight;
  float mRotateLookAt;

  // для матрицы проекции
  float mfFOV;
  float mfAspect;
  float mfNearPlane;
  float mfFarPlane;

public:
  TCamera();
  virtual ~TCamera();

  virtual void UpdateForRender();
  // выдать результат манипуляций
  virtual const nsStruct3D::TMatrix16* GetView();
  virtual const nsStruct3D::TMatrix16* GetProj();
  virtual const nsStruct3D::TVector3*  GetEyePt();

  // базовая настройка
  //virtual void SetView(nsStruct3D::TMatrix16* view);
  virtual void SetProj(nsStruct3D::TMatrix16* proj);

  virtual void SetProjParams( float fFOV, float fAspect, float fNearPlane, float fFarPlane );
  virtual void SetOrient(nsStruct3D::TVector3* up, bool use = true);
  //-------------------------------------------
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
  void UpdateView();

  void InitVector();

  void NormalByEarth();
};

#endif
