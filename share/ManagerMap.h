#ifndef ManagerMapH
#define ManagerMapH

#include "Map.h"


// инфо по карте, только для отладки
// P.S. вообще этот класс в архитектуру не вписывается, удалить если что
class TManagerMap
{

public:
  TManagerMap();
  ~TManagerMap();
  
  //TMap* GetMap(int flag);

protected:
  //unsigned short GetIDByFlag(int flag);
  //void LoadMapFile(TMap* pMap,unsigned short id_map);
  //void LoadFile(char* strNameFile,TMap* pMap);

};

extern TManagerMap GlobalManagerMap;

#endif
