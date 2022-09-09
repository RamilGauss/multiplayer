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


#ifndef ControlLightH
#define ControlLightH

#include "Struct3D.h"
#include <vector>

class ICamera;
class IGraphicEngine;
class IManagerObjectCommon;
class IBaseObjectCommon;

class TControlLight
{

  IGraphicEngine* pGE;
  IManagerObjectCommon* pMOC;
  ICamera* mICamera;
  
  nsStruct3D::TVector3 mPos;// центр вращения

  bool isDrag;
  int  mOldX;
  int  mOldY;

  typedef std::vector<IBaseObjectCommon*> TVectorPtr;
  TVectorPtr mVecObjLight;// указатели этих объектов содержатся в mMOC

public:

  TControlLight();
  ~TControlLight();

  void Init();

  void SetPosition(nsStruct3D::TVector3* pPos);// центр вращения

  // манипуляции с мышью
  void BeginDrag(int x, int y);// экранные координаты мыши
  void EndDrag();
  // при движении мыши вызвать
  void Drag(int iCurLight,// текущий источник
            const nsStruct3D::TMatrix16* view,// матрица ориентации камеры
            int x, int y);// экранные координаты мыши


  // дублируются здесь только для получения события
  void SetLightEnable(int index, bool v);
  bool GetLightEnable(int index);
  int  GetCountAllLight();

	const nsStruct3D::TVector3* GetLightPosition(int index);	
	void SetLightPosition(int index,nsStruct3D::TVector3* m3);
	const nsStruct3D::TVector3* GetLightColor(int index);
	void SetLightColor(int index, nsStruct3D::TVector3* color);

  void SetViewObject(bool v);

protected:

  void Done();
  void Update();


  void SetObjectPos(nsStruct3D::TVector3* m3, int i);
};


#endif

