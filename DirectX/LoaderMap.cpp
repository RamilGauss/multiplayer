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
// ������ �����: ����������, ���������� � ��������� �������� � �������� ��������
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
  // �������� �� �������� � �� �������� 
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
