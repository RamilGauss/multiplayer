#include "EffectDX.h"
#include "DXUT.h"


TEffectDX::TEffectDX()
{
  hAmbient = NULL;
  hDiffuse = NULL;
  hSpecular = NULL;
  hOpacity = NULL;
  hSpecularPower = NULL;
  hLightColor = NULL;
  hLightPosition = NULL;
  hCameraPosition = NULL;
  hTexture = NULL;
  hTime = NULL;
  hWorld = NULL;
  hWorldViewProjection = NULL;
}
//-----------------------------------------------------------
TEffectDX::~TEffectDX()
{
}
//-----------------------------------------------------------
void TEffectDX::Init(int subset)
{
  HRESULT hr;
  // Cache the effect handles
  hAmbient = p->GetParameterBySemantic( 0, "Ambient" );
  hDiffuse = p->GetParameterBySemantic( 0, "Diffuse" );
  hSpecular = p->GetParameterBySemantic( 0, "Specular" );
  hOpacity = p->GetParameterBySemantic( 0, "Opacity" );
  hSpecularPower = p->GetParameterBySemantic( 0, "SpecularPower" );
  hLightColor = p->GetParameterBySemantic( 0, "LightColor" );
  hLightPosition = p->GetParameterBySemantic( 0, "LightPosition" );
  hCameraPosition = p->GetParameterBySemantic( 0, "CameraPosition" );
  hTexture = p->GetParameterBySemantic( 0, "Texture" );
  hTime = p->GetParameterBySemantic( 0, "Time" );
  hWorld = p->GetParameterBySemantic( 0, "World" );
  hWorldViewProjection = p->GetParameterBySemantic( 0, "WorldViewProjection" );

  // Retrieve the ID3DXMesh pointer and current material from the MeshLoader helper
  // Set the lighting variables and texture for the current material
  V( p->SetValue(     hAmbient,       mMaterial.vAmbient, sizeof( D3DXVECTOR3 ) ) );
  V( p->SetValue(     hDiffuse,       mMaterial.vDiffuse, sizeof( D3DXVECTOR3 ) ) );
  V( p->SetValue(     hSpecular,      mMaterial.vSpecular, sizeof( D3DXVECTOR3 ) ) );
  V( p->SetTexture(   hTexture,       mMaterial.pTexture ) );
  V( p->SetFloat(     hOpacity,       mMaterial.fAlpha ) );
  V( p->SetInt(       hSpecularPower, mMaterial.nShininess ) );
  V( p->SetTechnique( mMaterial.hTechnique ) );

  mSubset = subset;
}
//-----------------------------------------------------------
HRESULT TEffectDX::SetMatrixWorld(D3DXMATRIXA16* matrix)
{
  HRESULT hr;
  V_RETURN(p->SetMatrix(hWorld,matrix));
  return S_OK;
}
//-----------------------------------------------------------
HRESULT TEffectDX::SetMatrixWorldViewProjection(D3DXMATRIXA16* matrix)
{
  HRESULT hr;
  V_RETURN(p->SetMatrix(hWorldViewProjection,matrix));
  return S_OK;
}
//-----------------------------------------------------------
HRESULT TEffectDX::Begin(UINT* cPasses , DWORD flag)
{
  HRESULT hr;
  V_RETURN( p->Begin( cPasses, flag ) );
  return S_OK;
}
//-----------------------------------------------------------
HRESULT TEffectDX::BeginPass(UINT iPass)
{
  HRESULT hr;
  V_RETURN( p->BeginPass( iPass ) );
  return S_OK;
}
//-----------------------------------------------------------
HRESULT TEffectDX::EndPass()
{
  HRESULT hr;
  V_RETURN( p->EndPass() );
  return S_OK;
}
//-----------------------------------------------------------
HRESULT TEffectDX::End()
{
  HRESULT hr;
  V_RETURN( p->End() );
  return S_OK;
}
//-----------------------------------------------------------
void TEffectDX::Destroy()
{
  HRESULT hr;
  SAFE_RELEASE(p);
}
//-----------------------------------------------------------
void TEffectDX::LostDevice()
{
  HRESULT hr;
  V(p->OnLostDevice());
}
//-----------------------------------------------------------
void TEffectDX::ResetDevice()
{
  HRESULT hr;
  V(p->OnResetDevice());
}
//-----------------------------------------------------------
