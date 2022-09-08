#include "ManagerModel.h"


TManagerModel GlobalManagerModel;

TManagerModel::TManagerModel()
{

}
//--------------------------------------------------------------------------------------
TManagerModel::~TManagerModel()
{

}
//--------------------------------------------------------------------------------------
HRESULT TManagerModel::Load(IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
             void* pUserContext )
{
  V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"..\\Shader\\SimpleSample.fx" ) );
  V_RETURN( D3DXCreateEffectFromFile( pd3dDevice, str, NULL, NULL, dwShaderFlags,
    NULL, &mEffect, NULL ) );

  hmWorldViewProjection = mEffect->GetParameterByName( NULL, "g_mWorldViewProjection" );
  hmWorld = mEffect->GetParameterByName( NULL, "g_mWorld" );
  hfTime  = mEffect->GetParameterByName( NULL, "g_fTime" );

}
//--------------------------------------------------------------------------------------

