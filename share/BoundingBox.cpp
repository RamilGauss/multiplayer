/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012
===========================================================================
                        Common Information
"Tanks" GPL Source Code

This file is part of the "Tanks" GPL Source Code.

"Tanks" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"Tanks" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "Tanks" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "Tanks" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "Tanks" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#include "BoundingBox.h"
#include <stdlib.h>
#include <stdio.h>


TBoundingBox::TBoundingBox()
{
  mMin.x = 0;
  mMin.y = 0;
  mMin.z = 0;

  mMax.x = -1;
  mMax.y = -1;
  mMax.z = -1;
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
bool TBoundingBox::IsCorrect()
{
  if((mMax.x>=mMin.x)&&
     (mMax.y>=mMin.y)&&
     (mMax.z>=mMin.z))
     return true;
  return false;
}
//-------------------------------------------------------------------------------------
bool TBoundingBox::Contains(TPoint3& point)
{
  if((mMin.x<point.x&&point.x<mMax.x)&&
     (mMin.y<point.y&&point.y<mMax.y)&&
     (mMin.z<point.z&&point.z<mMax.z))
    return true;
  return false;
}
//-------------------------------------------------------------------------------------
bool TBoundingBox::Collision(TBoundingBox& bb)
{
  // проверка по перекрытию плоскостей X:
  if(((mMin.x > bb.Min().x)&&(mMin.x > bb.Max().x))||
     ((mMax.x < bb.Min().x)&&(mMax.x < bb.Max().x)))
    return false;
  if(((mMin.y > bb.Min().y)&&(mMin.y > bb.Max().y))||
     ((mMax.y < bb.Min().y)&&(mMax.y < bb.Max().y)))
    return false;
  if(((mMin.z > bb.Min().z)&&(mMin.z > bb.Max().z))||
     ((mMax.z < bb.Min().z)&&(mMax.z < bb.Max().z)))
    return false;
  return true;
}
//-------------------------------------------------------------------------------------
float TBoundingBox::VolumeCollision(TBoundingBox& bb)
{
  float collision = 0;
  //// проверка по перекрытию плоскостей X:
  if(Collision(bb)==false)  
    return 0;
  float crossX, crossY, crossZ;
  float minLenX = __min(mMax.x-mMin.x,bb.Max().x-bb.Min().x);
  float minLenY = __min(mMax.y-mMin.y,bb.Max().y-bb.Min().y);
  float minLenZ = __min(mMax.z-mMin.z,bb.Max().z-bb.Min().z);

  float crossX0 = mMax.x - bb.Min().x;
  float crossX1 = bb.Max().x - mMin.x;

  float crossY0 = mMax.y - bb.Min().y;
  float crossY1 = bb.Max().y - mMin.y;

  float crossZ0 = mMax.z - bb.Min().z;
  float crossZ1 = bb.Max().z - mMin.z;

  crossX = __min(minLenX,__min(crossX0,crossX1));
  crossY = __min(minLenY,__min(crossY0,crossY1));
  crossZ = __min(minLenZ,__min(crossZ0,crossZ1));

  collision = crossX*crossY*crossZ;

  return collision;
}
//-------------------------------------------------------------------------------------
