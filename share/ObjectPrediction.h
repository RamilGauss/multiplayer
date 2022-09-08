#ifndef ObjectPredictionH
#define ObjectPredictionH

#include "BoundingBox.h"

class TObjectPrediction
{
  unsigned short mID;
  unsigned int mMaskState;// �������� ���� ������������� ������� - ���������� ��������� ���� ���������� mMaskState

  TBoundingBox mBB;// ���������� ��������, ������ ������

  unsigned short mHP;// ��� �� ��������� ���� ����� ������� ��� ��������� � ������ ��� ��������


public:
  
  TObjectPrediction();
  ~TObjectPrediction();

  unsigned short GetID();
  unsigned int GetMaskState();

};
#endif