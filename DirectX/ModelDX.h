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
  unsigned int mID; // ���������� ��� �������

public:
  
  TModelDX();
  ~TModelDX();
  unsigned int GetID(){return mID;}
  void SetID(unsigned int id){mID=id;}

  void TModelDX::Draw(unsigned int state,             //                           (�� ObjectDX)
    D3DXMATRIXA16* mArrMatrixSubset,//���-�� ��������� � cSubset (�� ObjectDX)
    D3DXMATRIXA16* mWorld,// ��� � ��� ���������� ������         (�� ObjectDX)
    D3DXMATRIXA16* mView, // ������������ � ���������� ������    (�� ManagerDirectX)
    D3DXMATRIXA16* mProj); // �������������� �� ��������� ������  (�� ManagerDirectX)

  void Init(IDirect3DDevice9* pd3dDevice, LPCWSTR strPath);
  void Destroy();
  void LostDevice();
  void ResetDevice();

  int GetCntEffect(){return mCntEffectVisual;}// ������� ����� ������ � �������

protected:

  //---------------------------------------------------------
  void Draw(TEffectDX* pEffect);
  float GetDist(D3DXMATRIXA16* mWorld, D3DXMATRIXA16* mView);
  bool Load(LPCWSTR strFilenameData);
  //---------------------------------------------------------
  friend class TManagerModel;
  
  float mLOD;// 2 ��������� �� ����, ���������� �� ������ �� ������ ���������

  ID3DXMesh* pArrMesh[2];// �� ��� ����
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

  int mCntEffectVisual;// ���-�� �������� ����� ���-�� ���������� � Mesh
  int mCntEffectMesh;// ��� ������� � 2 ��� 1 ��� ������ ��� mCntEffectVisual
  int mCntAllEffect;// ��� ������� � 4 ��� 2 ���� ������ ��� mCntEffectVisual
 


  D3DXMATRIXA16 mWorldViewProjection;
};
//-----------------------------------------------------------------



#endif