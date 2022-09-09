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

#include "DXUT.h"
#include "Struct3D.h"

#include "EffectDX.h"
#include "TObject.h"
#include <d3dx9math.h>
#include "ILoaderModelDX.h"
#include <d3d9types.h>
#include <vector>
#include <set>

class TManagerModelDX;

class TModelDX : public TObject
{
protected:

  IDirect3DDevice9* m_pd3dDevice;    // Direct3D Device object associated with this mesh

  // ������ ManagerModelDX
  unsigned int mID; // ���������� ��� �������


  // ������ ��������� � ������

public:
  
  TModelDX();
  ~TModelDX();
  unsigned int GetID(){return mID;}
  void SetID(unsigned int id){mID=id;}

  int GetCntEffect();

  /*
    ������ ����� ������
    � ����������� � ���������� ������
    � ������� �����������
  */
  void TModelDX::Draw(std::vector<unsigned char>* state, //                           (�� ObjectDX)
                      std::vector<unsigned char>* mask,  //                           (�� ObjectDX)
                      D3DXMATRIXA16* matrix,//���-�� ��������� � cSubset (�� ObjectDX)
                      D3DXMATRIXA16* mWorld,// ��� � ��� ���������� ������         (�� ObjectDX)
                      D3DXMATRIXA16* mView, // ������������ � ���������� ������    (�� ManagerDirectX)
                      D3DXMATRIXA16* mProj, // ������������� �� ��������� ������  (�� ManagerDirectX)
                      const D3DXVECTOR3* mCamera);
  bool Init(IDirect3DDevice9* pd3dDevice, LPCWSTR strPath);
  void Destroy();
  void LostDevice();
  void ResetDevice();

  // ������ ����� ���������� �����
  // ��� ������� ��������� ��� num-��� ���
  // ���� �� ����� ������� ������ 0 � ���������� BL_FIX_BUG()
  unsigned int GetIndexVisualGroupByName(char* sName, int num/*��������,10 � ����� �� ������*/);

  // ��� ������������ � blendBone part
  D3DXMATRIXA16* GetMatrixByName(char* sNameBlendBone/*� ���*/, int num, char* sNamePart/*���*/);

protected:

  //---------------------------------------------------------
  void SetupVectorLOD();
  bool AddEffectDX(ILoaderModelDX::TDefGroup* pDefGroup);
  float GetDist(D3DXMATRIXA16* mWorld, D3DXMATRIXA16* mView);
  void Draw(TEffectDX* pEffect,D3DXMATRIXA16& mWorldViewProjection,const D3DXVECTOR3* mCamera);

  virtual bool Load(LPCWSTR strFilenameData);
  void LoadTexture(TEffectDX::Material* pMaterial);
  //---------------------------------------------------------
  
  struct TLOD
  {
    std::vector<TEffectDX*>* GetNonZeroVector(){if(normal.size()) return &normal;return &damage;}
    // ���������� �������
    std::vector<TEffectDX*> normal;
    std::vector<TEffectDX*> damage;
    TLOD(){};
    ~TLOD()
    {
      normal.clear();
      damage.clear();
    }
  };

  // ��������������� ����� ��������
  std::vector<TLOD> mVectorLOD;

  // ��������� �� ����������
  std::vector<TEffectDX*> mVectorAllEffect;// ��� ���������, ����, ���� �����, ����� � �.�.
  float mLOD;// 2 ��������� �� ����, ���������� �� ������ �� ������ ���������


  // ��� ����������� ��������, ����� ��������� ��������� �������� �������
  std::set<std::wstring> mSetPathTexture;
};
//-----------------------------------------------------------------



#endif