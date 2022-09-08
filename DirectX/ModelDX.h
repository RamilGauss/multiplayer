#ifndef ModelDXH
#define ModelDXH

#include "DefineUnicode.h"
#include "Struct3D.h"

#include "DXUT.h"
#include "EffectDX.h"
#include "TObject.h"
#include <d3dx9math.h>
#include "ILoaderModelDX.h"
#include <d3d9types.h>

class TManagerModel;

class TModelDX : public TObject
{
protected:

  IDirect3DDevice9* m_pd3dDevice;    // Direct3D Device object associated with this mesh

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
  bool SetupEffectDX(TEffectDX* pEffect,ILoaderModelDX::TDefGroup* pDefGroup);
  float GetDist(D3DXMATRIXA16* mWorld, D3DXMATRIXA16* mView);
  void Draw(TEffectDX* pEffect);
  virtual bool Load(LPCWSTR strFilenameData);
  //---------------------------------------------------------
  friend class TManagerModel;
  
  // если у объекта одно состояние, то mEffectDX_normal==mEffectDX_damage
  struct TLOD
  {
    TEffectDX* mEffectDX_normal;
    TEffectDX* mEffectDX_damage;
    TLOD(){mEffectDX_normal=NULL;mEffectDX_damage=NULL;}
    ~TLOD()
    {
      mEffectDX_normal = NULL;
      mEffectDX_damage = NULL;
    }
  };

  TEffectDX* pArrAllEffect;

  TLOD* mArrEffect0;// подробно
  TLOD* mArrEffect1;// грубо

  int mCntEffectVisual;// кол-во эффектов равно кол-ву отображаемых примитивов
  int mCntEffectInLOD; // как правило в 2 или 1 раз больше чем mCntEffectVisual
  int mCntAllEffect;   // как правило в 4 или 2 раза больше чем mCntEffectVisual
 
  D3DXMATRIXA16 mWorldViewProjection;

  float mLOD;// 2 состояния по ЛОДу, расстояние от камеры до центра координат
};
//-----------------------------------------------------------------



#endif