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


#include "LoaderMap.h"
#include "ManagerObjectDX.h"
#include "ManagerModel.h"
#include "HiTimer.h"
#include "ObjectDX.h"
#include "LoggerDX.h"
#include "BL_Debug.h"

using namespace nsStruct3D;

TLoaderMap::TLoaderMap(TManagerObjectDX* pMO, TManagerModelDX* pMM)
{
  mMO = pMO;
  mMM = pMM;
}
//----------------------------------------------------------------------------------
TLoaderMap::~TLoaderMap()
{

}
//----------------------------------------------------------------------------------
// читает карту: координаты, ориентацию и состояние объектов в менеджер объектов
int TLoaderMap::LoadMap(unsigned int id_map, bool flgCleanObject)
{
  mListObject.clear();
                                            ht_msleep(5000);//### симуляция
  LoadMapObject();// загрузить список объектов карты

  if(flgCleanObject)
    mMO->Clean();

  LoadObjectDX();// загрузить объекты

  return eSuccess;
}
//----------------------------------------------------------------------------------
TObjectDX* TLoaderMap::LoadObjectDX(TDescObject * pDescObjDX, bool flgCleanObject)
{
  if(flgCleanObject)
    mMO->Clean();
  
  mListObject.clear();

  TDescObject* pDesc = new TDescObject();
  *pDesc = *pDescObjDX;
  mListObject.push_back(pDesc);

  return LoadObjectDX();
}
//----------------------------------------------------------------------------------
TObjectDX* TLoaderMap::LoadObjectDX()
{
  // пробежка по объектам и их загрузка 
  std::list<TDescObject*>::iterator it = mListObject.begin();
  std::list<TDescObject*>::iterator eit = mListObject.end();
  while(it!=eit)
  {
    TDescObject* pDesc = *it;
    TModelDX* pModel = mMM->Find(pDesc->id);
    if(pModel)
    {
      // идентификатор на карте
      TObjectDX* pObjectDX = new TObjectDX();
      pObjectDX->SetModel(pModel);
      pObjectDX->SetCoord(pDesc->coord);
      pObjectDX->SetOrient(pDesc->orient);
      pObjectDX->SetState(pDesc->state);
      mMO->Add(pObjectDX);
      return pObjectDX;
    }
    else
    {
      GlobalLoggerDX.WriteF_time("Загрузка карты: загрузка объекта DX: не удалось привязать модель к объекту.\n");
      BL_FIX_BUG();
    }
    it++;
  }
  return NULL;
}
//----------------------------------------------------------------------------------
void TLoaderMap::LoadMapObject()//###
{
  
}
//----------------------------------------------------------------------------------
