#ifndef LoaderMapH
#define LoaderMapH

#include "Struct3D.h"
#include <list>
class TManagerObjectDX;
class TManagerModel;

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

  bool LoadObjectDX(TDescObject * pDescObjDX, bool clean_MO = false);

protected:
  void LoadMapObject();
  void LoadObjectDX();


};

#endif