#ifndef ModelDXH
#define ModelDXH

#include "Struct3D.h"

#include "DXUT.h"
#include "EffectDX.h"
#include "TObject.h"
#include <d3dx9math.h>

class TManagerModel;

class TModelDX : public TObject
{
  unsigned int mID; // уникальный для моделей

public:
  
  TModelDX();
  ~TModelDX();
  unsigned int GetID(){return mID;}
  void SetID(unsigned int id){mID=id;}

  void TModelDX::Draw(unsigned int state,             //                           (От ObjectDX)
    D3DXMATRIXA16* mArrMatrixSubset,//кол-во совпадает с cSubset (От ObjectDX)
    D3DXMATRIXA16* mWorld,// где и как расположен объект         (От ObjectDX)
    D3DXMATRIXA16* mView, // расположение и ориентация камеры    (от ManagerDirectX)
    D3DXMATRIXA16* mProj); // проектирование на плоскость экрана  (от ManagerDirectX)

  void Init(IDirect3DDevice9* pd3dDevice, LPCWSTR strPath);
  void Destroy();
  void LostDevice();
  void ResetDevice();

  int GetCntEffect(){return mCntEffectVisual;}// снаружи знают только о видимых

protected:

  //---------------------------------------------------------
  void Draw(TEffectDX* pEffect);
  float GetDist(D3DXMATRIXA16* mWorld, D3DXMATRIXA16* mView);
  bool Load(LPCWSTR strFilenameData);
  //---------------------------------------------------------
  friend class TManagerModel;
  
  float mLOD;// 2 состояния по ЛОДу, расстояние от камеры до центра координат

  ID3DXMesh* pArrMesh[2];// на два ЛОДа
  ID3DXMesh* pCurMesh;


  // 0 LOD
  struct TLOD
  {
    TEffectDX mArrEffectDX_normal;
    TEffectDX mArrEffectDX_damage;
  };

  struct TDefEffect
  {
    // 0 LOD
    TLOD lod0;
    // 1 LOD
    TLOD lod1;
  };
  TDefEffect* mArrEffect;

  int mCntEffectVisual;// кол-во эффектов равно кол-ву примитивов в Mesh
  int mCntEffectMesh;// как правило в 2 или 1 раз больше чем mCntEffectVisual
  int mCntAllEffect;// как правило в 4 или 2 раза больше чем mCntEffectVisual
 


  D3DXMATRIXA16 mWorldViewProjection;
};
//-----------------------------------------------------------------



#endif