#include "ObjectPrediction.h"
#include <memory.h>



TObjectPrediction::TObjectPrediction(unsigned char type)
{
  mType = type;
  pUserData = NULL;
  mUserDataSize = 0;
  mArrDef = NULL;
  mCntDef = 0;
}
//--------------------------------------------------------------------------
TObjectPrediction::~TObjectPrediction()
{
  CleanUserData();
  CleanArrDef();
}
//--------------------------------------------------------------------------
unsigned short TObjectPrediction::GetID()
{
  return mID;
}
//--------------------------------------------------------------------------
void TObjectPrediction::SetID(unsigned short id)
{
  mID = id;
}
//--------------------------------------------------------------------------
unsigned int TObjectPrediction::GetMaskState()
{
  return mMaskState;
}
//--------------------------------------------------------------------------
void TObjectPrediction::SetUserData(char* pData, int size)
{
  CleanUserData();
  mUserDataSize = size;
  pUserData = new char[size];
  memcpy(pUserData,pData,mUserDataSize);
}
//--------------------------------------------------------------------------
void TObjectPrediction::GetUserData(char* pData, int& size)
{
  memcpy(pData,pUserData,mUserDataSize);
}
//--------------------------------------------------------------------------
void TObjectPrediction::CleanUserData()
{
  delete[] pUserData;
  pUserData = NULL;
  mUserDataSize = 0;
}
//--------------------------------------------------------------------------
void TObjectPrediction::CleanArrDef()
{
  delete []mArrDef;
  mArrDef = NULL;
  mCntDef = 0;
}
//--------------------------------------------------------------------------