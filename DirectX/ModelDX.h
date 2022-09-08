/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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