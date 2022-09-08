#ifndef LoaderMapH
#define LoaderMapH

#include "StructDirectX.h"
#include <list>
class TManagerObjectDX;
class TManagerModel;

class TLoaderMap
{

  struct TDescObject
  {
    unsigned int id;
    nsStructDirectX::TStateObjectDX state;
    nsStructDirectX::TCoord3 coord;
    nsStructDirectX::TOrient3 Orient;
  };

  std::list<TDescObject> mListObject;

  TManagerObjectDX* mMO; 
  TManagerModel* mMM;

public:
  TLoaderMap(TManagerObjectDX* pMO, TManagerModel* pMM);
  ~TLoaderMap();

  enum{
    eSuccess=0,
  };

  // читает карту: координаты, ориентацию и состояние объектов в менеджер объектов
  int Load(unsigned int id_map, bool flgCleanObject = true);

protected:
  void LoadMapObject();
  void LoadObjectDX();


};

#endif