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

#include "ModelDX.h"
#include "SDKmisc.h"
#include "LoaderModelDX.h"
#include "Logger.h"
#include "LoaderModelDX.h"
#include "BL_Debug.h"
#include "MakerLoaderModelGE.h"
#include <stdlib.h>
#include <d3dx10math.h>
#include "SortByAlphabetic.h"
#include "ManagerResourceDX.h"
#include "HiTimer.h"

using namespace nsStruct3D;
using namespace std;

const char* NameBeginOutputParam = "BeginOutputParam";// в будущем перенести в файл настроек
const char* NameEndOutputParam   = "EndOutputParam";

TModelDX::TModelDX(TManagerResourceDX* managerResourceDX)
{
  mManagerResourceDX = managerResourceDX;

  mID = -1;
  m_pd3dDevice = NULL;
}
//----------------------------------------------------------------------------------------------------
TModelDX::~TModelDX()
{
  Done();
}
//----------------------------------------------------------------------------------------------------
void TModelDX::Draw(vector<unsigned char>* state, // какое состояние нарисовать (От ObjectDX)
                    vector<unsigned char>* mask,  // какую из частей нарисовать (От ObjectDX)
                    vector<TMatrix16*>* matrix,// ориентация составных частей внутри объекта, кол-во совпадает с cSubset (От ObjectDX)
                    TMatrix16* pWorld,// где и как расположен объект         (От ObjectDX)
                    float alphaTransparency,
                    TMatrix16* pView)/*для определения дистанции*/ // расположение и ориентация камеры    (от ManagerDirectX)                   
{
  D3DXMATRIXA16 mView;
  MATRIX16_EQUAL_M_P(mView,pView)

  D3DXMATRIXA16 mWorld;// только из набора видимых
  MATRIX16_EQUAL_M_P(mWorld,pWorld)

  //1 Выбрать по ЛОДу mesh
  // расчет расстояния
  float dist = GetDist(&mWorld,&mView);
  //2 Настроить pEffect на координаты и ориентацию - передать в pEffect матрицу
  // если mArrMatrixSubset==NULL, то координаты и ориентация частей модели неизменны
  //3 Выбрать по состоянию subSet
  TLOD* pCurLOD = &mVectorLOD[0];
  if(mVectorLOD.size()>1)
    if(dist>mLOD)
      pCurLOD = &mVectorLOD[1];

  int cnt = GetCntEffect();
  int iView = 0;
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
      D3DXMATRIXA16 m;// только из набора видимых
      TMatrix16* pV = matrix->operator [](iView);// только из набора видимых
      MATRIX16_EQUAL_M_P(m,pV)

      D3DXMATRIXA16 mWorld_pro = m * mWorld;
      //-------------------------------------------------
      pEffect->GetAlphaTransparency(alphaTransparency);
      pEffect->SetInnerShaderParam();
      pEffect->SetMatrixWorld(&mWorld_pro);
      SetShaderStack(pEffect,i);// все части, включая дубликаты
      Draw(pEffect);
      iView++;
    }
  }
}
//---------------------------------------------------------------------------------------------------
void TModelDX::Draw( TEffectDX* pEffect)
{
  ID3DXMesh* pCurMesh = pEffect->pMesh;

  HRESULT hr;
  UINT iPass, cPasses;

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
  Done();
  m_pd3dDevice = pd3dDevice;
  // загрузка данных примитивов, текстур и индексов.
  GlobalLoggerGE.WriteF_time("Загрузка Mesh, ID=%u\n",mID);
  if(Load(strAbsPath)==false) 
    return false;

  GlobalLoggerGE.WriteF_time("Конец загрузки Mesh, ID=%u\n",mID);

  int cnt = mVectorAllEffect.size();
  // структурировать данные
  for(int i = 0 ; i < cnt ; i++)
  {
    TEffectDX* pEffectDX = mVectorAllEffect[i];
    LoadEffect(pEffectDX); 
    pEffectDX->Init();
  }
  SetupVectorLOD();

  SetupShaderStack();
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
  ILoaderModelGE* pLoadModel;
  pLoadModel = GetLoaderModelGE(m_pd3dDevice);
  if(pLoadModel->Load(strFilenameData)==false)
  {
    USES_CONVERSION;
    GlobalLoggerGE.WriteF_time("Не удалось загрузить модель: %s.\n",W2A(strFilenameData));
    delete pLoadModel;
    return false;
  }
  //-------------------------------------------------------------------------------------------
  mLOD = pLoadModel->GetLOD();
  int cnt = pLoadModel->GetCountGroup();
  for(int i = 0 ; i < cnt ; i++)
  {
    ILoaderModelGE::TDefGroup *pDef = pLoadModel->GetGroup(i);
    // заполнение
    AddEffectDX(pDef);
  }

  delete pLoadModel;
  return true;
}
//----------------------------------------------------------------------------------------------------
bool TModelDX::AddEffectDX(ILoaderModelGE::TDefGroup * pDefGroup)
{
  TEffectDX* pEffect = new TEffectDX;

  pEffect->mTypeLOD     = pDefGroup->mTypeLOD;
  pEffect->mflgNormal   = pDefGroup->mflgNormal;
  //----------------------------------------------------------
  // Load material textures
  TEffectDX::Material* pMaterial = &pEffect->mMaterial;
  // заполнить материал данными
  pMaterial->strTechnique = pDefGroup->strTechnique;
  pMaterial->strName      = pDefGroup->strName;
  pMaterial->mNumUse      = pDefGroup->mNumUse;
  pMaterial->strTexture   = pDefGroup->strTexture;
  USES_CONVERSION;
  pMaterial->strShader = A2W(pDefGroup->strPathShader.data());

  pMaterial->vAmbient   = D3DXVECTOR3(pDefGroup->vAmbient.x, pDefGroup->vAmbient.y, pDefGroup->vAmbient.z );
  pMaterial->vDiffuse   = D3DXVECTOR3(pDefGroup->vDiffuse.x, pDefGroup->vDiffuse.y, pDefGroup->vDiffuse.z );
  pMaterial->vSpecular  = D3DXVECTOR3(pDefGroup->vSpecular.x,pDefGroup->vSpecular.y,pDefGroup->vSpecular.z);

  pMaterial->nShininess = pDefGroup->nShininess;

  pMaterial->bSpecular  = pDefGroup->bSpecular;

  LoadTexture(pEffect);

  pEffect->pMesh = (ID3DXMesh*)pDefGroup->pMesh;

  mVectorAllEffect.push_back(pEffect);
  return true;
}
//----------------------------------------------------------------------------------------------------
unsigned int TModelDX::GetIndexVisualGroupByName(char* sName, int num)
{
  TLOD* pLOD = &mVectorLOD[0];
  vector<TEffectDX*>*  v = pLOD->GetNonZeroVector();
  int cnt = v->size();
  int iFound = 0;
  for(int i = 0 ; i < cnt ; i++)
  {
    if(strcmp(v->operator [](i)->mMaterial.strName.data(),sName)==0)
    {
      if(iFound==num)
        return i;
      iFound++;
    }
  }
  GlobalLoggerGE.WriteF_time("Не найден эффект модели имя %s номер %d\n",sName,num);
  return 0;
}
//----------------------------------------------------------------------------------------------------
const char* TModelDX::GetNameByIndex(int index)
{
  TLOD* pLOD = &mVectorLOD[0];
  vector<TEffectDX*>*  v = pLOD->GetNonZeroVector();
  return v->operator [](index)->mMaterial.strName.data();
}
//----------------------------------------------------------------------------------------------------
int TModelDX::GetNumUseByIndex(int index)
{
  TLOD* pLOD = &mVectorLOD[0];
  vector<TEffectDX*>*  v = pLOD->GetNonZeroVector();
  return v->operator [](index)->mMaterial.mNumUse;
}
//----------------------------------------------------------------------------------------------------
int TModelDX::GetCntEffect()
{
  TLOD* pLOD = &mVectorLOD[0];
  vector<TEffectDX*>*  v = pLOD->GetNonZeroVector();
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
void TModelDX::LoadTexture(TEffectDX* pEffectDX)
{
  TEffectDX::Material* pMaterial = &(pEffectDX->mMaterial);
  pMaterial->pTexture = (IDirect3DTexture9*)mManagerResourceDX->Load(pMaterial->strTexture.data());
}
//----------------------------------------------------------------------------------------------------
void TModelDX::LoadEffect(TEffectDX* pEffectDX)
{
  //guint32 start = ht_GetMSCount();

  pEffectDX->p = (ID3DXEffect*)mManagerResourceDX->Load(pEffectDX->mMaterial.strShader.data());

  //start = ht_GetMSCount() - start;
  //GlobalLoggerGE.WriteF_time("Загрузка эффекта t=%u\n",start);
}
//----------------------------------------------------------------------------------------------------
void TModelDX::SortPartByAlphabetic()
{
  int cntLod = mVectorLOD.size();
  for(int i = 0 ; i < cntLod ; i++)
  {
    TLOD* pLOD = &mVectorLOD.at(i);
    vector<TEffectDX*>* pVecEffect = NULL;
    switch(i)
    {
      case 0:
        pVecEffect = &(pLOD->normal);
        break;
      case 1:
        pVecEffect = &(pLOD->damage);
        break;
      default:;     
    }
    //----------------------------------------------------
    int cntPart = pVecEffect->size();
    vector<TSortByAlphabetic::T> vectorName_Use;
    for(int iPart = 0 ; iPart < cntPart ; iPart++)
    {
      TEffectDX* pEffectDX = pVecEffect->at(iPart);
      TSortByAlphabetic::T t;
      t.s = pEffectDX->mMaterial.strName;
      t.v = pEffectDX->mMaterial.mNumUse;
      t.index = iPart;
      vectorName_Use.push_back(t);
    }
    // сортируем
    TSortByAlphabetic sort;
    sort.Sort(&vectorName_Use);
    // в соответствии с вектором индексов согласуем вектор эффектов
    vector<TEffectDX*> pNewEffect;
    for(int iPart = 0 ; iPart < cntPart ; iPart++)
    {
      int index = vectorName_Use.at(iPart).index;
      TEffectDX* pEffectDX = pVecEffect->at(index);
      pNewEffect.push_back(pEffectDX);
    }
    *pVecEffect = pNewEffect;
  }
}
//----------------------------------------------------------------------------------------------------
void TModelDX::SetShaderStackMask(vector<int>* pVectorMask)// по этой маске настраиваются шейдеры эффектов
{
  mVectorShaderStackMask = *pVectorMask;
}
//----------------------------------------------------------------------------------------------------
TShaderStack* TModelDX::GetShaderStack(int index)// взять стек для настройки
{
  return mVectorShaderStack[index];
}
//----------------------------------------------------------------------------------------------------
void TModelDX::Done()
{
  mVectorLOD.clear();
  int cnt = mVectorAllEffect.size();
  for(int i = 0 ; i < cnt ; i++ )
    delete mVectorAllEffect[i];

  mVectorAllEffect.clear();

  cnt = mVectorShaderStack.size();
  for(int i = 0 ; i < cnt ; i++ )
  {
    delete mVectorShaderStack.at(i);
  }
  mVectorShaderStack.clear();

  mVectorShaderStackMask.clear();
}
//----------------------------------------------------------------------------------------------------
void TModelDX::SetShaderStack(TEffectDX* pEffect,int index)
{
  if(mVectorShaderStackMask[index]==1)
  {
    TShaderStack* pSS = mVectorShaderStack[index];
    mExecSS.Set(pEffect->p,pSS);
  }
}
//----------------------------------------------------------------------------------------------------
void TModelDX::SetupShaderStack()
{
  vector<TEffectDX*>* pPart = mVectorLOD.at(0).GetNonZeroVector();
  int cnt = pPart->size();
  for(int i = 0 ; i < cnt ; i++ )
  {
    TEffectDX* pEffectDX = pPart->at(i);
    if(pEffectDX->mflgNormal)
      AddShaderStack(pEffectDX->p);
  }
}
//----------------------------------------------------------------------------------------------------
void TModelDX::AddShaderStack(ID3DXEffect* p)
{
  char pData[200];// для ускорения загрузки фальшивых данных
  TShaderStack* pSS = new TShaderStack;
  mVectorShaderStack.push_back(pSS);
  mVectorShaderStackMask.push_back(0);
  //----------------------------------------
  HRESULT hr;
  D3DXEFFECT_DESC pDesc;
  V( p->GetDesc(&pDesc) );

  bool flgBeginParam = false;
  int cnt = pDesc.Parameters;
  // поиск начала output param
  for(int i = 0 ; i < cnt ; i++)
  {
    D3DXPARAMETER_DESC pDescParam;
    D3DXHANDLE handle = p->GetParameter( 0, i );
    V( p->GetParameterDesc( handle, &pDescParam ) );
    if(pDescParam.Semantic)
    if(strcmp(pDescParam.Semantic,NameBeginOutputParam)==0)
      flgBeginParam = true;
    else 
    {
      if(strcmp(pDescParam.Semantic,NameEndOutputParam)==0)
        flgBeginParam = false;
      else
      {
        if(flgBeginParam)
        {
          int size = pDescParam.Bytes;
          pSS->Push(pDescParam.Semantic,pData,size);
        }
      }
    }
  }
}
//----------------------------------------------------------------------------------------------------