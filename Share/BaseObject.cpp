#include "BaseObject.h"


using namespace nsStruct3D;

TBaseObject::TBaseObject()
{

}
//------------------------------------------------------------------------------------------------
TBaseObject::~TBaseObject()
{

}
//------------------------------------------------------------------------------------------------
void TBaseObject::SetCoord(TCoord3& coord)
{
  mWorld._41 = coord.x;
  mWorld._42 = coord.y;
  mWorld._43 = coord.z;
}
//------------------------------------------------------------------------------------------------
void TBaseObject::SetOrient(TOrient3& orient)
{
  //mWorld. ; // ����� ��� ���� �� ���������� � ����������� �������
}
//------------------------------------------------------------------------------------------------
void TBaseObject::SetState(unsigned int state)
{
  mState = state;
}
//------------------------------------------------------------------------------------------------
