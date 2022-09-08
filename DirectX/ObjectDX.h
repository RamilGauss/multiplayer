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


#ifndef ObjectDXH
#define ObjectDXH

#include "ModelDX.h"
#include "TObject.h"

class TManagerObjectDX;

class TObjectDX : public TObject
{

public:
  TObjectDX();
  ~TObjectDX();

  void SetModel(TModelDX* pModel);

  void Draw(D3DXMATRIXA16* mView,D3DXMATRIXA16* mProj);

  void SetCoord(nsStruct3D::TCoord3 coord);
  void SetOrient(nsStruct3D::TOrient3 orient);
  void SetState(unsigned int state);


protected:
  void Done();
  void SetOneMatrix(D3DXMATRIXA16& matrix);

  friend class TManagerObjectDX;

  TModelDX* mModel; 
  unsigned int ID_map;// идентификатор на карте

  unsigned int mState;

  D3DXMATRIXA16 mWorld; // здесь вся инфа по ориентации и координатам объекта
  D3DXMATRIXA16* mArrMatrixSubset;// кол-во см. в mModel

};
//-----------------------------------------------------------------



#endif