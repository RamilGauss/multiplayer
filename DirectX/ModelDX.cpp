#include "ModelDX.h"
#include "SDKmisc.h"
#include "LoaderModelDX.h"
#include "LoggerDX.h"
#include "LoaderModelDXEditor.h"
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
  mCntEffectInLOD   = 0;// ��� ������� � 2 ��� 1 ��� ������ ��� mCntEffectVisual
  mCntAllEffect    = 0;// ��� ������� � 4 ��� 2 ���� ������ ��� mCntEffectVisual
  mLOD             = 0;
  mArrEffect0       = NULL;
  mArrEffect1       = NULL;

  m_pd3dDevice = NULL;
}
//----------------------------------------------------------------------------------------------------
TModelDX::~TModelDX()
{
  delete []mArrEffect0;
  mArrEffect0       = NULL;
  delete []mArrEffect1;
  mArrEffect1       = NULL;
  mCntEffectVisual = 0;
  mCntEffectInLOD  = 0;// ��� ������� � 2 ��� 1 ��� ������ ��� mCntEffectVisual
  mCntAllEffect    = 0;// ��� ������� � 4 ��� 2 ���� ������ ��� mCntEffectVisual
}
//----------------------------------------------------------------------------------------------------
void TModelDX::Draw(unsigned int state,             //                              (�� ObjectDX)
                    D3DXMATRIXA16* mArrMatrixSubset,//���-�� ��������� � mCntEffect (�� ObjectDX)
                    D3DXMATRIXA16* mWorld,// ��� � ��� ���������� ������            (�� ObjectDX)
                    D3DXMATRIXA16* mView, // ������������ � ���������� ������       (�� ManagerDirectX)
                    D3DXMATRIXA16* mProj) // �������������� �� ��������� ������     (�� ManagerDirectX)
{
  mWorldViewProjection = (*mWorld) * (*mView) * (*mProj);
  //1 ������� �� ���� mesh
  // ������ ����������
  float dist = GetDist(mWorld,mView);
  //2 ��������� pEffect �� ���������� � ���������� - �������� � pEffect �������
  // ���� mArrMatrixSubset==NULL, �� ���������� � ���������� ������ ������ ���������
  //3 ������� �� ��������� subSet
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
    V( pCurMesh->DrawSubset( pEffect->mSubset ) );
    V( pEffect->EndPass() );
  }
  V( pEffect->End() );
}
//----------------------------------------------------------------------------------------------------
void TModelDX::Init(IDirect3DDevice9* pd3dDevice, LPCWSTR strAbsPath/*���� � ������ ������*/)
{
  m_pd3dDevice = pd3dDevice;
  // �������� ������ ����������, ������� � ��������.
  if(Load(strAbsPath)==false) 
    return;

  LPCWSTR strFilenameShader = NULL;
  HRESULT hr;
  WCHAR str[MAX_PATH];
  DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;

  // ��������� ������ �� �����
  V( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, strFilenameShader ) );

  for(int i = 0 ; i < mCntEffectVisual ; i++)
  {
    ID3DXEffect* pEffect;
    // If this fails, there should be debug output as to 
    // they the .fx file failed to compile
    V( D3DXCreateEffectFromFile( pd3dDevice, str, NULL, NULL, dwShaderFlags,
      NULL, &pEffect, NULL ) );
    mArrEffect0[i].mEffectDX_normal->p = pEffect;
    mArrEffect1[i].mEffectDX_normal->Init(i*4+0);
    //-----------------------------------------------------------------------------
    // If this fails, there should be debug output as to 
    // they the .fx file failed to compile
    V( D3DXCreateEffectFromFile( pd3dDevice, str, NULL, NULL, dwShaderFlags,
      NULL, &pEffect, NULL ) );
    mArrEffect0[i].mEffectDX_damage->p = pEffect;
    mArrEffect1[i].mEffectDX_damage->Init(i*4+1);
    //-----------------------------------------------------------------------------
    // If this fails, there should be debug output as to 
    // they the .fx file failed to compile
    V( D3DXCreateEffectFromFile( pd3dDevice, str, NULL, NULL, dwShaderFlags,
      NULL, &pEffect, NULL ) );
    mArrEffect0[i].mEffectDX_damage->p = pEffect;
    mArrEffect1[i].mEffectDX_damage->Init(i*4+2);
    //-----------------------------------------------------------------------------
    // If this fails, there should be debug output as to 
    // they the .fx file failed to compile
    V( D3DXCreateEffectFromFile( pd3dDevice, str, NULL, NULL, dwShaderFlags,
      NULL, &pEffect, NULL ) );
    mArrEffect0[i].mEffectDX_damage->p = pEffect;
    mArrEffect1[i].mEffectDX_damage->Init(i*4+3);
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
    mArrEffect0[i].mEffectDX_normal->Destroy();
    mArrEffect0[i].mEffectDX_damage->Destroy();
    mArrEffect1[i].mEffectDX_normal->Destroy();
    mArrEffect1[i].mEffectDX_damage->Destroy();
  }
}
//----------------------------------------------------------------------------------------------------
void TModelDX::LostDevice()
{
  for(int i = 0 ; i < mCntEffectVisual ; i++)
  {
    mArrEffect0[i].mEffectDX_normal->LostDevice();
    mArrEffect0[i].mEffectDX_damage->LostDevice();
    mArrEffect1[i].mEffectDX_normal->LostDevice();
    mArrEffect1[i].mEffectDX_damage->LostDevice();
  }
}
//----------------------------------------------------------------------------------------------------
void TModelDX::ResetDevice()
{
  for(int i = 0 ; i < mCntEffectVisual ; i++)
  {
    mArrEffect0[i].mEffectDX_normal->ResetDevice();
    mArrEffect0[i].mEffectDX_damage->ResetDevice();
    mArrEffect1[i].mEffectDX_normal->ResetDevice();
    mArrEffect1[i].mEffectDX_damage->ResetDevice();
  }
}
//----------------------------------------------------------------------------------------------------
bool TModelDX::Load(LPCWSTR strFilenameData)
{
  ILoaderModelDX* pLoadModel;
#ifdef _EDITOR_MODEL // �������� �������
  pLoadModel = new TLoaderModelDXEditor;//� �������� ��� �������������
#else
  pLoadModel = new TLoaderModelDX;
#endif
  if(pLoadModel->Load(strFilenameData)==false)
  {
    USES_CONVERSION;
    GlobalLoggerDX.WriteF_time("�� ������� ��������� ������ � HDD: %s.\n",W2A(strFilenameData));
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
  TEffectDX* pArrEffect = new TEffectDX[mCntAllEffect];
  for(int i = 0 ; i < mCntAllEffect ; i++)
  {
    ILoaderModelDX::TDefGroup *pDef = &(pLoadModel->GetArrDefGroup()[i]);
    // ����������
    SetupEffectDX(&pArrEffect[i],pDef);
  }
  // ��������������� ������
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
    if(pArrEffect[i].mTypeLOD==0)
      pCurLOD = mArrEffect0;
    if(pArrEffect[i].mflgNormal)
      pCurLOD[pArrEffect[i].mIndexVisual].mEffectDX_normal = &pArrEffect[i];
    else
      pCurLOD[pArrEffect[i].mIndexVisual].mEffectDX_damage = &pArrEffect[i];
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
  // ��������� �������� �������
  wcscpy(pMaterial->strName,pDefGroup->strName);
  wcscpy(pMaterial->strTexture,pDefGroup->strTexture);

  pMaterial->vAmbient   = pDefGroup->vAmbient;
  pMaterial->vDiffuse   = pDefGroup->vDiffuse;
  pMaterial->vSpecular  = pDefGroup->vSpecular;

  pMaterial->nShininess = pDefGroup->nShininess;
  pMaterial->fAlpha     = pDefGroup->fAlpha;

  pMaterial->bSpecular  = pDefGroup->bSpecular;

  V( D3DXCreateTextureFromFile( m_pd3dDevice, pMaterial->strTexture,
    &( pMaterial->pTexture ) ) );

  // Create the encapsulated mesh
  V( D3DXCreateMesh( pDefGroup->sizeIndexes / 3, pDefGroup->sizeVertex,
    D3DXMESH_MANAGED | D3DXMESH_32BIT, VERTEX_MODELDX_DECL,
    m_pd3dDevice, &(pEffect->pMesh) ) );

  // Copy the vertex data
  TEffectDX::VERTEX* pVertex;
  V( pEffect->pMesh->LockVertexBuffer( 0, ( void** )&pVertex ) );
  memcpy( pVertex, pDefGroup->vertex, pDefGroup->sizeVertex * sizeof( TEffectDX::VERTEX ) );
  pEffect->pMesh->UnlockVertexBuffer();

  // Copy the index data
  DWORD* pIndex;
  V( pEffect->pMesh->LockIndexBuffer( 0, ( void** )&pIndex ) );
  memcpy( pIndex, pDefGroup->indexes, pDefGroup->sizeIndexes * sizeof( DWORD ) );
  pEffect->pMesh->UnlockIndexBuffer();

  // ����������� ��������� ������
  DWORD* aAdjacency = new DWORD[pEffect->pMesh->GetNumFaces() * 3];
  if( aAdjacency == NULL )
  {
    GlobalLoggerDX.WriteF_time("�������� ������.SetupEffectDX().\n");
    return false;
  }

  V( pEffect->pMesh->GenerateAdjacency( 1e-6f, aAdjacency ) );
  V( pEffect->pMesh->OptimizeInplace( D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE, aAdjacency, NULL, NULL, NULL ) );

  SAFE_DELETE_ARRAY( aAdjacency );

  return true;
}
//----------------------------------------------------------------------------------------------------
