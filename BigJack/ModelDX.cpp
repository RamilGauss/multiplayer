/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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
#include <atlconv.h>
#include "IGraphicEngine.h"

using namespace nsStruct3D;
using namespace std;

const char* NameBeginOutputParam = "BeginOutputParam";// � ������� ��������� � ���� ��������
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
void TModelDX::Draw(vector<void*>* pVecTexCubeMap,    //                           (�� ObjectDX)
                    vector<unsigned char>* state, // ����� ��������� ���������� (�� ObjectDX)
                    vector<unsigned char>* mask,  // ����� �� ������ ���������� (�� ObjectDX)
                    vector<TMatrix16*>* matrix,// ���������� ��������� ������ ������ �������, ���-�� ��������� � cSubset (�� ObjectDX)
                    TMatrix16* pWorld,// ��� � ��� ���������� ������         (�� ObjectDX)
                    float alphaTransparency,
                    const TMatrix16* pView,/*��� ����������� ���������*/ // ������������ � ���������� ������    (�� ManagerDirectX)                   
                    void* pEffect)
{
  if(pEffect)
    DrawBy(pEffect,
           pVecTexCubeMap,    //                           (�� ObjectDX)
           state, // ����� ��������� ���������� (�� ObjectDX)
           mask,  // ����� �� ������ ���������� (�� ObjectDX)
           matrix,// ���������� ��������� ������ ������ �������, ���-�� ��������� � cSubset (�� ObjectDX)
           pWorld,// ��� � ��� ���������� ������         (�� ObjectDX)
           alphaTransparency,
           pView);/*��� ����������� ���������*/ // ������������ � ���������� ������    (�� ManagerDirectX)                   
  else
    DrawSelf(pVecTexCubeMap,    //
             state, // ����� ��������� ���������� 
             mask,  // ����� �� ������ ���������� 
             matrix,// ���������� ��������� ������ ������ �������, ���-�� ��������� � cSubset
             pWorld,// ��� � ��� ���������� ������ 
             alphaTransparency,
             pView);/*��� ����������� ���������*/ // ������������ � ���������� ������
}
//----------------------------------------------------------------------------------------------------
void TModelDX::DrawSelf(vector<void*>* pVecTexCubeMap,    //    
										    vector<unsigned char>* state, // ����� ��������� ���������� 
                        vector<unsigned char>* mask,  // ����� �� ������ ���������� 
                        vector<TMatrix16*>* matrix,// ���������� ��������� ������ ������ �������, ���-�� ��������� � cSubset
                        TMatrix16* pWorld,// ��� � ��� ���������� ������        
                        float alphaTransparency,
                        const TMatrix16* pView)/*��� ����������� ���������*/ // ������������ � ���������� ������
{
  D3DXMATRIXA16 mWorld;// ������ �� ������ �������
  MATRIX16_EQUAL_M_P(mWorld,pWorld)

  //1 ������� �� ���� mesh
  // ������ ����������
  float dist = CalcDist(pWorld,pView);
  //2 ��������� pEffect �� ���������� � ���������� - �������� � pEffect �������
  // ���� mArrMatrixSubset==NULL, �� ���������� � ���������� ������ ������ ���������
  //3 ������� �� ��������� subSet
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
      D3DXMATRIXA16 m;// ������ �� ������ �������
      TMatrix16* pV = matrix->operator [](iView);// ������ �� ������ �������
      MATRIX16_EQUAL_M_P(m,pV)

      D3DXMATRIXA16 mWorld_pro = m * mWorld;
      //-------------------------------------------------
      pEffect->SetAlphaTransparency(alphaTransparency);
      pEffect->SetInnerShaderParam();
      pEffect->SetMatrixWorld(&mWorld_pro);
			pEffect->SetTexCubeMap((IDirect3DCubeTexture9**)pVecTexCubeMap->at(i));
      SetShaderStack(pEffect,i);// ��� �����, ������� ���������
      Draw(pEffect, pEffect->GetMesh());
      iView++;
    }
  }
}
//---------------------------------------------------------------------------------------------------
void TModelDX::Draw( TEffectDX* pEffect, ID3DXMesh* pCurMesh)
{
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
bool TModelDX::Init(IDirect3DDevice9* pd3dDevice, LPCWSTR strAbsPath/*���� � ������ ������*/)
{
  Done();
  m_pd3dDevice = pd3dDevice;
  // �������� ������ ����������, ������� � ��������.
  GetLogger()->Get("GE")->WriteF_time("�������� Mesh, ID=%u\n",mID);
  if(Load(strAbsPath)==false) 
    return false;

  GetLogger()->Get("GE")->WriteF_time("����� �������� Mesh, ID=%u\n",mID);

  int cnt = mVectorAllEffect.size();
  // ��������������� ������
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
	LostCubeMap();
}
//----------------------------------------------------------------------------------------------------
void TModelDX::ResetDevice()
{
  int cnt = GetCntEffect();
  for(int i = 0 ; i < cnt ; i++)
    mVectorAllEffect[i]->ResetDevice();

	ResetCubeMap();
}
//----------------------------------------------------------------------------------------------------
bool TModelDX::Load(LPCWSTR strFilenameData)
{
  ILoaderModelGE* pLoadModel;
  TMakerLoaderModelGE makerLoaderModelGE;
  pLoadModel = makerLoaderModelGE.New(m_pd3dDevice);
  if(pLoadModel->Load((wchar_t*)strFilenameData)==false)
  {
    USES_CONVERSION;
    GetLogger()->Get("GE")->WriteF_time("�� ������� ��������� ������: %s.\n",W2A(strFilenameData));
    delete pLoadModel;
    return false;
  }
  //-------------------------------------------------------------------------------------------
  mLOD = pLoadModel->GetLOD();
  int cnt = pLoadModel->GetCountGroup();
  for(int i = 0 ; i < cnt ; i++)
  {
    ILoaderModelGE::TDefGroup *pDef = pLoadModel->GetGroup(i);
    // ����������
    AddEffectDX(pDef);
  }

  makerLoaderModelGE.Delete(pLoadModel);

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
  // ��������� �������� �������
	pMaterial->flgUseCubeMap = pDefGroup->flgUseCubeMap;
  pMaterial->strTechnique  = pDefGroup->strTechnique;
  pMaterial->strName       = pDefGroup->strName;
  pMaterial->mNumUse       = pDefGroup->mNumUse;
  pMaterial->strTexture    = pDefGroup->strTexture;
  USES_CONVERSION;
  pMaterial->strShader = A2W(pDefGroup->strPathShader.data());

  pMaterial->vAmbient   = D3DXVECTOR3(pDefGroup->vAmbient.x, pDefGroup->vAmbient.y, pDefGroup->vAmbient.z );
  pMaterial->vDiffuse   = D3DXVECTOR3(pDefGroup->vDiffuse.x, pDefGroup->vDiffuse.y, pDefGroup->vDiffuse.z );
  pMaterial->vSpecular  = D3DXVECTOR3(pDefGroup->vSpecular.x,pDefGroup->vSpecular.y,pDefGroup->vSpecular.z);

  pMaterial->nShininess = pDefGroup->nShininess;

  pMaterial->bSpecular  = pDefGroup->bSpecular;

  LoadTexture(pEffect);

  pEffect->SetMesh( (ID3DXMesh*)pDefGroup->pMesh );

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
  GetLogger()->Get("GE")->WriteF_time("�� ������ ������ ������ ��� %s ����� %d\n",sName,num);
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
  // ��������������� ������
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

  pEffectDX->SetEffect( (ID3DXEffect*)mManagerResourceDX->Load(pEffectDX->mMaterial.strShader.data()) );

  //start = ht_GetMSCount() - start;
  //GlobalLoggerGE.WriteF_time("�������� ������� t=%u\n",start);
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
    // ���������
    TSortByAlphabetic sort;
    sort.Sort(&vectorName_Use);
    // � ������������ � �������� �������� ��������� ������ ��������
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
void TModelDX::SetShaderStackMask(vector<int>* pVectorMask)// �� ���� ����� ������������� ������� ��������
{
  mVectorShaderStackMask = *pVectorMask;
}
//----------------------------------------------------------------------------------------------------
TShaderStack* TModelDX::GetShaderStack(int index)// ����� ���� ��� ���������
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
    mExecSS.Set(pEffect->GetEffect(),pSS);
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
      AddShaderStack(pEffectDX->GetEffect());
  }
}
//----------------------------------------------------------------------------------------------------
void TModelDX::AddShaderStack(ID3DXEffect* p)
{
  char pData[200];// ��� ��������� �������� ��������� ������
  TShaderStack* pSS = new TShaderStack;
  mVectorShaderStack.push_back(pSS);
  mVectorShaderStackMask.push_back(0);
  //----------------------------------------
  HRESULT hr;
  D3DXEFFECT_DESC pDesc;
  V( p->GetDesc(&pDesc) );

  bool flgBeginParam = false;
  int cnt = pDesc.Parameters;
  // ����� ������ output param
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
//������ IDirect3DCubeTexture9**
void* TModelDX::MakeTextureForCubeMap(int index)// ��� ��������� �������� - ���� ��������� ��� �������
{
	if(mVectorAllEffect[index]->UseCubeMap())
	{
		IDirect3DCubeTexture9* pTexCubeMap = NULL;
		// Create the cube textures
		HRESULT hr = m_pd3dDevice->CreateCubeTexture( IGraphicEngine::eEnvMapSizeCubemap,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A16B16G16R16F,
			D3DPOOL_DEFAULT,
			&pTexCubeMap,
			NULL );
		mListCubeMap.push_back(pTexCubeMap);
		return &mListCubeMap.front();
	}

	return NULL;
}
//----------------------------------------------------------------------------------------------------	
void TModelDX::LostCubeMap()
{
	list<IDirect3DCubeTexture9*>::iterator bit = mListCubeMap.begin();
	list<IDirect3DCubeTexture9*>::iterator eit = mListCubeMap.end();
	while(bit!=eit)
	{
		SAFE_RELEASE(*bit);
		bit++;
	}
}
//----------------------------------------------------------------------------------------------------	
void TModelDX::ResetCubeMap()
{
	list<IDirect3DCubeTexture9*>::iterator bit = mListCubeMap.begin();
	list<IDirect3DCubeTexture9*>::iterator eit = mListCubeMap.end();
	while(bit!=eit)
	{
		IDirect3DCubeTexture9* pTexCubeMap = *bit;
		HRESULT hr = m_pd3dDevice->CreateCubeTexture( IGraphicEngine::eEnvMapSizeCubemap,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A16B16G16R16F,
			D3DPOOL_DEFAULT,
			&pTexCubeMap,
			NULL );
		*bit = pTexCubeMap;
		bit++;
	}
}
//----------------------------------------------------------------------------------------------------	
void TModelDX::DrawBy(void* pVecEffect,// ���� ���� ������ �� ������, �� ����� ����� ���������� �� ������
                      vector<void*>* pVecTexCubeMap,    //                           
                      vector<unsigned char>* state,     //                           
                      vector<unsigned char>* mask,      //                           
                      vector<TMatrix16*>* matrix,//���-�� ��������� � cSubset 
                      TMatrix16* pWorld,    // ��� � ��� ���������� ������         
                      float alphaTransparency,  // ������������                        
                      const TMatrix16* pView)// ������������ � ���������� ������    
{
  TEffectDX* pEffectBy   = (TEffectDX*)pVecEffect;// ����� �������� ��������������� ��� �����
  //------------------------------------------------
  D3DXMATRIXA16 mWorld;// ������ �� ������ �������
  MATRIX16_EQUAL_M_P(mWorld,pWorld)

  //1 ������� �� ���� mesh
  // ������ ����������
  float dist = CalcDist(pWorld,pView);
  //2 ��������� pEffect �� ���������� � ���������� - �������� � pEffect �������
  // ���� mArrMatrixSubset==NULL, �� ���������� � ���������� ������ ������ ���������
  //3 ������� �� ��������� subSet
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
      TEffectDX* pEffectSelf = NULL;// ����� �������� ��������������� ��� �����
      if(pCurLOD->normal.size()&&pCurLOD->damage.size())
      {
        pEffectSelf = pCurLOD->normal[i];
        if(!state->operator[](i))
          pEffectSelf = pCurLOD->damage[i];
      }
      else 
      {
        if(pCurLOD->normal.size())
          pEffectSelf = pCurLOD->normal[i];
        else
          pEffectSelf = pCurLOD->damage[i];
      }
      //------------------------------------------
      D3DXMATRIXA16 m;// ������ �� ������ �������
      TMatrix16* pV = matrix->operator [](iView);// ������ �� ������ �������
      MATRIX16_EQUAL_M_P(m,pV)

      D3DXMATRIXA16 mWorld_pro = m * mWorld;
      //-------------------------------------------------
      pEffectBy->SetAlphaTransparency(alphaTransparency);
      pEffectBy->SetInnerShaderParam();
      pEffectBy->SetMatrixWorld(&mWorld_pro);
      pEffectBy->SetTexCubeMap((IDirect3DCubeTexture9**)pVecTexCubeMap->at(i));
      SetShaderStack(pEffectBy,i);// ��� �����, ������� ���������
      Draw(pEffectBy, pEffectSelf->GetMesh());
      iView++;
    }
  }
}
//----------------------------------------------------------------------------------------------------	
