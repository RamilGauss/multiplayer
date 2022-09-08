/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
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
the "Tanks" Source Code.  If not, please request a copy in writing from id Software at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 

#ifndef BaseObjectH
#define BaseObjectH


#include "TObject.h"
#include "Struct3D.h"
#include <windows.h>
#include <d3d9types.h>
#include <d3dx10math.h>


class TBaseObject : public TObject
{
  // свойства, характерные для физики и графики
  // ориентация, координаты, состояние

public:
  TBaseObject();
  virtual ~TBaseObject();

  virtual void SetCoord(nsStruct3D::TCoord3& coord);
  virtual void SetOrient(nsStruct3D::TOrient3& orient);
  virtual void SetState(unsigned int state);

protected:
  unsigned int ID_map;// идентификатор на карте
  unsigned int ID_model;// идентификатор модели

  unsigned int mState;

  D3DXMATRIXA16 mWorld; // здесь вся инфа по ориентации и координатам объекта

};


#endif

