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

#ifndef ICameraH
#define ICameraH

#include "Struct3D.h"
#include "TypeDef.h"

// ��������������� ����� � ����� ������� �� ���������� �� ������� �������.

// ���������� ����� 3 �������� ������������� ������:
// 1. ��������� ������ (���������� ����� ���� � ����������, ����������)."��������� �����"
// ������ 2 �����: 1 - ��� Roll, 2 - � Roll
//    ������������  ICamera 
// 2. �������� � ������� (������ �������� ��� ��������� ���� �� �������)."Linked Object"
// ������ 2 �����: 1 - � ��������� ������ �� �����������, 2 - � ���������� � ����������
//    ������������  IControlCamera 
// 3. ��� ��������, �� � �������� ������� �������������� � ���������� (�����������)."�����������"
//    ������������  ICamera 

// ���������� - ����� ����� ��������� ���������� �������. �� �������� ������������������ ���������
// �� ����� � ����� ������� ���� ��������, �����������.
// ����������� - ����� ������� �����������
class SHARE_EI ICamera
{
public:
  virtual ~ICamera(){};

  virtual void Update() = 0;
  // ������ ��������� �����������
  virtual const nsStruct3D::TMatrix16* GetView() = 0;
  virtual const nsStruct3D::TVector3*  GetEyePt()= 0;
  virtual const nsStruct3D::TMatrix16* GetProj() = 0;

  // ������� ���������
  virtual void SetView(nsStruct3D::TMatrix16* view) = 0;
  virtual void SetProj(nsStruct3D::TMatrix16* proj) = 0;

  virtual void SetProjParams( float fFOV, float fAspect, float fNearPlane, float fFarPlane ) = 0;

  // ���� ������ ������������� ����� ���������, �� ����� ������ �� ������������� Roll(...),
  // ���� ������ � ������ Roll
  // ������� � �����, ��������, ��� ������������� �� � ������� ��� �������.
  virtual void SetOrient(nsStruct3D::TVector3* up, bool use = true) = 0;

  // �����������
  // ���������
  virtual void SetPositionLookAt(nsStruct3D::TVector3* pPosLookAt)     = 0;
  virtual void SetPosition(nsStruct3D::TVector3* pPos)                 = 0;
  virtual void MoveInDirection(float dist, nsStruct3D::TVector3* pDir) = 0;
  virtual void MoveForward(float dist)                                 = 0;// ����� ���� ������
  virtual void MoveRight(float dist)                                   = 0;
  virtual void MoveUp(float dist)                                      = 0;

  // �������� ��������
  virtual void ClearRotate()            = 0;
  // ������� 
  virtual void RotateDown(float angle)  = 0;
  virtual void RotateRight(float angle) = 0;
  virtual void Roll(float angle)        = 0;

  virtual void SetDir(nsStruct3D::TVector3* right, nsStruct3D::TVector3* up, nsStruct3D::TVector3* lookat) = 0;
};

#endif
