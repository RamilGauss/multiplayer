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
#include "LoggerDX.h"
#include "LoaderModelDX.h"
#include "BL_Debug.h"



// Vertex declaration
D3DVERTEXELEMENT9 VERTEX_MODELDX_DECL[] =
{
  { 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_POSITION, 0},
  { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_NORMAL,   0},
  { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TEXCOORD, 0},
  D3DDECL_END()
};


using namespace nsStruct3D;

TModelDX::TModelDX()
{
  mID = -1;
  mCntEffectVisual = 0;
  mCntEffectInLOD   = 0;// как правило в 2 или 1 раз больше чем mCntEffectVisual
  mCntAllEffect    = 0;// как правило в 4 или 2 раза больше чем mCntEffectVisual
  mLOD             = 0;
  mArrEffect0       = NULL;
  mArrEffect1       = NULL;
  pArrAllEffect = NULL;

  m_pd3dDevice = NULL;
}
//----------------------------------------------------------------------------------------------------
TModelDX::~TModelDX()
{
  if(mArrEffect0!=mArrEffect1)
  {
    delete []mArrEffect0;
    mArrEffect0       = NULL;
    delete []mArrEffect1;
    mArrEffect1       = NULL;
  }
  else
  {
    delete []mArrEffect0;
    mArrEffect0       = NULL;
    mArrEffect1       = NULL;
  }
  delete []pArrAllEffect;
  pArrAllEffect = NULL;

  mCntEffectVisual = 0;
  mCntEffectInLOD  = 0;// как правило в 2 или 1 раз больше чем mCntEffectVisual
  mCntAllEffect    = 0;// как правило в 4 или 2 раза больше чем mCntEffectVisual
}
//----------------------------------------------------------------------------------------------------
void TModelDX::Draw(unsigned int state,             //                              (От ObjectDX)
                    D3DXMATRIXA16* mArrMatrixSubset,//кол-во совпадает с mCntEffect (От ObjectDX)
                    D3DXMATRIXA16* mWorld,// где и как расположен объект            (От ObjectDX)
                    D3DXMATRIXA16* mView, // расположение и ориентация камеры       (от ManagerDirectX)
                    D3DXMATRIXA16* mProj) // проектирование на плоскость экрана     (от ManagerDirectX)
{
  mWorldViewProjection = (*mWorld) * (*mView) * (*mProj);
  //1 Выбрать по ЛОДу mesh
  // расчет расстояния
  float dist = GetDist(mWorld,mView);
  //2 Настроить pEffect на координаты и ориентацию - передать в pEffect матрицу
  // если mArrMatrixSubset==NULL, то координаты и ориентация частей модели неизменны
  //3 Выбрать по состоянию subSet
  TLOD* pCurLOD = mArrEffect0;
  if(dist>mLOD)
    pCurLOD = mArrEffect1;

  for(int i = 0 ; i < mCntEffectVisual ; i++)
  {
    TEffectDX* pEffect;
    int subset;
    if(state&(i>>1))
      pEffect = pCurLOD[i].mEffectDX_damage;
    else
      pEffect = pCurLOD[i].mEffectDX_normal;

    pEffect->SetMatrixWorld(&mArrMatrixSubset[i]);
    Draw(pEffect);
  }
}
//---------------------------------------------------------------------------------------------------
void TModelDX::Draw( TEffectDX* pEffect)
{
  ID3DXMesh* pCurMesh = pEffect->pMesh;
  HRESULT hr;
  UINT iPass, cPasses;
  pEffect->SetMatrixWorldViewProjection(&mWorldViewProjection);

  V( pEffect->Begin( &cPasses, 0 ) );
  for( iPass = 0; iPass < cPasses; iPass++ )
  {
    V( pEffect->BeginPass( iPass ) );
    // Render the mesh with the applied technique
    V( pCurMesh->DrawSubset( 0/*pEffect->mSubset*/ ) );
    V( pEffect->EndPass() );
  }
  V( pEffect->End() );
}
//----------------------------------------------------------------------------------------------------
void TModelDX::Init(IDirect3DDevice9* pd3dDevice, LPCWSTR strAbsPath/*путь к файлам модели*/)
{
  m_pd3dDevice = pd3dDevice;
  // загрузка данных примитивов, текстур и индексов.
  if(Load(strAbsPath)==false) 
    return;

  LPCWSTR strFilenameShader = NULL;
  HRESULT hr;
  WCHAR str[MAX_PATH];
  DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;

  for(int i = 0 ; i < mCntEffectVisual ; i++)
  {
    ID3DXEffect* pEffect;
    // If this fails, there should be debug output as to 
    // they the .fx file failed to compile
    if(mArrEffect0[i].mEffectDX_normal->isN0() ||
       mArrEffect0[i].mEffectDX_normal->p==NULL)
    {
      V( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, 
         mArrEffect0[i].mEffectDX_normal->mMaterial.strShader) );

      V( D3DXCreateEffectFromFile( pd3dDevice, str, 
        NULL, NULL, dwShaderFlags, NULL, &pEffect, NULL ) );
      mArrEffect0[i].mEffectDX_normal->p = pEffect;
      mArrEffect0[i].mEffectDX_normal->Init(i*4+0);
    }
    //-----------------------------------------------------------------------------
    // If this fails, there should be debug output as to 
    // they the .fx file failed to compile
    if(mArrEffect0[i].mEffectDX_damage->isD0() ||
       mArrEffect0[i].mEffectDX_damage->p==NULL)
    {
      V( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, 
        mArrEffect0[i].mEffectDX_damage->mMaterial.strShader) );

      V( D3DXCreateEffectFromFile( pd3dDevice, str, 
        NULL, NULL, dwShaderFlags, NULL, &pEffect, NULL ) );
      mArrEffect0[i].mEffectDX_damage->p = pEffect;
      mArrEffect0[i].mEffectDX_damage->Init(i*4+1);
    }
    //-----------------------------------------------------------------------------
    // If this fails, there should be debug output as to 
    // they the .fx file failed to compile
    if(mArrEffect1[i].mEffectDX_normal->isN1() ||
       mArrEffect1[i].mEffectDX_normal->p==NULL)
    {
      V( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, 
        mArrEffect1[i].mEffectDX_normal->mMaterial.strShader) );

      V( D3DXCreateEffectFromFile( pd3dDevice, str, 
        NULL, NULL, dwShaderFlags, NULL, &pEffect, NULL ) );
      mArrEffect1[i].mEffectDX_normal->p = pEffect;
      mArrEffect1[i].mEffectDX_normal->Init(i*4+2);
    }
    //-----------------------------------------------------------------------------
    // If this fails, there should be debug output as to 
    // they the .fx file failed to compile
    if(mArrEffect1[i].mEffectDX_damage->isD1() ||
       mArrEffect1[i].mEffectDX_damage->p==NULL)
    {
      V( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, 
        mArrEffect1[i].mEffectDX_damage->mMaterial.strShader) );

      V( D3DXCreateEffectFromFile( pd3dDevice, str, 
        NULL, NULL, dwShaderFlags, NULL, &pEffect, NULL ) );
      mArrEffect1[i].mEffectDX_damage->p = pEffect;
      mArrEffect1[i].mEffectDX_damage->Init(i*4+3);
    }
  }
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
  for(int i = 0 ; i < mCntEffectVisual ; i++)
  {
    if(mArrEffect0[i].mEffectDX_normal->isN0())
      mArrEffect0[i].mEffectDX_normal->Destroy();
    if(mArrEffect0[i].mEffectDX_damage->isD0())
      mArrEffect0[i].mEffectDX_damage->Destroy();
    if(mArrEffect1[i].mEffectDX_normal->isN1())
      mArrEffect1[i].mEffectDX_normal->Destroy();
    if(mArrEffect1[i].mEffectDX_damage->isD0())
      mArrEffect1[i].mEffectDX_damage->Destroy();
  }
}
//----------------------------------------------------------------------------------------------------
void TModelDX::LostDevice()
{
  for(int i = 0 ; i < mCntEffectVisual ; i++)
  {
    if(mArrEffect0[i].mEffectDX_normal->isN0())
      mArrEffect0[i].mEffectDX_normal->LostDevice();
    if(mArrEffect0[i].mEffectDX_damage->isD0())
      mArrEffect0[i].mEffectDX_damage->LostDevice();
    if(mArrEffect1[i].mEffectDX_normal->isN1())
      mArrEffect1[i].mEffectDX_normal->LostDevice();
    if(mArrEffect1[i].mEffectDX_damage->isD0())
      mArrEffect1[i].mEffectDX_damage->LostDevice();
  }
}
//----------------------------------------------------------------------------------------------------
void TModelDX::ResetDevice()
{
  for(int i = 0 ; i < mCntEffectVisual ; i++)
  {
    if(mArrEffect0[i].mEffectDX_normal->isN0())
      mArrEffect0[i].mEffectDX_normal->ResetDevice();
    if(mArrEffect0[i].mEffectDX_damage->isD0())
      mArrEffect0[i].mEffectDX_damage->ResetDevice();
    if(mArrEffect1[i].mEffectDX_normal->isN1())
      mArrEffect1[i].mEffectDX_normal->ResetDevice();
    if(mArrEffect1[i].mEffectDX_damage->isD0())
      mArrEffect1[i].mEffectDX_damage->ResetDevice();
  }
}
//----------------------------------------------------------------------------------------------------
bool TModelDX::Load(LPCWSTR strFilenameData)
{
  ILoaderModelDX* pLoadModel;
  pLoadModel = new TLoaderModelDX(m_pd3dDevice);
  if(pLoadModel->Load(strFilenameData)==false)
  {
    USES_CONVERSION;
    GlobalLoggerDX.WriteF_time("Не удалось загрузить модель с HDD: %s.\n",W2A(strFilenameData));
    delete pLoadModel;
    return false;
  }
  //-------------------------------------------------------------------------------------------
  mCntEffectVisual = pLoadModel->mCntEffectVisual;
  mCntEffectInLOD  = pLoadModel->mCntEffectInLOD;
  //------------------------------------------------
  mLOD = pLoadModel->GetLOD();
  mID  = pLoadModel->GetID_Unique();
  mCntAllEffect = pLoadModel->GetCountSubset();
  pArrAllEffect = new TEffectDX[mCntAllEffect];
  for(int i = 0 ; i < mCntAllEffect ; i++)
  {
    ILoaderModelDX::TDefGroup *pDef = &(pLoadModel->GetArrDefGroup()[i]);
    // заполнение
    SetupEffectDX(&pArrAllEffect[i],pDef);
  }
  // структурировать данные
  if(mCntAllEffect!=mCntEffectInLOD)
  {
    mArrEffect0 = new TLOD[mCntEffectVisual];
    mArrEffect1 = new TLOD[mCntEffectVisual];
  }
  else
  {
    mArrEffect0 = new TLOD[mCntEffectVisual];
    mArrEffect1 = mArrEffect0;
  }
  for(int i = 0 ; i < mCntAllEffect ; i++)
  {
    TLOD* pCurLOD = mArrEffect1;
    if(pArrAllEffect[i].mTypeLOD==0)
      pCurLOD = mArrEffect0;
    if(pArrAllEffect[i].mflgNormal)
      pCurLOD[pArrAllEffect[i].mIndexVisual].mEffectDX_normal = &pArrAllEffect[i];
    else
      pCurLOD[pArrAllEffect[i].mIndexVisual].mEffectDX_damage = &pArrAllEffect[i];
  }
  for(int i = 0 ; i < mCntEffectVisual ; i++)
  {
    if(mArrEffect0[i].mEffectDX_damage == NULL)
      mArrEffect0[i].mEffectDX_damage = mArrEffect0[i].mEffectDX_normal;
    else
      mArrEffect0[i].mEffectDX_normal = mArrEffect0[i].mEffectDX_damage;
    BL_ASSERT(mArrEffect0[i].mEffectDX_normal!=NULL);
    BL_ASSERT(mArrEffect0[i].mEffectDX_damage!=NULL);
    //--------------------------------------------------------------------
    if(mArrEffect1[i].mEffectDX_damage == NULL)
      mArrEffect1[i].mEffectDX_damage = mArrEffect1[i].mEffectDX_normal;
    else
      mArrEffect1[i].mEffectDX_normal = mArrEffect1[i].mEffectDX_damage;
    BL_ASSERT(mArrEffect1[i].mEffectDX_normal!=NULL);
    BL_ASSERT(mArrEffect1[i].mEffectDX_damage!=NULL);
  }
  delete pLoadModel;
  return true;
}
//----------------------------------------------------------------------------------------------------
bool TModelDX::SetupEffectDX(TEffectDX *pEffect,ILoaderModelDX::TDefGroup * pDefGroup)
{
  HRESULT hr;
  
  pEffect->mIndexVisual = pDefGroup->mIndex;
  pEffect->mTypeLOD     = pDefGroup->mTypeLOD;
  pEffect->mflgNormal   = pDefGroup->mflgNormal;
  
  // Load material textures
  TEffectDX::Material* pMaterial = &pEffect->mMaterial;
  // заполнить материал данными
  strcpy(pMaterial->strTechnique,pDefGroup->strTechnique);
  strcpy(pMaterial->strName,pDefGroup->strName);
  wcscpy(pMaterial->strTexture,pDefGroup->strTexture);
  USES_CONVERSION;
  wcscpy(pMaterial->strShader,A2W(pDefGroup->strPathShader));

  pMaterial->vAmbient   = pDefGroup->vAmbient;
  pMaterial->vDiffuse   = pDefGroup->vDiffuse;
  pMaterial->vSpecular  = pDefGroup->vSpecular;

  pMaterial->nShininess = pDefGroup->nShininess;
  pMaterial->fAlpha     = pDefGroup->fAlpha;

  pMaterial->bSpecular  = pDefGroup->bSpecular;


  V( D3DXCreateTextureFromFile( m_pd3dDevice, pMaterial->strTexture,
    &( pMaterial->pTexture ) ) );
  
  pEffect->pMesh = pDefGroup->pMesh;
  // оптимизация структуры данных
  int cntAdjacency = pEffect->pMesh->GetNumFaces() * 3;
  DWORD* aAdjacency = new DWORD[cntAdjacency];
  if( aAdjacency == NULL )
  {
    GlobalLoggerDX.WriteF_time("Нехватка памяти.SetupEffectDX().\n");
    return false;
  }

  V( pEffect->pMesh->GenerateAdjacency( 1e-6f, aAdjacency ) );
  V( pEffect->pMesh->OptimizeInplace( D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE, aAdjacency, NULL, NULL, NULL ) );

  SAFE_DELETE_ARRAY( aAdjacency );
  return true;
}
//----------------------------------------------------------------------------------------------------
