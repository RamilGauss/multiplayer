#include "LoaderMap.h"
#include "ManagerObjectDX.h"
#include "ManagerModel.h"
#include "HiTimer.h"
#include "ObjectDX.h"
#include "LoggerDX.h"
#include "BL_Debug.h"

using namespace nsStruct3D;

TLoaderMap::TLoaderMap(TManagerObjectDX* pMO, TManagerModel* pMM)
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
  LoadMapObject();

  if(flgCleanObject)
    mMO->Clean();

  LoadObjectDX();

  return eSuccess;
}
//----------------------------------------------------------------------------------
bool TLoaderMap::LoadObjectDX(TDescObject * pDescObjDX, bool flgCleanObject)
{
  if(flgCleanObject)
    mMO->Clean();
  
  mListObject.clear();

  TDescObject* pDesc = new TDescObject();
  *pDesc = *pDescObjDX;
  mListObject.push_back(pDesc);

  LoadObjectDX();

  return true;
}
//----------------------------------------------------------------------------------
void TLoaderMap::LoadObjectDX()
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
    }
    else
    {
      GlobalLoggerDX.WriteF_time("Загрузка карты: загрузка объекта DX: не удалось привязать модель к объекту.\n");
      BL_FIX_BUG();
    }
    it++;
  }
}
//----------------------------------------------------------------------------------
void TLoaderMap::LoadMapObject()//###
{
  
}
//----------------------------------------------------------------------------------
