#ifndef ObjectPredictionH
#define ObjectPredictionH

#include "BoundingBox.h"
#include "Struct3D.h"

class TObjectPrediction
{
public:
  // ��� �������, ����� ��� Prediction
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

  char* pUserData;    // � ������� ������ TObjectDX*, � ������� ������ id
  int   mUserDataSize;

  unsigned short mID;// ���
  unsigned char mType;// ��. eTypeObject

public:
  //=====================================
  unsigned int mMaskState;// �������� ���� ������������� ������� - ���������� ��������� ���� ���������� mMaskState
  // 1 - ����������, 0 - �����������
  
  nsStruct3D::TCoord3  mCoord; // �������� ���� ������������� �������
  nsStruct3D::TOrient3 mOrient;// 
  //=====================================

  TBoundingBox mBBCommon;// ���������� ��������, ������ ������

  struct TDef
  {
    unsigned short HP;// ��� �� ��������� ���� ����� ������� ��� ��������� � ������ ��� ��������
    TBoundingBox   BB;// ����� ������ ����������� ��������
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