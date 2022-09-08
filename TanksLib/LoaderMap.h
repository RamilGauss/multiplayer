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


#ifndef LoaderMapH
#define LoaderMapH

#include "Struct3D.h"
#include <list>
class TManagerObjectDX;
class TManagerModel;
class TObjectDX;


template < class TMakerBehavior >
class TLoaderMap
{
public:
  struct TDescObject
  {
    unsigned short       id;
    unsigned int         state;
    nsStruct3D::TCoord3  coord;
    nsStruct3D::TOrient3 orient;
  };
protected:
  std::list<TDescObject*> mListObject;

  TManagerObjectDX* mMO; 
  TManagerModel* mMM;

public:
  TLoaderMap(TManagerObjectDX* pMO, TManagerModel* pMM);
  ~TLoaderMap();

  enum{
    eSuccess=0,
  };

  // читает карту: координаты, ориентацию и состояние объектов в менеджер объектов
  int LoadMap(unsigned int id_map, bool flgCleanObject = true);

  TObjectDX* LoadObjectDX(TDescObject * pDescObjDX, bool clean_MO = false);

protected:
  void LoadMapObject();
  TObjectDX* LoadObjectDX();


};

#endif