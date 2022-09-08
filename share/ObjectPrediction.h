#ifndef ObjectPredictionH
#define ObjectPredictionH

#include "BoundingBox.h"

class TObjectPrediction
{
  unsigned short mID;
  unsigned int mMaskState;// конечная цель существования объекта - определить состояние этой переменной mMaskState

  TBoundingBox mBB;// определить коллизии, грубая оценка

  unsigned short mHP;// что бы разрушить надо иметь энергию при вхождении в объект при коллизии


public:
  
  TObjectPrediction();
  ~TObjectPrediction();

  unsigned short GetID();
  unsigned int GetMaskState();

};
#endif