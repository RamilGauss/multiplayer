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

  // ������ �������� ����������� �����
  // �����������, ���������(����������� �����������, �����, �������), ���������� �������

  void Clean();
  void Add(TObjectDX* pObj);

  int GetCnt(){return mArrObjectDX.Count();};
  TObjectDX* Get(int i){return (TObjectDX*)mArrObjectDX.Get(i);};

protected:

};

extern TManagerObjectDX GlobalManagerObjectDX;

#endif