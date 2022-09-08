/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
√удаков –амиль —ергеевич 
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

#include "BL_Debug.h"

#include "EffectDX.h"
#include "DXUT.h"
#include "ModelDX.h"

namespace nsEffectDX
{
	const char* strAmbient  			= "Ambient";
	const char* strDiffuse  			= "Diffuse";
	const char* strSpecular 			= "Specular";
	const char* strOpacity  			= "Opacity";
	const char* strSpecularPower  = "SpecularPower";
	const char* strLightColor     = "LightColor";
	const char* strLightPosition  = "LightPosition";
	const char* strCameraPosition = "CameraPosition";
	const char* strTexture    		= "Texture";
	const char* strTime       		= "Time";
	const char* strWorld      		= "World";
	const char* strView       		= "View";
	const char* strProjection 		= "Proj";
	// доп. параметры
	const char* strTexCubeMap     = "TextureCubeMap";
};

using namespace nsEffectDX;

TEffectDX::TEffectDX()
{
  p = NULL;
  pMesh = NULL;

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
  hProjection = NULL;
  hView = NULL;

	hTexCubeMap = NULL;
} 
//-----------------------------------------------------------
TEffectDX::~TEffectDX()
{
  
}
//-----------------------------------------------------------
void TEffectDX::Init()
{
  // Cache the effect handles
  hAmbient = p->GetParameterBySemantic( 0, strAmbient );
  hDiffuse = p->GetParameterBySemantic( 0, strDiffuse );
  hSpecular = p->GetParameterBySemantic( 0, strSpecular );
  hOpacity = p->GetParameterBySemantic( 0, strOpacity );
  hSpecularPower = p->GetParameterBySemantic( 0, strSpecularPower );
  hLightColor = p->GetParameterBySemantic( 0, strLightColor );
  hLightPosition = p->GetParameterBySemantic( 0, strLightPosition );
  hCameraPosition = p->GetParameterBySemantic( 0, strCameraPosition );
  hTexture = p->GetParameterBySemantic( 0, strTexture );
  hTime = p->GetParameterBySemantic( 0, strTime );
  hWorld = p->GetParameterBySemantic( 0, strWorld );
  hView = p->GetParameterBySemantic( 0, strView );
  hProjection = p->GetParameterBySemantic( 0, strProjection );
	//---------------------------------------------------
	// доп. параметры
	hTexCubeMap = p->GetParameterBySemantic( 0, strTexCubeMap );
}
//-----------------------------------------------------------
HRESULT TEffectDX::SetMatrixWorld(D3DXMATRIXA16* matrix)
{
  HRESULT hr;
  V_RETURN(p->SetMatrix(hWorld,matrix));
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
  // модель освободит ресурсы
  mMaterial.pTexture = NULL;
  p = NULL;

  SAFE_RELEASE(pMesh);
}
//-----------------------------------------------------------
void TEffectDX::LostDevice()
{
}
//-----------------------------------------------------------
void TEffectDX::ResetDevice()
{
  HRESULT hr;
  // загрузка техники исполнени€ в шейдере
  mMaterial.hTechnique = p->GetTechniqueByName( mMaterial.strTechnique.data() );
  V( p->SetTechnique( mMaterial.hTechnique ) );
}
//-----------------------------------------------------------
void TEffectDX::SetInnerShaderParam()
{
  HRESULT hr;
  V( p->SetValue(     hAmbient,       mMaterial.vAmbient, sizeof( D3DXVECTOR3 ) ) );
  V( p->SetValue(     hDiffuse,       mMaterial.vDiffuse, sizeof( D3DXVECTOR3 ) ) );
  V( p->SetValue(     hSpecular,      mMaterial.vSpecular, sizeof( D3DXVECTOR3 ) ) );
  V( p->SetFloat(     hOpacity,       mMaterial.fAlpha ) );
  V( p->SetInt(       hSpecularPower, mMaterial.nShininess ) );
  V( p->SetTexture(   hTexture,       mMaterial.pTexture ) );
}
//-----------------------------------------------------------
void TEffectDX::SetAlphaTransparency(float alphaTransparency)
{
  mMaterial.fAlpha = alphaTransparency;
}
//-----------------------------------------------------------
bool TEffectDX::UseCubeMap()
{
	return mMaterial.flgUseCubeMap;
}
//-----------------------------------------------------------
void TEffectDX::SetTexCubeMap(IDirect3DCubeTexture9** pTexture)
{
	if(mMaterial.flgUseCubeMap==false) return;
	HRESULT hr;
	V( p->SetTexture( hTexCubeMap, *pTexture ) );
}
//-----------------------------------------------------------
void TEffectDX::SetTexture(IDirect3DTexture9* pTexture)
{
	mMaterial.pTexture = pTexture;
}
//----------------------------------------------------------------------------------------------------	
void TEffectDX::SetTexture(const char* nameTexture, IDirect3DTexture9* pTexture)
{
  D3DXHANDLE hNameTexture = p->GetParameterBySemantic( 0, nameTexture );
	HRESULT hr;
  V( p->SetTexture( hNameTexture, pTexture ) );
}
//----------------------------------------------------------------------------------------------------	
void TEffectDX::SetBool(const char* nameTexture, bool v)
{
	D3DXHANDLE hNameTexture = p->GetParameterBySemantic( 0, nameTexture );
	HRESULT hr;
	V( p->SetBool( hNameTexture, v ) );
}
//----------------------------------------------------------------------------------------------------	
void TEffectDX::SetFloat(const char* nameTexture, float v)
{
	D3DXHANDLE hNameTexture = p->GetParameterBySemantic( 0, nameTexture );
	HRESULT hr;
	V( p->SetFloat( hNameTexture, v ) );
}
//----------------------------------------------------------------------------------------------------	
