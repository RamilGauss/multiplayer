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


#include "EffectDX.h"
#include "Define_DX.h"


TEffectDX::TEffectDX()
{
  mArrJoint = NULL;
  mCntJoint = 0;

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
  hWorldViewProjection = NULL;
}
//-----------------------------------------------------------
TEffectDX::~TEffectDX()
{
  delete[] mArrJoint;
  mCntJoint = 0;
}
//-----------------------------------------------------------
void TEffectDX::Init()
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

  V( p->SetValue(     hAmbient,       mMaterial.vAmbient, sizeof( D3DXVECTOR3 ) ) );
  V( p->SetValue(     hDiffuse,       mMaterial.vDiffuse, sizeof( D3DXVECTOR3 ) ) );
  V( p->SetValue(     hSpecular,      mMaterial.vSpecular, sizeof( D3DXVECTOR3 ) ) );
  V( p->SetTexture(   hTexture,       mMaterial.pTexture ) );
  V( p->SetFloat(     hOpacity,       mMaterial.fAlpha ) );
  V( p->SetInt(       hSpecularPower, mMaterial.nShininess ) );
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
HRESULT TEffectDX::SetCameraPosition(const D3DXVECTOR3* camera_pos)
{
  HRESULT hr;
  V_RETURN(p->SetValue(hCameraPosition,camera_pos, sizeof( D3DXVECTOR3 )));
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
  SAFE_RELEASE(mMaterial.pTexture);
  SAFE_RELEASE(p);
  SAFE_RELEASE(pMesh);
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

  // загрузка техники исполнения в шейдере
  mMaterial.hTechnique = p->GetTechniqueByName( mMaterial.strTechnique.data() );
  V( p->SetTechnique( mMaterial.hTechnique ) );
}
//-----------------------------------------------------------
D3DXMATRIXA16* TEffectDX::GetBlendMatrixByName(char* sNamePart)
{

  return NULL;
}
//-----------------------------------------------------------
void TEffectDX::CreateJoint(int cnt)
{
  delete []mArrJoint;
  mArrJoint = new TJointPart[cnt];
  mCntJoint = cnt;
}
//-----------------------------------------------------------
void TEffectDX::AddJoint(int i, std::string s, D3DXMATRIXA16& matrix)
{
  BL_ASSERT(i<mCntJoint);
  mArrJoint[i].namePart = s;
  mArrJoint[i].matrix   = matrix;
}
//-----------------------------------------------------------
