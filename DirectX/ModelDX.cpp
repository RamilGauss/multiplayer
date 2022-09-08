#include "ModelDX.h"
#include "SDKmisc.h"
#include "LoaderModelDX.h"
#include "LogerDX.h"

using namespace nsStruct3D;

TModelDX::TModelDX()
{
  mID = -1;
  mCntEffectVisual = 0;
  mCntEffectMesh   = 0;// как правило в 2 или 1 раз больше чем mCntEffectVisual
  mCntAllEffect    = 0;// как правило в 4 или 2 раза больше чем mCntEffectVisual
  mLOD             = 0;
  mArrEffect       = NULL;
}
//----------------------------------------------------------------------------------------------------
TModelDX::~TModelDX()
{
  delete []mArrEffect;
  mArrEffect       = NULL;
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
  unsigned char lod = 0;
  pCurMesh = NULL;
  //1 Выбрать по ЛОДу mesh
  // расчет расстояния
  float dist = GetDist(mWorld,mView);
  if(dist>mLOD)
    pCurMesh = pArrMesh[0];
  else
  {
    pCurMesh = pArrMesh[1];
    lod = 1;
  }
  //2 Настроить pEffect на координаты и ориентацию - передать в pEffect матрицу
  // если mArrMatrixSubset==NULL, то координаты и ориентация частей модели неизменны
  //3 Выбрать по состоянию subSet
  for(int i = 0 ; i < mCntEffectVisual ; i++)
  {
    TEffectDX* pEffect;
    int subset;
    if(lod)
      if(state&(i>>1))
        pEffect = &mArrEffect[i].lod0.mArrEffectDX_damage;
      else
        pEffect = &mArrEffect[i].lod0.mArrEffectDX_normal;
    else
      if(state&(i>>1))
        pEffect = &mArrEffect[i].lod1.mArrEffectDX_damage;
      else
        pEffect = &mArrEffect[i].lod1.mArrEffectDX_normal;

    pEffect->SetMatrixWorld(&mArrMatrixSubset[i]);
    Draw(pEffect);
  }
}
//---------------------------------------------------------------------------------------------------
void TModelDX::Draw( TEffectDX* pEffect)
{
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
  if(Load(strPath)==false) 
  {
    GlobalLoggerDX.WriteF_time("Не удалось проинициализировать модель: %s.\n",strPath);
    return;// загрузка данных примитивов, текстур и индексов.
  }

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
    mArrEffect[i].lod0.mArrEffectDX_normal.p = pEffect;
    mArrEffect[i].lod0.mArrEffectDX_normal.Init(i*4+0);
    //-----------------------------------------------------------------------------
    // If this fails, there should be debug output as to 
    // they the .fx file failed to compile
    V( D3DXCreateEffectFromFile( pd3dDevice, str, NULL, NULL, dwShaderFlags,
      NULL, &pEffect, NULL ) );
    mArrEffect[i].lod0.mArrEffectDX_damage.p = pEffect;
    mArrEffect[i].lod0.mArrEffectDX_damage.Init(i*4+1);
    //-----------------------------------------------------------------------------
    // If this fails, there should be debug output as to 
    // they the .fx file failed to compile
    V( D3DXCreateEffectFromFile( pd3dDevice, str, NULL, NULL, dwShaderFlags,
      NULL, &pEffect, NULL ) );
    mArrEffect[i].lod1.mArrEffectDX_normal.p = pEffect;
    mArrEffect[i].lod1.mArrEffectDX_normal.Init(i*4+2);
    //-----------------------------------------------------------------------------
    // If this fails, there should be debug output as to 
    // they the .fx file failed to compile
    V( D3DXCreateEffectFromFile( pd3dDevice, str, NULL, NULL, dwShaderFlags,
      NULL, &pEffect, NULL ) );
    mArrEffect[i].lod1.mArrEffectDX_damage.p = pEffect;
    mArrEffect[i].lod1.mArrEffectDX_damage.Init(i*4+3);
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
    mArrEffect[i].lod0.mArrEffectDX_normal.Destroy();
    mArrEffect[i].lod0.mArrEffectDX_damage.Destroy();
    mArrEffect[i].lod1.mArrEffectDX_normal.Destroy();
    mArrEffect[i].lod1.mArrEffectDX_damage.Destroy();
  }
}
//----------------------------------------------------------------------------------------------------
void TModelDX::LostDevice()
{
  for(int i = 0 ; i < mCntEffectVisual ; i++)
  {
    mArrEffect[i].lod0.mArrEffectDX_normal.LostDevice();
    mArrEffect[i].lod0.mArrEffectDX_damage.LostDevice();
    mArrEffect[i].lod1.mArrEffectDX_normal.LostDevice();
    mArrEffect[i].lod1.mArrEffectDX_damage.LostDevice();
  }
}
//----------------------------------------------------------------------------------------------------
void TModelDX::ResetDevice()
{
  for(int i = 0 ; i < mCntEffectVisual ; i++)
  {
    mArrEffect[i].lod0.mArrEffectDX_normal.ResetDevice();
    mArrEffect[i].lod0.mArrEffectDX_damage.ResetDevice();
    mArrEffect[i].lod1.mArrEffectDX_normal.ResetDevice();
    mArrEffect[i].lod1.mArrEffectDX_damage.ResetDevice();
  }
}
//----------------------------------------------------------------------------------------------------
bool TModelDX::Load(LPCWSTR strFilenameData)
{
  TLoaderModelDX loadObj;
  if(loadObj.Load(strFilenameData)==false)
  {
    GlobalLoggerDX.WriteF_time("Не удалось загрузить модель %s.\n",strFilenameData);
    return false;
  }

  mCntEffectVisual = 0;
  return true;
}
//----------------------------------------------------------------------------------------------------
