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


#ifndef ILoaderObjectDXH
#define ILoaderObjectDXH

#include "Struct3D.h"
#include <wtypes.h>
#include "EffectDX.h"
#include <d3dx9math.h>
#include "BL_ConfigFile.h"
#include <d3d9.h>

class ILoaderModelDX
{
protected:
  IDirect3DDevice9* m_pd3dDevice;
public:

  struct TDefGroup
  {
    unsigned char      mIndex;
    char   strPathShader[MAX_PATH];// ������������� ����
    char   strTechnique[MAX_PATH];
    WCHAR  strTexture[MAX_PATH];
    char   strName[MAX_PATH];

    ID3DXMesh* pMesh;
    //DWORD* indexes;
    //TEffectDX::VERTEX* vertex;
    //DWORD  cntIndexes;// ������ �� �����
    //DWORD  cntVertex;// ������ �� �����

    D3DXVECTOR3 vAmbient;
    D3DXVECTOR3 vDiffuse;
    D3DXVECTOR3 vSpecular;

    int   nShininess;
    float fAlpha;

    bool bSpecular;

    unsigned char mTypeLOD;
    bool          mflgNormal;

    TDefGroup()
    {
      mIndex = 0;
      strTexture[0]    = '\0';
      strPathShader[0] = '\0';
      strTechnique[0]  = '\0';
      strName[0]       = '\0';
      pMesh            = NULL;
      //indexes = NULL;
      //vertex = NULL;
      //cntVertex = 0;
      //cntIndexes = 0;

      vAmbient  = D3DXVECTOR3(0,0,0);
      vDiffuse  = D3DXVECTOR3(0,0,0);
      vSpecular = D3DXVECTOR3(0,0,0);

      nShininess = 0;
      fAlpha     = 0.0f;

      bSpecular  = false;

      mTypeLOD   = 0;
      mflgNormal = true;
    }
  };
public:
  int mCntEffectVisual;// ������ ����� ��������
  int mCntEffectInLOD;
protected:
  float mLOD;
  unsigned short mID_unique;
  int mCntGroup;
  TDefGroup* mArrDefGroup;

public:
  enum{
    eUndefID = -1,
  };

  ILoaderModelDX( IDirect3DDevice9* m_pd3dDevice );
  virtual ~ILoaderModelDX();

  TDefGroup* GetArrDefGroup(){return mArrDefGroup;}
  // ����� �������� ���������� ����������
  float GetLOD();
  unsigned short GetID_Unique();// ���������� ��� ������� ������, ���������� � ����� �����
  int GetCountSubset();
  //DWORD* GetPinterIndexes(int iGroup, int &size);
  //TEffectDX::VERTEX* GetPinterVertex(int iGroup, int &size);
  WCHAR* GetTexture(int iGroup, int &size);
  char* GetStrPathShader(int iGroup);

public:
  // virtual
  virtual bool Load(LPCWSTR strFilenameData) = 0;

protected:
  
  void Done();

  void UpPath(char* sPath);

protected:

  TBL_ConfigFile mFileIniMain;
  TBL_ConfigFile mFileIniRes;

  char pStrPathShader[MAX_PATH];

  char pStrFilenameData[MAX_PATH];
  char pStrPathPrimitive[MAX_PATH];
  char pStrFilenameDataMainIni[MAX_PATH];
};


#endif