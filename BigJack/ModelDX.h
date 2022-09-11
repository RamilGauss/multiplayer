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


#ifndef ModelDXH
#define ModelDXH

#include "DXUT.h"

#include <d3dx9math.h>
#include <d3d9types.h>
#include <vector>
#include <map>
#include <list>

#include "Struct3D.h"
#include "EffectDX.h"
#include "ILoaderModelGE.h"
#include "ShaderStack.h"
#include "ExecShaderStack.h"
#include "IModelGE.h"


class TManagerResourceDX;

class TModelDX : public IModelGE
{
protected:

  TManagerResourceDX* mManagerResourceDX;

  IDirect3DDevice9* m_pd3dDevice;    // Direct3D Device object associated with this mesh

  // ������ ManagerModelDX
  unsigned int mID; // ���������� ��� �������

  std::vector<TShaderStack*> mVectorShaderStack;
  std::vector<int>           mVectorShaderStackMask;
  TExecShaderStack           mExecSS;

	typedef std::list<IDirect3DCubeTexture9*> TListPtr;
	TListPtr mListCubeMap;

public:
  
  TModelDX(TManagerResourceDX* managerResourceDX);
  virtual ~TModelDX();
  virtual unsigned int GetID(){return mID;}
  virtual void SetID(unsigned int id){mID=id;}

  virtual int GetCntEffect();

  virtual void Draw(std::vector<void*>* pVecTexCubeMap,//                           
                    std::vector<unsigned char>* state, //                           
                    std::vector<unsigned char>* mask,  //                           
                    std::vector<nsStruct3D::TMatrix16*>* matrix,//���-�� ��������� � cSubset 
                    nsStruct3D::TMatrix16* pWorld,    // ��� � ��� ���������� ������         
                    float alphaTransparency,  // ������������                       
                    const nsStruct3D::TMatrix16* pView, // ������������ � ���������� ������    
                    void* pEffect = NULL);


  bool Init(IDirect3DDevice9* pd3dDevice, LPCWSTR strPath);
  void Destroy();
  void LostDevice();
  void ResetDevice();
  virtual void SortPartByAlphabetic();

  virtual unsigned int GetIndexVisualGroupByName(char* sName, int num);
  virtual const char* GetNameByIndex(int index);
  virtual int GetNumUseByIndex(int index);

  
  virtual void SetShaderStackMask(std::vector<int>* pVectorMask);// �� ���� ����� ������������� ������� ��������
  virtual TShaderStack* GetShaderStack(int index);// ����� ���� ��� ���������

	virtual void* MakeTextureForCubeMap(int index);// ��� ��������� �������� - ���� ��������� ��� �������

protected:
  void Done();
  //---------------------------------------------------------
  void SetupVectorLOD();
  bool AddEffectDX(ILoaderModelGE::TDefGroup* pDefGroup);
  void Draw(TEffectDX* pEffect, ID3DXMesh* pCurMesh);

  virtual bool Load(LPCWSTR strFilenameData);
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
  void LoadTexture(TEffectDX* pEffectDX);
  void LoadEffect(TEffectDX* pEffectDX);


  void SetupShaderStack();
  void SetShaderStack(TEffectDX* pEffect,int index);
  void AddShaderStack(ID3DXEffect* p);

	void LostCubeMap();
	void ResetCubeMap();

  // ������������ ���� ������
  void DrawSelf(std::vector<void*>* pVecTexCubeMap,//                           
						    std::vector<unsigned char>* state, //                           
                std::vector<unsigned char>* mask,  //                           
                std::vector<nsStruct3D::TMatrix16*>* matrix,//���-�� ��������� � cSubset 
                nsStruct3D::TMatrix16* pWorld,    // ��� � ��� ���������� ������         
                float alphaTransparency,  // ������������                       
                const nsStruct3D::TMatrix16* pView); // ������������ � ���������� ������    

  // ��������� ������ ��� ��������� (������ ������ ��� �������� ��������)
  void DrawBy(void* pVecEffect,// ���� ���� ������ �� ������, �� ����� ����� ���������� �� ������
              std::vector<void*>* pVecTexCubeMap,    //                           
              std::vector<unsigned char>* state,     //                           
              std::vector<unsigned char>* mask,      //                           
              std::vector<nsStruct3D::TMatrix16*>* matrix,//���-�� ��������� � cSubset 
              nsStruct3D::TMatrix16* pWorld,    // ��� � ��� ���������� ������         
              float alphaTransparency,  // ������������                        
              const nsStruct3D::TMatrix16* pView); // ������������ � ���������� ������    
};
//-----------------------------------------------------------------



#endif