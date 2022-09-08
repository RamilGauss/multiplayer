#include "LoaderMap.h"
#include "ManagerObjectDX.h"
#include "LoaderObjectDX.h"
#include "ManagerModel.h"
#include "HiTimer.h"

using namespace nsStructDirectX;

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
int TLoaderMap::Load(unsigned int id_map, bool flgCleanObject)
{
  ht_msleep(5000);//### симуляция

  LoadMapObject();

  if(flgCleanObject)
    GlobalManagerObjectDX.Clean();

  LoadObjectDX();

  return eSuccess;
}
//----------------------------------------------------------------------------------
void TLoaderMap::LoadObjectDX()
{
  TLoaderObjectDX loaderObj;
  // пробежка по объектам и их загрузка 
  std::list<TDescObject>::iterator it = mListObject.begin();
  std::list<TDescObject>::iterator eit = mListObject.end();
  while(it!=eit)
  {
    TObjectDX* pObj = loaderObj.Load(it->id);//
    if(pObj)
    {
      //pObj->
    }
  }
}
//----------------------------------------------------------------------------------
void TLoaderMap::LoadMapObject()
{

}
//----------------------------------------------------------------------------------
