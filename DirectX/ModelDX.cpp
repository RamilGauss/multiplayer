#include "ModelDX.h"
#include "SDKmisc.h"
#include "LoaderModelDX.h"
#include "LoggerDX.h"
#include "LoaderModelDXEditor.h"



// Vertex declaration
D3DVERTEXELEMENT9 VERTEX_DECL[] =
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
  mCntEffectMesh   = 0;// как правило в 2 или 1 раз больше чем mCntEffectVisual
  mCntAllEffect    = 0;// как правило в 4 или 2 раза больше чем mCntEffectVisual
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
  mCntEffectMesh   = 0;// как правило в 2 или 1 раз больше чем mCntEffectVisual
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
    V( pCurMesh->DrawSubset( pEffect->mSubset ) );
    V( pEffect->EndPass() );
  }
  V( pEffect->End() );
}
//----------------------------------------------------------------------------------------------------
void TModelDX::Init(IDirect3DDevice9* pd3dDevice, LPCWSTR strPath/*путь к файлам модели*/)
{
  m_pd3dDevice = pd3dDevice;
  // загрузка данных примитивов, текстур и индексов.
  if(Load(strPath)==false) 
    return;

  LPCWSTR strFilenameShader = NULL;
  HRESULT hr;
  WCHAR str[MAX_PATH];
  DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;

  // прочитать эффект из файла
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
#ifdef _EDITOR_MODEL // редактор моделей
  pLoadModel = new TLoaderModelDXEditor;//в основном для экспериментов
#else
  pLoadModel = new TLoaderModelDX;
#endif
  if(pLoadModel->Load(strFilenameData)==false)
  {
    USES_CONVERSION;
    GlobalLoggerDX.WriteF_time("Не удалось загрузить модель с HDD: %s.\n",W2A(strFilenameData));
    delete pLoadModel;
    return false;
  }
  //-------------------------------------------------------------------------------------------
  mLOD = pLoadModel->GetLOD();
  mID  = pLoadModel->GetID_Unique();
  mCntAllEffect = pLoadModel->GetCountSubset();
  TEffectDX* pArrEffect = new TEffectDX[mCntAllEffect];
  for(int i = 0 ; i < mCntAllEffect ; i++)
  {
    ILoaderModelDX::TDefGroup *pDef = &(pLoadModel->GetArrDefGroup()[i]);
    // заполнение
    SetupEffectDX(&pArrEffect[i],pDef);
  }
  // раскидать по группам:
  // кому принадлежит
  // normal состояние 0 ЛОДа
  //if(pDef->)
  // damage состояние 0 ЛОДа

  // normal состояние 1 ЛОДа

  // damage состояние 1 ЛОДа


  delete pLoadModel;
  return true;
}
//----------------------------------------------------------------------------------------------------
bool TModelDX::SetupEffectDX(TEffectDX *pEffect,ILoaderModelDX::TDefGroup * pDefGroup)
{
  HRESULT hr;
  ID3DXMesh* pMesh = pEffect->pMesh;
  // Load material textures
 /* for( int iMaterial = 0; iMaterial < m_Materials.GetSize(); iMaterial++ )
  {
    TEffectDX::Material* pMaterial = m_Materials.GetAt( iMaterial );
    if( pMaterial->strTexture[0] )
    {
      // Avoid loading the same texture twice
      bool bFound = false;
      for( int x = 0; x < iMaterial; x++ )
      {
        TEffectDX::Material* pCur = m_Materials.GetAt( x );
        if( 0 == wcscmp( pCur->strTexture, pMaterial->strTexture ) )
        {
          bFound = true;
          pMaterial->pTexture = pCur->pTexture;
          break;
        }
      }

      // Not found, load the texture
      if( !bFound )
      {
        V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, pMaterial->strTexture ) );
        V_RETURN( D3DXCreateTextureFromFile( pd3dDevice, pMaterial->strTexture,
          &( pMaterial->pTexture ) ) );
      }
    }
  }*/
  // Create the encapsulated mesh
  pMesh = NULL;
  V( D3DXCreateMesh( pDefGroup->sizeIndexes / 3, pDefGroup->sizeVertex,
    D3DXMESH_MANAGED | D3DXMESH_32BIT, VERTEX_DECL,
    m_pd3dDevice, &pMesh ) );

  // Copy the vertex data
  TEffectDX::VERTEX* pVertex;
  V( pMesh->LockVertexBuffer( 0, ( void** )&pVertex ) );
  memcpy( pVertex, pDefGroup->vertex, pDefGroup->sizeVertex * sizeof( TEffectDX::VERTEX ) );
  pMesh->UnlockVertexBuffer();

  // Copy the index data
  DWORD* pIndex;
  V( pMesh->LockIndexBuffer( 0, ( void** )&pIndex ) );
  memcpy( pIndex, pDefGroup->indexes, pDefGroup->sizeIndexes * sizeof( DWORD ) );
  pMesh->UnlockIndexBuffer();

  // Copy the attribute data
/*  DWORD* pSubset;
  V_RETURN( pMesh->LockAttributeBuffer( 0, &pSubset ) );
  memcpy( pSubset, m_Attributes.GetData(), m_Attributes.GetSize() * sizeof( DWORD ) );
  pMesh->UnlockAttributeBuffer();
  m_Attributes.RemoveAll();

  надо глянуть в MSDN что это за хрень
*/
  // Reorder the vertices according to subset and optimize the mesh for this graphics 
  // card's vertex cache. When rendering the mesh's triangle list the vertices will 
  // cache hit more often so it won't have to re-execute the vertex shader.
  DWORD* aAdjacency = new DWORD[pMesh->GetNumFaces() * 3];
  if( aAdjacency == NULL )
  {
    GlobalLoggerDX.WriteF_time("Нехватка памяти.SetupEffectDX().\n");
    return false;
  }

  V( pMesh->GenerateAdjacency( 1e-6f, aAdjacency ) );
  V( pMesh->OptimizeInplace( D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE, aAdjacency, NULL, NULL, NULL ) );

  SAFE_DELETE_ARRAY( aAdjacency );

  return true;
}
//----------------------------------------------------------------------------------------------------
