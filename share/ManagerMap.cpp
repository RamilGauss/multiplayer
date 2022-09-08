#include "ManagerMap.h"

TManagerMap GlobalManagerMap;

TManagerMap::TManagerMap()
{

}
//-------------------------------------------------------------
TManagerMap::~TManagerMap()
{

}
//-------------------------------------------------------------
//TMap* TManagerMap::GetMap(int flag)
//{
//  TMap* pMap = new TMap;
//  unsigned short id_map = GetIDByFlag(flag);
//  pMap->SetID(id_map);
//  LoadMapFile(pMap,id_map);
//  return pMap;
//}
////-------------------------------------------------------------
//unsigned short TManagerMap::GetIDByFlag(int flag)
//{
//  return 0;// пока так
//}
////-------------------------------------------------------------
//void TManagerMap::LoadMapFile(TMap* pMap,unsigned short id_map)
//{
//  char strNameFile[500];
//  switch(id_map)
//  {
//    case 0:
//      //strcpy(strNameFile,);
//      break;
//    default:;
//  }
//  LoadFile(strNameFile,pMap);
//}
////-------------------------------------------------------------
//void TManagerMap::LoadFile(char* strNameFile,TMap* pMap)
//{
//  // загрузка параметров карты
//}
////-------------------------------------------------------------
