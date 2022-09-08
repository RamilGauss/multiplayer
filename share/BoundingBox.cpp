#include "BoundingBox.h"


TBoundingBox::TBoundingBox()
{
  mMin.x = 0;
  mMin.y = 0;
  mMin.z = 0;

  mMax.x = 0;
  mMax.y = 0;
  mMax.z = 0;
}
//-------------------------------------------------------------------------------------
TBoundingBox::~TBoundingBox()
{

}
//-------------------------------------------------------------------------------------
void TBoundingBox::SetBound(TPoint3& min,TPoint3& max)
{
  mMin = min;
  mMax = max;
}
//-------------------------------------------------------------------------------------
void TBoundingBox::AddBound(TPoint3& min,TPoint3& max)
{
  if(min.x<mMin.x) mMin.x = min.x;
  if(min.y<mMin.y) mMin.y = min.y;
  if(min.z<mMin.z) mMin.z = min.z;

  if(max.x>mMax.x) mMax.x = max.x;
  if(max.y>mMax.y) mMax.y = max.y;
  if(max.z>mMax.z) mMax.z = max.z;
}
//-------------------------------------------------------------------------------------

