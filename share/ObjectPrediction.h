#ifndef ObjectPredictionH
#define ObjectPredictionH

#include "BoundingBox.h"
#include "Struct3D.h"

class TObjectPrediction
{
public:
  // тип объекта, важно для Prediction
  // 
  typedef 
    enum{
       eTank    = 0,
       eBullet  = 1,
       eObject  = 2,
       eTree    = 3,
       eTerrain = 4,
       eBush    = 5,
  } eTypeObject;
protected:

  char* pUserData;    // в клиенте хранит TObjectDX*, в сервере хранит id
  int   mUserDataSize;

  unsigned short mID;// тип
  unsigned char mType;// см. eTypeObject

public:
  //=====================================
  unsigned int mMaskState;// конечная цель существования объекта - определить состояние этой переменной mMaskState
  // 1 - нормальное, 0 - разрушенное
  
  nsStruct3D::TCoord3  mCoord; // конечная цель существования объекта
  nsStruct3D::TOrient3 mOrient;// 
  //=====================================

  TBoundingBox mBBCommon;// определить коллизии, грубая оценка

  struct TDef
  {
    unsigned short HP;// что бы разрушить надо иметь энергию при вхождении в объект при коллизии
    TBoundingBox   BB;// более точное определение коллизии
    unsigned int   mask;//
  };

  TDef* mArrDef;
  int   mCntDef;


public:
  
  TObjectPrediction(unsigned char type);
  ~TObjectPrediction();

  unsigned short GetID();
  void SetID(unsigned short id);
  unsigned char GetType(){return mType;}

  unsigned int GetMaskState();

  void SetUserData(char* pData, int size);
  void GetUserData(char* pData, int& size);

protected:
  void CleanUserData();
  void CleanArrDef();

};
#endif