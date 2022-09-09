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

#include "ControlCamera.h"
#include "BL_Debug.h"
#include "IBaseObjectCommon.h"
#include "BaseObject.h"

using namespace std;


TControlCamera* pControlCamera = NULL;
void ObjectEvent(void* p, int s)
{
  if(pControlCamera)
  {
    pControlCamera->Notify(p,s);
  }
}
//------------------------------------------------------------------------------------------------
TControlCamera::TControlCamera()
{
  pControlCamera = this;
  mMode = eFree;
}
//------------------------------------------------------------------------------------------------
TControlCamera::~TControlCamera()
{
  pControlCamera = NULL;
  Done();
}
//------------------------------------------------------------------------------------------------
void TControlCamera::SetListObject(vector<IBaseObjectCommon*>* vec)
{
  mVecObject = *vec;
  ZeroCurIndex();
}
//------------------------------------------------------------------------------------------------
void TControlCamera::SetView(D3DXMATRIXA16* view)
{
  mCamera.SetView(view);
}
//------------------------------------------------------------------------------------------------
void TControlCamera::SetProj(D3DXMATRIXA16* proj)
{
  mCamera.SetProj(proj);
}
//------------------------------------------------------------------------------------------------
const D3DXMATRIXA16* TControlCamera::GetView()const
{
  return mCamera.GetView();
}
//------------------------------------------------------------------------------------------------
const D3DXMATRIXA16* TControlCamera::GetProj()const
{
  return mCamera.GetProj();
}
//------------------------------------------------------------------------------------------------
void TControlCamera::Done()
{
  if(IsModeBindObject())
    UnregisterOnCurObjectEvent();
  mVecObject.clear();
}
//------------------------------------------------------------------------------------------------
void TControlCamera::SetFirstObject()
{
  BL_ASSERT(mVecObject.size());
  if(IsModeBindObject()==false) return;
  //---------------------------------------------------
  UnregisterOnCurObjectEvent();
  ZeroCurIndex();
  RegisterOnCurObjectEvent();
}
//------------------------------------------------------------------------------------------------
void TControlCamera::SetLastObject()
{
  BL_ASSERT(mVecObject.size());
  if(IsModeBindObject()==false) return;
  //---------------------------------------------------
  UnregisterOnCurObjectEvent();
  mCurIndex = mVecObject.size()-1;
  RegisterOnCurObjectEvent();
}
//------------------------------------------------------------------------------------------------
void TControlCamera::SetBelowObject()
{
  BL_ASSERT(mVecObject.size());
  if(IsModeBindObject()==false) return;
  //---------------------------------------------------
  UnregisterOnCurObjectEvent();
  mCurIndex--;
  if(mCurIndex<0)
    mCurIndex = mVecObject.size()-1;
  RegisterOnCurObjectEvent();
}
//------------------------------------------------------------------------------------------------
void TControlCamera::SetNextObject()
{
  BL_ASSERT(mVecObject.size());
  if(IsModeBindObject()==false) return;
  //---------------------------------------------------
  UnregisterOnCurObjectEvent();
  mCurIndex++;
  if(mCurIndex==mVecObject.size())
    ZeroCurIndex();
  RegisterOnCurObjectEvent();
}
//------------------------------------------------------------------------------------------------
void TControlCamera::SetMode(int type)
{
  BL_ASSERT(mMode!=type);
  switch(mMode)
  {
    case eFree:
    case eBindCoord:
      break;
    case eBindObjectCoord:
    case eBindObjectFull:
      UnregisterOnCurObjectEvent();// отвязаться от текущего объекта
      break;
  }
  //--------------------------------------------------
  mMode = type;
  switch(mMode)
  {
    case eFree:
      break;
    case eBindObjectCoord:
    case eBindObjectFull:
      RegisterOnCurObjectEvent();// привязка к объекту
      break;
    case eBindCoord:
      break;
    default:BL_FIX_BUG();
  }
}
//------------------------------------------------------------------------------------------------
void TControlCamera::Notify(void* p, int s)
{
  int event = *((int*)p);
  switch(event)
  {
  case IBaseObject::eWorld:
    {
      IBaseObjectCommon* pBOC = mVecObject.at(mCurIndex);  
      const D3DXMATRIXA16* pMat = pBOC->GetWorld();// координаты изменились, обновить
      // что-то там сделать
      break;
    }
    default:;
  }
}
//------------------------------------------------------------------------------------------------
void TControlCamera::RegisterOnCurObjectEvent()
{
  IBaseObjectCommon* pBOC = mVecObject.at(mCurIndex);  
  pBOC->RegisterOnEvent(ObjectEvent);
}
//------------------------------------------------------------------------------------------------
void TControlCamera::UnregisterOnCurObjectEvent()
{
  IBaseObjectCommon* pBOC = mVecObject.at(mCurIndex);  
  pBOC->UnregisterOnEvent(ObjectEvent);
}
//------------------------------------------------------------------------------------------------
void TControlCamera::SetRadius(float radius)
{

}
//------------------------------------------------------------------------------------------------  
void TControlCamera::Set(int type, float val)
{

}
//------------------------------------------------------------------------------------------------  
void TControlCamera::Add(int type, float val)
{

}
//------------------------------------------------------------------------------------------------  
void TControlCamera::SetCoordBind(float* coord)
{

}
//------------------------------------------------------------------------------------------------  