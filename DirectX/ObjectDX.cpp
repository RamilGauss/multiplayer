#include "ObjectDX.h"
#include <d3d9.h>

using namespace nsStruct3D;

TObjectDX::TObjectDX()
{

}
//------------------------------------------------------------------------------------------------
TObjectDX::~TObjectDX()
{

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
  
}
//------------------------------------------------------------------------------------------------
void TObjectDX::SetOrient(TOrient3 orient)
{

}
//------------------------------------------------------------------------------------------------
void TObjectDX::SetState(unsigned int state)
{

}
//------------------------------------------------------------------------------------------------