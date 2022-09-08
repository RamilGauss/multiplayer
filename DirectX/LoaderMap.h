#ifndef LoaderMapH
#define LoaderMapH

#include "Struct3D.h"
#include <list>
class TManagerObjectDX;
class TManagerModel;

class TLoaderMap
{

  struct TDescObject
  {
    unsigned short       id;
    unsigned int         state;
    nsStruct3D::TCoord3  coord;
    nsStruct3D::TOrient3 orient;
  };

  std::list<TDescObject*> mListObject;

  TManagerObjectDX* mMO; 
  TManagerModel* mMM;

public:
  TLoaderMap(TManagerObjectDX* pMO, TManagerModel* pMM);
  ~TLoaderMap();

  enum{
    eSuccess=0,
  };

  // ������ �����: ����������, ���������� � ��������� �������� � �������� ��������
  int Load(unsigned int id_map, bool flgCleanObject = true);

protected:
  void LoadMapObject();
  void LoadObjectDX();


};

#endif