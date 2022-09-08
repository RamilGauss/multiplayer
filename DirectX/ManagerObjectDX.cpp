#include "ManagerObjectDX.h"


TManagerObjectDX GlobalManagerObjectDX;


TManagerObjectDX::TManagerObjectDX()
{

}
//--------------------------------------------------------------------------------------------
TManagerObjectDX::~TManagerObjectDX()
{

}
//--------------------------------------------------------------------------------------------
void TManagerObjectDX::Clean()
{
  mArrObjectDX.Clear();
}
//--------------------------------------------------------------------------------------------
void TManagerObjectDX::Add(TObjectDX* pObj)
{
  mArrObjectDX.Add(pObj);
}
//--------------------------------------------------------------------------------------------