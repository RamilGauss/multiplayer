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
  mModel->Draw(mState,             //                           (�� ObjectDX)
               mArrMatrixSubset,//���-�� ��������� � cSubset (�� ObjectDX)
               &mWorld,// ��� � ��� ���������� ������         (�� ObjectDX)
               mView, // ������������ � ���������� ������    (�� ManagerDirectX)
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