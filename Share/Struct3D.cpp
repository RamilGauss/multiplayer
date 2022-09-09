/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012
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
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#include "Struct3D.h"

//----------------------------------------------------------------------
#define SET_MATRIX_ROTATE_DX(pV,ugol,AXE) \
D3DXMATRIX* InOut = (D3DXMATRIX*)pV; \
D3DXMatrixRotation##AXE(InOut,ugol);
//----------------------------------------------------------------------


#ifdef WIN32
  #include <d3dx9math.h>
#endif

using namespace nsStruct3D;

//-------------------------------------------------------------------------
void CopyMatrix16(float* pSrc, float* pDst)
{
  memcpy(pDst,pSrc,sizeof(TMatrix16));
}
//-------------------------------------------------------------------------
TMatrix16* SetMatrixIdentity(TMatrix16* pV)
{
#ifdef WIN32
  D3DXMATRIXA16* pM = (D3DXMATRIXA16*)pV;
  D3DXMatrixIdentity(pM);
#else
#endif
  return pV;
}
//-------------------------------------------------------------------------
TMatrix9* SetMatrixIdentity(TMatrix9* pV)
{
  pV->_11 = 1.0f;  pV->_12 = 0.0f;  pV->_13 = 0.0f;
  pV->_21 = 0.0f;  pV->_22 = 1.0f;  pV->_23 = 0.0f;
  pV->_31 = 0.0f;  pV->_32 = 0.0f;  pV->_33 = 1.0f;
  return pV;
}
//-------------------------------------------------------------------------
TMatrix16* SetMatrixRotateX(TMatrix16* pV, float ugol)
{
#ifdef WIN32
  SET_MATRIX_ROTATE_DX(pV,ugol,X)
#else
#endif
  return pV;
}
//-------------------------------------------------------------------------
TMatrix16* SetMatrixRotateY(TMatrix16* pV, float ugol)
{
#ifdef WIN32
  SET_MATRIX_ROTATE_DX(pV,ugol,Y)
#else
#endif
  return pV;
}
//-------------------------------------------------------------------------
TMatrix16* SetMatrixRotateZ(TMatrix16* pV, float ugol)
{
#ifdef WIN32
  SET_MATRIX_ROTATE_DX(pV,ugol,Z)
#else
#endif
  return pV;
}
//-------------------------------------------------------------------------
TMatrix16* SetMatrixRotationYawPitchRoll(nsStruct3D::TMatrix16* pV,
                                                   float Yaw,
                                                   float Pitch,
                                                   float Roll )
{
#ifdef WIN32
  D3DXMATRIX* InOut = (D3DXMATRIX*)pV;
  D3DXMatrixRotationYawPitchRoll(InOut,
                                 Yaw,
                                 Pitch,
                                 Roll);
#else
#endif
  return pV;
}
//-------------------------------------------------------------------------
TMatrix16* SetMatrixRotationAxis(TMatrix16 *pOut,
                                 const TVector3 *pV_,
                                 float Angle)
{
#ifdef WIN32
  D3DXMATRIX* InOut = (D3DXMATRIX*)pOut;
  const D3DXVECTOR3* pV = (const D3DXVECTOR3*)pV_;
  D3DXMatrixRotationAxis(InOut,
                         pV,
                         Angle);
#else
#endif
  return pOut;
}
//-------------------------------------------------------------------------
TMatrix16* SetMatrixMultiply(TMatrix16 *pOut,
                             const TMatrix16 *pM1,
                             const TMatrix16 *pM2)
{
#ifdef WIN32
  D3DXMATRIX* InOut = (D3DXMATRIX*)pOut;
  D3DXMATRIX* In1 = (D3DXMATRIX*)pM1;
  D3DXMATRIX* In2 = (D3DXMATRIX*)pM2;
  D3DXMatrixMultiply(InOut,
                     In1,
                     In2);
#else
#endif
  return pOut;
}
//-------------------------------------------------------------------------
TVector3*  SetVec3TransformCoord(TVector3* pOut,
                                 const TVector3* pV_,
                                 const TMatrix16* pM_)
{
#ifdef WIN32
  D3DXVECTOR3* InOut = (D3DXVECTOR3*)pOut;
  const D3DXVECTOR3* pV = (const D3DXVECTOR3*)pV_;
  const D3DXMATRIX* pM  = (const D3DXMATRIX*)pM_;
  D3DXVec3TransformCoord(InOut,
                         pV,
                         pM );
#else
#endif
  return pOut;
}
//-------------------------------------------------------------------------
TVector3* SetVec3Cross( TVector3* pOut,
                        const TVector3* pV1_,
                        const TVector3* pV2_)
{
#ifdef WIN32
  D3DXVECTOR3* InOut     = (D3DXVECTOR3*) pOut;
  const D3DXVECTOR3* pV1 = (const D3DXVECTOR3*) pV1_;
  const D3DXVECTOR3* pV2 = (const D3DXVECTOR3*) pV2_;
 
  D3DXVec3Cross(InOut,
                pV1,
                pV2);
#else
#endif
  return pOut;
}
//-------------------------------------------------------------------------
float SetVec3Dot( const TVector3* pV1_,
                  const TVector3* pV2_)
{
  float res;
#ifdef WIN32
  const D3DXVECTOR3* pV1 = (const D3DXVECTOR3*)pV1_;
  const D3DXVECTOR3* pV2 = (const D3DXVECTOR3*)pV2_;

  res = D3DXVec3Dot(pV1,
                    pV2);
#else
#endif
  return res;
}
//-------------------------------------------------------------------------
TVector3* SetVec3Normalize(TVector3* pOut,
                           const TVector3* pV_)
{
#ifdef WIN32
  D3DXVECTOR3 *InOut    = (D3DXVECTOR3*)pOut;
  const D3DXVECTOR3* pV = (const D3DXVECTOR3*)pV_;
  D3DXVec3Normalize(InOut,
                    pV);
#else
#endif
  return pOut;
}
//-------------------------------------------------------------------------
TMatrix16* SetMatrixPerspectiveFovLH( TMatrix16* pOut, 
                                      float fovy, 
                                      float Aspect, 
                                      float zn, float zf )
{
#ifdef WIN32
  D3DXMATRIX* InOut = (D3DXMATRIX*)pOut;
  D3DXMatrixPerspectiveFovLH( InOut, 
                              fovy, 
                              Aspect, 
                              zn, zf );
#else
#endif
  return pOut;
}
//-------------------------------------------------------------------------
TMatrix16::TMatrix16( float _11, float _12, float _13, float _14,
                      float _21, float _22, float _23, float _24,
                      float _31, float _32, float _33, float _34,
                      float _41, float _42, float _43, float _44 )
{
  this->_11 = _11; this->_12 = _12; this->_13 = _13; this->_14 = _14;
  this->_21 = _21; this->_22 = _22; this->_23 = _23; this->_24 = _24;
  this->_31 = _31; this->_32 = _32; this->_33 = _33; this->_34 = _34;
  this->_41 = _41; this->_42 = _42; this->_43 = _43; this->_44 = _44;
}
//-------------------------------------------------------------------------
// assignment operators
TMatrix16& TMatrix16::operator += ( const TMatrix16& v)
{
  MATRIX16_OP_P_M(this,v, += )
  return *this;
}
//-------------------------------------------------------------------------
TMatrix16& TMatrix16::operator -= ( const TMatrix16& v)
{
  MATRIX16_OP_P_M(this,v, -= )
  return *this;
}
//-------------------------------------------------------------------------
TMatrix16& TMatrix16::operator *= ( float v)
{
  MATRIX16_OP_P(this,v,*=)
  return *this;
}
//-------------------------------------------------------------------------
TMatrix16& TMatrix16::operator /= ( float v)
{
  MATRIX16_OP_P(this,v,/=)
  return *this;
}
//-------------------------------------------------------------------------
TMatrix16 TMatrix16::operator + () const
{
  return *this;
}
//-------------------------------------------------------------------------
TMatrix16 TMatrix16::operator - () const
{
  return *this;
}
//-------------------------------------------------------------------------
TMatrix16 TMatrix16::operator + ( const TMatrix16& v) const
{
  TMatrix16 res = *this;
  MATRIX16_OP_M_M(res,v,+=)  
  return res;
}
//-------------------------------------------------------------------------
TMatrix16 TMatrix16::operator - ( const TMatrix16& v) const
{
  TMatrix16 res = *this;
  MATRIX16_OP_M_M(res,v,-=)
  return res;
}
//-------------------------------------------------------------------------
TMatrix16 TMatrix16::operator * ( float v) const
{
  TMatrix16 res = *this;
  MATRIX16_OP_M(res,v,*=)

  return res;
}
//-------------------------------------------------------------------------
TMatrix16 TMatrix16::operator / ( float v) const
{
  TMatrix16 res = *this;
  MATRIX16_OP_M(res,v,/=)

  return res;
}
//-------------------------------------------------------------------------
bool TMatrix16::operator == ( const TMatrix16& v) const
{
  for(int k = 0 ; k < 4 ; k++)
    for(int n = 0 ; n < 4 ; n++)
      if(BASE_MATRIX_OP(this,v,!=,->,.,k,n))
        return false;
  return true;
}
//-------------------------------------------------------------------------
bool TMatrix16::operator != ( const TMatrix16& v) const
{
  for(int k = 0 ; k < 4 ; k++)
    for(int n = 0 ; n < 4 ; n++)
      if(BASE_MATRIX_OP(this,v,==,->,.,k,n))
        return false;
  return true;
}
//-------------------------------------------------------------------------
TMatrix16& TMatrix16::operator *= ( const TMatrix16& v)
{
#ifdef WIN32
  TMatrix16 res;
  D3DXMATRIX* In1 = (D3DXMATRIX*)this,
            * In2 = (D3DXMATRIX*)&v,
            * Out = (D3DXMATRIX*)&res;
  D3DXMatrixMultiply(In1,In1,In2);
#else
#endif
  return *this;
}
//-------------------------------------------------------------------------
TMatrix16 TMatrix16::operator * ( const TMatrix16& v) const
{
#ifdef WIN32
  TMatrix16 res;
  D3DXMATRIX* In1 = (D3DXMATRIX*)this,
            * In2 = (D3DXMATRIX*)&v,
            * Out = (D3DXMATRIX*)&res;
  D3DXMatrixMultiply(Out,In1,In2);
#else
#endif
  return res;
}
//-------------------------------------------------------------------------
// assignment operators
TVector3& TVector3::operator += ( const TVector3& pV)
{
  x += pV.x;
  y += pV.y;
  z += pV.z;
  return *this;
}
//-------------------------------------------------------------------------
TVector3& TVector3::operator -= ( const TVector3& pV)
{
  x -= pV.x;
  y -= pV.y;
  z -= pV.z;
  return *this;
}
//-------------------------------------------------------------------------
TVector3& TVector3::operator *= ( float v)
{
  x *= v;
  y *= v;
  z *= v;
  return *this;
}
//-------------------------------------------------------------------------
TVector3& TVector3::operator /= ( float v)
{
  x /= v;
  y /= v;
  z /= v;
  return *this;
}
//-------------------------------------------------------------------------
// unary operators
TVector3 TVector3::operator + () const
{
  return *this;
}
//-------------------------------------------------------------------------
TVector3 TVector3::operator - () const
{
  return *this;
}
//-------------------------------------------------------------------------
// binary operators
TVector3 TVector3::operator + ( const TVector3& pV) const
{
  TVector3 res;
  res.x = pV.x + x;
  res.y = pV.y + y;
  res.z = pV.z + z;
  return res;
}
//-------------------------------------------------------------------------
TVector3 TVector3::operator - ( const TVector3& pV) const
{
  TVector3 res;
  res.x = pV.x - x;
  res.y = pV.y - y;
  res.z = pV.z - z;
  return res;
}
//-------------------------------------------------------------------------
TVector3 TVector3::operator * ( float v) const
{
  TVector3 res;
  res.x = x * v;
  res.y = y * v;
  res.z = z * v;
  return res;
}
//-------------------------------------------------------------------------
TVector3 TVector3::operator / ( float v) const
{
  TVector3 res;
  res.x = x / v;
  res.y = y / v;
  res.z = z / v;
  return res;
}
//-------------------------------------------------------------------------
bool TVector3::operator == ( const TVector3& pV) const
{
  return ((pV.x==x)&&(pV.y==y)&&(pV.z==z));
}
//-------------------------------------------------------------------------
bool TVector3::operator != ( const TVector3& pV) const
{
  return ((pV.x!=x)||(pV.y!=y)||(pV.z!=z));
}
//-------------------------------------------------------------------------
