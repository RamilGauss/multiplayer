#include "LoaderMap.h"
#include "ManagerObjectDX.h"
#include "LoaderObjectDX.h"

using namespace nsStructDirectX;

TLoaderMap::TLoaderMap()
{

}
//----------------------------------------------------------------------------------
TLoaderMap::~TLoaderMap()
{

}
//----------------------------------------------------------------------------------
// читает карту: координаты, ориентацию и состояние объектов в менеджер объектов
int TLoaderMap::Load(unsigned int id_map, bool flgCleanObject)
{
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
  std::list<TDescObject>::iterator it = listObject.begin();
  std::list<TDescObject>::iterator eit = listObject.end();
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
