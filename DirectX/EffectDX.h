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
  struct VERTEX
  {
    D3DXVECTOR3 position;
    D3DXVECTOR3 normal;
    D3DXVECTOR2 texcoord;
  };

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