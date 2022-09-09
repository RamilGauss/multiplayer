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

#ifndef IControlCameraH
#define IControlCameraH

#include "ICamera.h"

class IBaseObject;

class SHARE_EI IControlCamera : public ICamera
{

public:

  enum{
    eX          =1<<0,
    eY          =1<<1,
    eZ          =1<<2,
    eRoll       =1<<3,
    eRotateDown =1<<4,
    eRotateRight=1<<5,
  };


  IControlCamera(){}
  virtual ~IControlCamera(){}

  virtual IBaseObject* GetLinkedObject() = 0;
  // если указатель на объект существует, то берем с объекта параметры, которые указаны в маске
  // делаем SetXXX иначе SetXXX, который задан снаружи. Потом деламе AddXXX, который задан снаружи.
  virtual void Link(IBaseObject* pObject, int maskUse ) = 0;
  virtual void Unlink() = 0;

  virtual ICamera* GetCamera() = 0;

  // смещение от eye обзора. Например, от 3 лица вид координаты точка больше 0
  // от 1 лица координаты равны нулю
  // влияет на вращение камеры (задает центр вращения)
  virtual float GetDistObj()             = 0;
  virtual void  SetDistObj(float radius) = 0;
  virtual void  AddDistObj(float dR)     = 0;
};

#endif