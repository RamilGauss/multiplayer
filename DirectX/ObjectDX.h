#ifndef ObjectDXH
#define ObjectDXH

#include "ModelDX.h"

class TManagerObjectDX;

class TObjectDX
{

public:
  TObjectDX();
  ~TObjectDX();

  void SetModel(TModelDX* pModel){mModel=pModel;};

  void Draw(D3DXMATRIXA16* mView,D3DXMATRIXA16* mProj);

  void SetCoord(nsStruct3D::TCoord3 coord);
  void SetOrient(nsStruct3D::TOrient3 orient);
  void SetState(unsigned int state);


protected:
  friend class TManagerObjectDX;

  TModelDX* mModel; 
  unsigned int ID_map;// идентификатор на карте

  unsigned int mState;

  D3DXMATRIXA16 mWorld; // здесь вся инфа по ориентации и координатам объекта
  D3DXMATRIXA16* mArrMatrixSubset;// кол-во см. в mModel

};
//-----------------------------------------------------------------



#endif