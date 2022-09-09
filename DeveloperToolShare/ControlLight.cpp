/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012, 2013
===========================================================================
                        Common Information
"TornadoEngine" GPL Source Code

This file is part of the "TornadoEngine" GPL Source Code.

"TornadoEngine" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"TornadoEngine" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "TornadoEngine" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "TornadoEngine" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "TornadoEngine" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 

#include "ControlLight.h"
#include "..\BigJack\IGraphicEngine.h"
#include "..\GBaseLib\BL_Debug.h"
#include "..\Share\MakerCamera.h"
#include "..\GameLib\IClientDeveloperTool.h"
#include "..\GameLib\IManagerObjectCommon.h"
#include "Struct3D.h"
#include "..\GameLib\IBaseObjectCommon.h"
#include <valarray>

using namespace nsStruct3D;


#define ASPECT_LIGHT_X 0.01f
#define ASPECT_LIGHT_Y 0.01f


TControlLight::TControlLight()
{
  TMakerCamera maker;
  mICamera = maker.New();
  isDrag = false;
}
//----------------------------------------------------------------------------------
TControlLight::~TControlLight()
{
  Done();
}
//----------------------------------------------------------------------------------
void TControlLight::Done()
{
  TMakerCamera maker;
  maker.Delete(mICamera);
  mICamera = NULL;

  mVecObjLight.clear();
}
//----------------------------------------------------------------------------------
void TControlLight::Init()
{
  pGE  = IClientDeveloperTool::GetSingleton()->GetComponent()->mGraphicEngine;
  pMOC = IClientDeveloperTool::GetSingleton()->GetComponent()->mMOC;
  // загрузка объектов
  int cnt = GetCountAllLight(); 
  for(int i = 0 ; i < cnt ; i++ )
  {
    IBaseObjectCommon* pSphere = pMOC->CreateObject(3);// 
    pGE->AddObject((IBaseObjectGE*)pSphere);
    mVecObjLight.push_back(pSphere);
  }
  Update();
}
//----------------------------------------------------------------------------------
void TControlLight::SetLightEnable(int index, bool v)
{
  BL_ASSERT(pGE);
  pGE->SetLightEnable(index, v);
  mVecObjLight[index]->SetShow(v);
}
//----------------------------------------------------------------------------------
bool TControlLight::GetLightEnable(int index)
{
  BL_ASSERT(pGE);
  return pGE->GetLightEnable(index);
}
//----------------------------------------------------------------------------------
int TControlLight::GetCountAllLight()
{
  BL_ASSERT(pGE);
  return pGE->GetCountAllLight();
}
//----------------------------------------------------------------------------------
const TVector3* TControlLight::GetLightPosition(int index)	
{
  BL_ASSERT(pGE);
  return pGE->GetLightPosition(index);
}
//----------------------------------------------------------------------------------
void TControlLight::SetLightPosition(int index, TVector3* m3)
{
  BL_ASSERT(pGE);
  pGE->SetLightPosition(index, m3);

  SetObjectPos(m3, index);
}
//----------------------------------------------------------------------------------
const TVector3* TControlLight::GetLightColor(int index)
{
  BL_ASSERT(pGE);
  return pGE->GetLightColor(index);
}
//----------------------------------------------------------------------------------
void TControlLight::SetLightColor(int index, TVector3* color)
{
  BL_ASSERT(pGE);
  pGE->SetLightColor(index, color);
}
//----------------------------------------------------------------------------------
void TControlLight::BeginDrag(int x, int y)// экранные координаты мыши
{
  isDrag = true;
  mOldX = x;
  mOldY = y;
}
//----------------------------------------------------------------------------------
void TControlLight::EndDrag()
{
  isDrag = false;
}
//----------------------------------------------------------------------------------
void TControlLight::Drag(int iCurLight,// текущий источник
                         const TMatrix16* view, 
                         int x, int y)// экранные координаты мыши
{
  if(isDrag==false) return;

  TMatrix16 mInv;
  SetMatrixInverse(&mInv,NULL,view);
  TVector3 right = TVector3(mInv._11,mInv._12,mInv._13);
  TVector3 up    = TVector3(mInv._21,mInv._22,mInv._23);

  TVector3 posL = *GetLightPosition(iCurLight);
  TVector3 vecL = posL - mPos;
    
  float lenVecL_old = vecL.lenght();

  float angleV = ASPECT_LIGHT_Y*float(mOldY - y);
  float angleH = ASPECT_LIGHT_X*float(mOldX - x);
  float angle = 0;// 

  // поворот вокруг горизонтальной оси
  TQuaternion axis;
  SetQuaternionRotationAxis(&axis,&right,angleV);// вокруг этой оси будет производится вращение
  //-----------------------------------------
  TQuaternion rot = TQuaternion(vecL.x,vecL.y,vecL.z,0);// этот вектор будет вращаться
  SetQuaternionMultiply(&rot,&rot,&axis);// вращаем
  SetQuaternionToAxisAngle(&rot,&vecL,&angle);// получаем новый вектор и, как побочный продукт, угол

  // поворот вокруг вертикальной оси
  SetQuaternionRotationAxis(&axis,&up,angleH);// вокруг этой оси будет производится вращение
  //-----------------------------------------
  rot = TQuaternion(vecL.x,vecL.y,vecL.z,0);// этот вектор будет вращаться
  SetQuaternionMultiply(&rot,&rot,&axis);// вращаем
  SetQuaternionToAxisAngle(&rot,&vecL,&angle);// получаем новый вектор и, как побочный продукт, угол
  
  float lenVecL_new = vecL.lenght();
  vecL *= lenVecL_old/lenVecL_new;
  float lenVecL = vecL.lenght();

  posL = vecL + mPos;

  mOldX = x;
  mOldY = y;

  SetLightPosition(iCurLight, &posL);
}
//----------------------------------------------------------------------------------
void TControlLight::SetPosition(TVector3* pPos)// центр вращения
{
  mPos = *pPos;
}
//----------------------------------------------------------------------------------
void TControlLight::Update()
{
  int cnt = mVecObjLight.size();
  for(int i = 0 ; i < cnt ; i++)
  {
    const TVector3* pV = GetLightPosition(i);
    SetObjectPos((TVector3*)pV,i);
    mVecObjLight[i]->SetShow(GetLightEnable(i));
  }
}
//----------------------------------------------------------------------------------
void TControlLight::SetObjectPos(TVector3* m3, int i)
{
  IBaseObjectCommon* pObj = mVecObjLight[i];
  TMatrix16 world = *pObj->GetWorld();
  world._41 = m3->x;
  world._42 = m3->y;
  world._43 = m3->z;
  pObj->SetWorld(&world);
}
//----------------------------------------------------------------------------------
void TControlLight::SetViewObject(bool v)
{
  if(v==false)
  {
    int cnt = mVecObjLight.size();
    for(int i = 0 ; i < cnt ; i++)
      mVecObjLight[i]->SetShow(false);
  }
  else
    Update();
}
//----------------------------------------------------------------------------------