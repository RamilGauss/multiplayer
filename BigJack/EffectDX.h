/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
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


#ifndef EffectDXH
#define EffectDXH

#include <d3dx9effect.h>
#include <d3dx9mesh.h>

class TEffectDX
{
  //--------------------------------------------------------------------------------------
  // Effect parameter handles
  //--------------------------------------------------------------------------------------
  D3DXHANDLE  hAmbient;
  D3DXHANDLE  hDiffuse;
  D3DXHANDLE  hSpecular;
  D3DXHANDLE  hOpacity;
  D3DXHANDLE  hSpecularPower;
  D3DXHANDLE  hLightColor;
  D3DXHANDLE  hLightPosition;
  D3DXHANDLE  hCameraPosition;
  D3DXHANDLE  hTexture;
  D3DXHANDLE  hTime;
  D3DXHANDLE  hWorld;
  D3DXHANDLE  hWorldViewProjection;

public:

  ID3DXMesh* pMesh;

  ID3DXEffect* p;
  // Material format 
  struct Material
  {
    char   strName[MAX_PATH];

    D3DXVECTOR3 vAmbient;
    D3DXVECTOR3 vDiffuse;
    D3DXVECTOR3 vSpecular;

    int nShininess;
    float fAlpha;

    bool bSpecular;
    
    WCHAR   strShader[MAX_PATH];
    char    strTechnique[MAX_PATH];

    WCHAR   strTexture[MAX_PATH];
    IDirect3DTexture9* pTexture;
    D3DXHANDLE hTechnique;
  };
  // Vertex format
#pragma pack(push, 1)
  struct VERTEX
  {
    D3DXVECTOR3 position;
    D3DXVECTOR3 normal;
    D3DXVECTOR2 texcoord;
  };
#pragma pack(pop)
  TEffectDX();
  ~TEffectDX();

  void Init(int subset);
  void Destroy();
  void LostDevice();
  void ResetDevice();

  HRESULT SetMatrixWorld(D3DXMATRIXA16* matrix);
  HRESULT SetMatrixWorldViewProjection(D3DXMATRIXA16* matrix);
  HRESULT Begin(UINT* cPasses , DWORD flag);
  HRESULT BeginPass(UINT iPass);
  HRESULT EndPass();
  HRESULT End();

  Material      mMaterial;

  unsigned char mTypeLOD;
  bool          mflgNormal;

  int           mSubset;// номер в структуре TEffectDX
  unsigned char mIndexVisual;// индекс при отрисовке, для определения по состоянию

  bool isN0();
  bool isN1();
  bool isD0();
  bool isD1();
};


#endif