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


#include "BaseObject.h"


using namespace nsStruct3D;

TBaseObject::TBaseObject()
{
  ID_model = 0;
  ID_map = 0;
  SetOneMatrix(mWorld);
}
//------------------------------------------------------------------------------------------------
TBaseObject::~TBaseObject()
{

}
//------------------------------------------------------------------------------------------------
void TBaseObject::SetCoord(TCoord3& coord)
{
  mWorld._41 = coord.x;
  mWorld._42 = coord.y;
  mWorld._43 = coord.z;
}
//------------------------------------------------------------------------------------------------
void TBaseObject::SetOrient(TOrient3& orient)
{
  //mWorld. ; // здесь вся инфа по ориентации и координатам объекта
}
//------------------------------------------------------------------------------------------------
void TBaseObject::SetState(std::vector<unsigned char>* state)
{
  mState = *state;
}
//------------------------------------------------------------------------------------------------
void TBaseObject::SetOneMatrix(D3DXMATRIXA16& matrix)
{
  matrix._11 = 1;matrix._12 = 0;matrix._13 = 0;matrix._14 = 0;
  matrix._21 = 0;matrix._22 = 1;matrix._23 = 0;matrix._24 = 0;
  matrix._31 = 0;matrix._32 = 0;matrix._33 = 1;matrix._34 = 0;
  matrix._41 = 0;matrix._42 = 0;matrix._43 = 0;matrix._44 = 1;
}
//------------------------------------------------------------------------------------------------
