#include "ObjectDX.h"
#include <d3d9.h>

using namespace nsStruct3D;

TObjectDX::TObjectDX()
{
  SetOneMatrix(mWorld);
  
  mArrMatrixSubset = NULL;
}
//------------------------------------------------------------------------------------------------
TObjectDX::~TObjectDX()
{
  Done();
}
//------------------------------------------------------------------------------------------------
void TObjectDX::Draw(D3DXMATRIXA16* mView,D3DXMATRIXA16* mProj)
{                    
  mModel->Draw(mState,             //                           (От ObjectDX)
               mArrMatrixSubset,//кол-во совпадает с cSubset (От ObjectDX)
               &mWorld,// где и как расположен объект         (От ObjectDX)
               mView, // расположение и ориентация камеры    (от ManagerDirectX)
               mProj);
}
//------------------------------------------------------------------------------------------------
void TObjectDX::SetCoord(TCoord3 coord)
{
  mWorld._41 = coord.x;
  mWorld._42 = coord.y;
  mWorld._43 = coord.z;
}
//------------------------------------------------------------------------------------------------
void TObjectDX::SetOrient(TOrient3 orient)
{
  //mWorld. ; // здесь вся инфа по ориентации и координатам объекта
}
//------------------------------------------------------------------------------------------------
void TObjectDX::SetState(unsigned int state)
{
  mState = state;
}
//------------------------------------------------------------------------------------------------
void TObjectDX::SetModel(TModelDX* pModel)
{
  mModel = pModel;
  int cnt = mModel->GetCntEffect();
  mArrMatrixSubset = new D3DXMATRIXA16[cnt];
  for(int i = 0 ; i < cnt ; i++ )
    SetOneMatrix(mArrMatrixSubset[i]);
}
//------------------------------------------------------------------------------------------------
void TObjectDX::Done()
{
  delete[] mArrMatrixSubset;
  mArrMatrixSubset = NULL;
}
//------------------------------------------------------------------------------------------------
void TObjectDX::SetOneMatrix(D3DXMATRIXA16& matrix)
{
  matrix._11 = 1;matrix._12 = 0;matrix._13 = 0;matrix._14 = 0;
  matrix._21 = 0;matrix._22 = 1;matrix._23 = 0;matrix._24 = 0;
  matrix._31 = 0;matrix._32 = 0;matrix._33 = 1;matrix._34 = 0;
  matrix._41 = 0;matrix._42 = 0;matrix._43 = 0;matrix._44 = 1;
}
//------------------------------------------------------------------------------------------------
