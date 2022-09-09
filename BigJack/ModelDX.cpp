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
the "Tanks" Source Code.  If not, please request a copy in writing from id Software at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 

#include "ModelDX.h"
#include "SDKmisc.h"
#include "LoaderModelDX.h"
#include "Logger.h"
#include "LoaderModelDX.h"
#include "BL_Debug.h"
#include "MakerLoaderModelDX.h"
#include <stdlib.h>
#include <d3dx10math.h>





using namespace nsStruct3D;
using namespace std;

TModelDX::TModelDX()
{
  mID = -1;
  m_pd3dDevice = NULL;
}
//----------------------------------------------------------------------------------------------------
TModelDX::~TModelDX()
{
  mVectorLOD.clear();
  int cnt = mVectorAllEffect.size();
  for(int i = 0 ; i < cnt ; i++ )
  {
    delete mVectorAllEffect[i];
  }
  mVectorAllEffect.clear();

  mSetPathTexture.clear();
}
//----------------------------------------------------------------------------------------------------
void TModelDX::Draw(std::vector<unsigned char>* state, // какое состояние нарисовать (От ObjectDX)
                    std::vector<unsigned char>* mask,  // какую из частей нарисовать (От ObjectDX)
                    D3DXMATRIXA16* matrix,// ориентация составных частей внутри объекта, кол-во совпадает с cSubset (От ObjectDX)
                    D3DXMATRIXA16* mWorld,// где и как расположен объект         (От ObjectDX)
                    D3DXMATRIXA16* mView, // расположение и ориентация камеры    (от ManagerDirectX)
                    D3DXMATRIXA16* mProj,
                    const D3DXVECTOR3* mCamera) // проецирование на плоскость экрана  (от ManagerDirectX)
{
  D3DXMATRIXA16 mWorldViewProjection;
  mWorldViewProjection = (*mWorld) * (*mView) * (*mProj);
  //1 Выбрать по ЛОДу mesh
  // расчет расстояния
  float dist = GetDist(mWorld,mView);
  //2 Настроить pEffect на координаты и ориентацию - передать в pEffect матрицу
  // если mArrMatrixSubset==NULL, то координаты и ориентация частей модели неизменны
  //3 Выбрать по состоянию subSet
  TLOD* pCurLOD = &mVectorLOD[0];
  if(mVectorLOD.size()>1)
    if(dist>mLOD)
      pCurLOD = &mVectorLOD[1];

  int cnt = GetCntEffect();
  for(int i = 0 ; i < cnt ; i++)
  {
    if(mask->operator[](i)==1)
    {
      TEffectDX* pEffect = NULL;
      
      if(pCurLOD->normal.size()&&pCurLOD->damage.size())
      {
        pEffect = pCurLOD->normal[i];
        if(!state->operator[](i))
          pEffect = pCurLOD->damage[i];
      }
      else 
      {
        if(pCurLOD->normal.size())
          pEffect = pCurLOD->normal[i];
        else
          pEffect = pCurLOD->damage[i];
      }
      //pEffect->SetMatrixWorld(matrix+i);
      pEffect->SetMatrixWorld(mWorld);//###
      Draw(pEffect,mWorldViewProjection,mCamera);
    }
  }
}
//---------------------------------------------------------------------------------------------------
void TModelDX::Draw( TEffectDX* pEffect,D3DXMATRIXA16& mWorldViewProjection,const  D3DXVECTOR3* mCamera)
{
  ID3DXMesh* pCurMesh = pEffect->pMesh;

  HRESULT hr;
  UINT iPass, cPasses;
  pEffect->SetMatrixWorldViewProjection(&mWorldViewProjection);
  pEffect->SetCameraPosition(mCamera);

  V( pEffect->Begin( &cPasses, 0 ) );
  for( iPass = 0; iPass < cPasses; iPass++ )
  {
    V( pEffect->BeginPass( iPass ) );
    // Render the mesh with the applied technique
      V( pCurMesh->DrawSubset( 0 /*Subset*/ ) );
    V( pEffect->EndPass() );
  }
  V( pEffect->End() );
}
//----------------------------------------------------------------------------------------------------
bool TModelDX::Init(IDirect3DDevice9* pd3dDevice, LPCWSTR strAbsPath/*путь к файлам модели*/)
{
  m_pd3dDevice = pd3dDevice;
  // загрузка данных примитивов, текстур и индексов.
  GlobalLoggerDX.WriteF_time("Загрузка Mesh, id=%u\n",mID);
  if(Load(strAbsPath)==false) 
    return false;

  GlobalLoggerDX.WriteF_time("Конец загрузки Mesh, id=%u\n",mID);

  LPCWSTR strFilenameShader = NULL;
  HRESULT hr;
  WCHAR str[MAX_PATH];
  DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;

  int cnt = mVectorAllEffect.size();
  // структурировать данные
  for(int i = 0 ; i < cnt ; i++)
  {
    ID3DXEffect* pEffect;
    TEffectDX* pEffectDX = mVectorAllEffect[i];
    // If this fails, there should be debug output as to 
    // they the .fx file failed to compile
    V( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, 
      pEffectDX->mMaterial.strShader.data() ) );

    V( D3DXCreateEffectFromFile( pd3dDevice, str, 
      NULL, NULL, dwShaderFlags, NULL, &pEffect, NULL ) );
    pEffectDX->p = pEffect;
    pEffectDX->Init();
  }

  SetupVectorLOD();

  return true;
}
//----------------------------------------------------------------------------------------------------
float TModelDX::GetDist(D3DXMATRIXA16* mWorld, D3DXMATRIXA16* mView)
{
  float dist;
  float dx = mView->_41-mWorld->_41;
  float dy = mView->_42-mWorld->_42;
  float dz = mView->_43-mWorld->_43;
  dist = sqrt(dx*dx+dy*dy+dz*dz);
  return dist;
}
//----------------------------------------------------------------------------------------------------
void TModelDX::Destroy()
{
  int cnt = GetCntEffect();
  for(int i = 0 ; i < cnt ; i++)
    mVectorAllEffect[i]->Destroy();
}
//----------------------------------------------------------------------------------------------------
void TModelDX::LostDevice()
{
  int cnt = GetCntEffect();
  for(int i = 0 ; i < cnt ; i++)
    mVectorAllEffect[i]->LostDevice();
}
//----------------------------------------------------------------------------------------------------
void TModelDX::ResetDevice()
{
  int cnt = GetCntEffect();
  for(int i = 0 ; i < cnt ; i++)
    mVectorAllEffect[i]->ResetDevice();
}
//----------------------------------------------------------------------------------------------------
bool TModelDX::Load(LPCWSTR strFilenameData)
{
  ILoaderModelDX* pLoadModel;
  pLoadModel = GetLoaderModelDX(m_pd3dDevice);
  if(pLoadModel->Load(strFilenameData)==false)
  {
    USES_CONVERSION;
    GlobalLoggerDX.WriteF_time("Не удалось загрузить модель с HDD: %s.\n",W2A(strFilenameData));
    delete pLoadModel;
    return false;
  }
  //-------------------------------------------------------------------------------------------
  mLOD = pLoadModel->GetLOD();
  int cnt = pLoadModel->GetCountGroup();
  for(int i = 0 ; i < cnt ; i++)
  {
    ILoaderModelDX::TDefGroup *pDef = pLoadModel->GetGroup(i);
    // заполнение
    AddEffectDX(pDef);
  }

  delete pLoadModel;
  return true;
}
//----------------------------------------------------------------------------------------------------
bool TModelDX::AddEffectDX(ILoaderModelDX::TDefGroup * pDefGroup)
{
  TEffectDX* pEffect = new TEffectDX;

  pEffect->mTypeLOD     = pDefGroup->mTypeLOD;
  pEffect->mflgNormal   = pDefGroup->mflgNormal;
  pEffect->sName        = pDefGroup->strName;

  // Joint
  pEffect->CreateJoint( pDefGroup->mCntJoint );
  for(int i = 0 ; i < pDefGroup->mCntJoint ; i++ )
  {
    pEffect->AddJoint(i,pDefGroup->pArrJoint[i].namePart, pDefGroup->pArrJoint[i].matrix);
  }
  //----------------------------------------------------------
  // Load material textures
  TEffectDX::Material* pMaterial = &pEffect->mMaterial;
  // заполнить материал данными
  pMaterial->strTechnique = pDefGroup->strTechnique;
  pMaterial->strName      = pDefGroup->strName;
  pMaterial->strTexture   = pDefGroup->strTexture;
  USES_CONVERSION;
  pMaterial->strShader = A2W(pDefGroup->strPathShader.data());

  pMaterial->vAmbient   = pDefGroup->vAmbient;
  pMaterial->vDiffuse   = pDefGroup->vDiffuse;
  pMaterial->vSpecular  = pDefGroup->vSpecular;

  pMaterial->nShininess = pDefGroup->nShininess;
  pMaterial->fAlpha     = pDefGroup->fAlpha;

  pMaterial->bSpecular  = pDefGroup->bSpecular;

  LoadTexture(pMaterial);

  pEffect->pMesh = pDefGroup->pMesh;

  mVectorAllEffect.push_back(pEffect);
  return true;
}
//----------------------------------------------------------------------------------------------------
unsigned int TModelDX::GetIndexVisualGroupByName(char* sName, int num)
{
  TLOD* pLOD = &mVectorLOD[0];
  std::vector<TEffectDX*>*  v = pLOD->GetNonZeroVector();
  int cnt = v->size();
  int iFound = 0;
  for(int i = 0 ; i < cnt ; i++)
  {
    if(strcmp(v->operator [](i)->sName.data(),sName)==0)
    {
      if(iFound==num)
        return i;
      iFound++;
    }
  }
  GlobalLoggerDX.WriteF_time("Не найден эффект модели имя %s номер %d\n",sName,num);
  return 0;
}
//----------------------------------------------------------------------------------------------------
D3DXMATRIXA16* TModelDX::GetMatrixByName(char* sNameBlendBone/*к чему*/, int num, char* sNamePart/*что*/)
{
  unsigned int index = GetIndexVisualGroupByName(sNameBlendBone, num);
  TLOD* pLOD = &mVectorLOD[0];
  std::vector<TEffectDX*>*  v = pLOD->GetNonZeroVector();
  TEffectDX* pEffectDX = v->operator [](index);
  if(pEffectDX)
    return pEffectDX->GetBlendMatrixByName(sNamePart);
  else
    GlobalLoggerDX.WriteF_time("Не найден эффект модели имя %s номер %d\n",sNameBlendBone,num);
  return NULL;
}
//----------------------------------------------------------------------------------------------------
int TModelDX::GetCntEffect()
{
  TLOD* pLOD = &mVectorLOD[0];
  std::vector<TEffectDX*>*  v = pLOD->GetNonZeroVector();
  return v->size();
}
//----------------------------------------------------------------------------------------------------
void TModelDX::SetupVectorLOD()
{
  mVectorLOD.clear();
  TLOD lod;
  mVectorLOD.push_back(lod);
  bool TwoLOD = false;
  int cnt = mVectorAllEffect.size();
  for(int i = 0 ; i < cnt ; i++)
  {
    if(mVectorAllEffect[i]->mTypeLOD==1)
    {
      TwoLOD = true;
      break;
    }
  }
  //----------------------------------------------------------------------------------------------------
  if(TwoLOD)
  mVectorLOD.push_back(lod);
  //----------------------------------------------------------------------------------------------------
  // структурировать данные
  for(int i = 0 ; i < cnt ; i++)
  {
    TEffectDX* pEffectDX = mVectorAllEffect[i];
    if(pEffectDX->mflgNormal)
      mVectorLOD[pEffectDX->mTypeLOD].normal.push_back(pEffectDX);
    else
      mVectorLOD[pEffectDX->mTypeLOD].damage.push_back(pEffectDX);
  }
}
//----------------------------------------------------------------------------------------------------
void TModelDX::LoadTexture(TEffectDX::Material* pMaterial)
{
  HRESULT hr;
  set<wstring>::iterator fit = mSetPathTexture.find(pMaterial->strTexture);
  set<wstring>::iterator eit = mSetPathTexture.end();
  if(fit==eit)// не нашли
  {
    V( D3DXCreateTextureFromFile( m_pd3dDevice, pMaterial->strTexture.data(),
      &( pMaterial->pTexture ) ) );

  }
  else// дубликат
  {

  }
}
//----------------------------------------------------------------------------------------------------