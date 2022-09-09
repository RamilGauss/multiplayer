/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
2011, 2012
===========================================================================
                        Common Information
"Tanks" GPL Source Code

This file is part of the "Tanks" GPL Source Code.

"Tanks" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"Tanks" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "Tanks" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "Tanks" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "Tanks" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef IGraphicEngineH
#define IGraphicEngineH

//#include "DXUT.h"
#include "glibconfig.h"
#include "Struct3D.h"
//#include "MakerDirectX_Realize.h"

class IBaseObjectGE;

class IGraphicEngine
{
protected:
  guint32 mTime_ms;// ����� ��� �������, ������������ ��� ��������

public:

  IGraphicEngine(){}
  virtual ~IGraphicEngine(){}

  virtual bool IsFullScreen() = 0;
  virtual void ToggleFullScreen() = 0;
  //------------------------------------------------------------------------
  virtual void Init() = 0;
  virtual void Work(guint32 time_ms) = 0;
  virtual bool HandleInternalEvent() = 0;// true - success, false - exit
  virtual void Done() = 0;
  virtual void GetResolutionFrame(int& h, int& w ) = 0;// ������ X8R8G8B8
  virtual void SetResolutionFrame(int  h, int  w ) = 0;// ������ X8R8G8B8
  //------------------------------------------------------------------------
  virtual void AddObject(IBaseObjectGE* pObject) = 0;
  virtual void Clear() = 0;
  // ������
  virtual void SetViewParams(nsStruct3D::TVector3* pvEyePt3, nsStruct3D::TVector3* pvLookatPt3) = 0;// ������������ ������

  // 21 ������� 2012 ���� ��������:
  // ���������� ������� ������, �� ������ �� ���������� ��������� ��������
  virtual void SetEffect(unsigned short id_effect/*���������� ������, ��. ������� ��������*/,
    nsStruct3D::TVector3* coord3,     // ���
    nsStruct3D::TVector3* orient3,    // ���������� �������
    guint32 time_past/* ������ �������, ��*/ = 0) = 0;
  virtual void SetViewFPS(bool val) = 0;
  // ��������� ���������
  // ###
  // ���� ��������� ����������� ������� �� ������. �� ������ ��������� ���������
  virtual int GetCountLight() = 0;
  virtual const float* GetCoordLight(int index) = 0;
  virtual const float* GetCoordAtLight(int index) = 0;
  virtual unsigned int GetColorLight(int index) = 0;
  virtual void SetCoordLight(int index,nsStruct3D::TVector3* m3) = 0;
  virtual void SetCoordAtLight(int index,nsStruct3D::TVector3* m3) = 0;
  virtual void SetColorLight(int index, unsigned int color) = 0;
  virtual void AddLight() = 0;
  virtual void RemoveLight(int index) = 0;
  // ###
  //----------------------------------------------------------------
  //                             ~INTERFACE
  //----------------------------------------------------------------
};


#endif

