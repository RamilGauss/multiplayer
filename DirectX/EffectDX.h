#ifndef EffectDXH
#define EffectDXH

#include <d3dx9effect.h>

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

  ID3DXEffect* p;
  struct Material
  {
    WCHAR   strName[MAX_PATH];

    D3DXVECTOR3 vAmbient;
    D3DXVECTOR3 vDiffuse;
    D3DXVECTOR3 vSpecular;

    int nShininess;
    float fAlpha;

    bool bSpecular;

    WCHAR   strTexture[MAX_PATH];
    IDirect3DTexture9* pTexture;
    D3DXHANDLE hTechnique;
  };


  TEffectDX();
  virtual ~TEffectDX();

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


  Material mMaterial;

  int mSubset;

};


#endif