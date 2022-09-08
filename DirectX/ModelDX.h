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
  bool SetupEffectDX(TEffectDX* pEffect,ILoaderModelDX::TDefGroup* pDefGroup);
  float GetDist(D3DXMATRIXA16* mWorld, D3DXMATRIXA16* mView);
  void Draw(TEffectDX* pEffect);
  virtual bool Load(LPCWSTR strFilenameData);
  //---------------------------------------------------------
  friend class TManagerModel;
  
  // ���� � ������� ���� ���������, �� mEffectDX_normal==mEffectDX_damage
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

  TLOD* mArrEffect0;// ��������
  TLOD* mArrEffect1;// �����

  int mCntEffectVisual;// ���-�� �������� ����� ���-�� ������������ ����������
  int mCntEffectInLOD; // ��� ������� � 2 ��� 1 ��� ������ ��� mCntEffectVisual
  int mCntAllEffect;   // ��� ������� � 4 ��� 2 ���� ������ ��� mCntEffectVisual
 
  D3DXMATRIXA16 mWorldViewProjection;

  float mLOD;// 2 ��������� �� ����, ���������� �� ������ �� ������ ���������
};
//-----------------------------------------------------------------



#endif