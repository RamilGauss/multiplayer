/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef BaseObjectPredictionH
#define BaseObjectPredictionH

#include "BoundingBox.h"
#include "IBaseObject.h"

class IBaseObjectPrediction : virtual public IBaseObject
{
public:
  // ��� �������, ����� ��� Prediction
  // 
 /* typedef 
    enum{
       eTank    = 0,
       eBullet  = 1,
       eObject  = 2,
       eTree    = 3,
       eTerrain = 4,
       eBush    = 5,
  } eTypeObject;*/
protected:

  //char* pUserData;    // � ������� ������ TObjectDX*, � ������� ������ id
  //int   mUserDataSize;

  //unsigned short mID;// ���
  //unsigned char mType;// ��. eTypeObject

public:
  //=====================================
  //unsigned int mMaskState;// �������� ���� ������������� ������� - ���������� ��������� ���� ���������� mMaskState
  // 1 - ����������, 0 - �����������
  
  //nsStruct3D::TCoord3  mCoord; // �������� ���� ������������� �������
  //nsStruct3D::TOrient3 mOrient;// 
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
  
  IBaseObjectPrediction();
  virtual ~IBaseObjectPrediction();

  //unsigned short GetID();
  //void SetID(unsigned short id);
  //unsigned char GetType(){return mType;}

  //unsigned int GetMaskState();

  //void SetUserData(char* pData, int size);
  //void GetUserData(char* pData, int& size);

protected:
  //void CleanUserData();
  //void CleanArrDef();
  virtual void SetupVectorNamePart();

};
#endif