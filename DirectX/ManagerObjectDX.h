#ifndef ManagerObjectDXH
#define ManagerObjectDXH

#include "ObjectDX.h"
#include "hArray.h"

class TManagerObjectDX
{

  TArrayObject mArrObjectDX;

public:
  TManagerObjectDX();
  ~TManagerObjectDX();

  // список объектов загруженной карты
  // статические, серверные(разрушаемые неподвижные, танки, снаряды), клиентские объекты

  void Clean();
  void Add(TObjectDX* pObj);

  int GetCnt(){return mArrObjectDX.Count();};
  TObjectDX* Get(int i){return (TObjectDX*)mArrObjectDX.Get(i);};

protected:

};

extern TManagerObjectDX GlobalManagerObjectDX;

#endif