#ifndef ManagerMapH
#define ManagerMapH

#include "Map.h"


// ���� �� �����, ������ ��� �������
// P.S. ������ ���� ����� � ����������� �� �����������, ������� ���� ���
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
