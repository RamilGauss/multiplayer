/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
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