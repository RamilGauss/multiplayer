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
the "Tanks" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#include "Struct3D.h"

#ifdef WIN32
  #include <d3dx10math.h>
#endif

using namespace nsStruct3D;

//-------------------------------------------------------------------------
void VectorIdentity(TVector4_4* pV)
{
  pV->_11 = 1.0f;  pV->_12 = 0.0f;  pV->_13 = 0.0f;  pV->_14 = 0.0f;
  pV->_21 = 0.0f;  pV->_22 = 1.0f;  pV->_23 = 0.0f;  pV->_24 = 0.0f;
  pV->_31 = 0.0f;  pV->_32 = 0.0f;  pV->_33 = 1.0f;  pV->_34 = 0.0f;
  pV->_41 = 0.0f;  pV->_42 = 0.0f;  pV->_43 = 0.0f;  pV->_44 = 1.0f;
}
//-------------------------------------------------------------------------
void VectorIdentity(TVector3_3* pV)
{
  pV->_11 = 1.0f;  pV->_12 = 0.0f;  pV->_13 = 0.0f;
  pV->_21 = 0.0f;  pV->_22 = 1.0f;  pV->_23 = 0.0f;
  pV->_31 = 0.0f;  pV->_32 = 0.0f;  pV->_33 = 1.0f;
}
//-------------------------------------------------------------------------
// assignment operators
TVector4_4& TVector4_4::operator += ( const TVector4_4& v)
{
  _11 += v._11;  _12 += v._12;  _13 += v._13;  _14 += v._14;
  _21 += v._21;  _22 += v._22;  _23 += v._23;  _24 += v._24;
  _31 += v._31;  _32 += v._32;  _33 += v._33;  _34 += v._34;
  _41 += v._41;  _42 += v._42;  _43 += v._43;  _44 += v._44;
  return *this;
}
//-------------------------------------------------------------------------
TVector4_4& TVector4_4::operator -= ( const TVector4_4& v)
{
  _11 -= v._11;  _12 -= v._12;  _13 -= v._13;  _14 -= v._14;
  _21 -= v._21;  _22 -= v._22;  _23 -= v._23;  _24 -= v._24;
  _31 -= v._31;  _32 -= v._32;  _33 -= v._33;  _34 -= v._34;
  _41 -= v._41;  _42 -= v._42;  _43 -= v._43;  _44 -= v._44;
  return *this;
}
//-------------------------------------------------------------------------
TVector4_4& TVector4_4::operator *= ( float v)
{
  _11 *= v;  _12 *= v;  _13 *= v;  _14 *= v;
  _21 *= v;  _22 *= v;  _23 *= v;  _24 *= v;
  _31 *= v;  _32 *= v;  _33 *= v;  _34 *= v;
  _41 *= v;  _42 *= v;  _43 *= v;  _44 *= v;

  return *this;
}
//-------------------------------------------------------------------------
TVector4_4& TVector4_4::operator /= ( float v)
{
  _11 /= v;  _12 /= v;  _13 /= v;  _14 /= v;
  _21 /= v;  _22 /= v;  _23 /= v;  _24 /= v;
  _31 /= v;  _32 /= v;  _33 /= v;  _34 /= v;
  _41 /= v;  _42 /= v;  _43 /= v;  _44 /= v;

  return *this;
}
//-------------------------------------------------------------------------
TVector4_4 TVector4_4::operator + () const
{
  return *this;
}
//-------------------------------------------------------------------------
TVector4_4 TVector4_4::operator - () const
{
  return *this;
}
//-------------------------------------------------------------------------
TVector4_4 TVector4_4::operator + ( const TVector4_4& v) const
{
  TVector4_4 res = *this;
  res._11 += v._11;  res._12 += v._12;  res._13 += v._13;  res._14 += v._14;
  res._21 += v._21;  res._22 += v._22;  res._23 += v._23;  res._24 += v._24;
  res._31 += v._31;  res._32 += v._32;  res._33 += v._33;  res._34 += v._34;
  res._41 += v._41;  res._42 += v._42;  res._43 += v._43;  res._44 += v._44;
    
  return res;
}
//-------------------------------------------------------------------------
TVector4_4 TVector4_4::operator - ( const TVector4_4& v) const
{
  TVector4_4 res = *this;
  res._11 -= v._11;  res._12 -= v._12;  res._13 -= v._13;  res._14 -= v._14;
  res._21 -= v._21;  res._22 -= v._22;  res._23 -= v._23;  res._24 -= v._24;
  res._31 -= v._31;  res._32 -= v._32;  res._33 -= v._33;  res._34 -= v._34;
  res._41 -= v._41;  res._42 -= v._42;  res._43 -= v._43;  res._44 -= v._44;

  return res;
}
//-------------------------------------------------------------------------
TVector4_4 TVector4_4::operator * ( float v) const
{
  TVector4_4 res = *this;
  res._11 *= v;  res._12 *= v;  res._13 *= v;  res._14 *= v;
  res._21 *= v;  res._22 *= v;  res._23 *= v;  res._24 *= v;
  res._31 *= v;  res._32 *= v;  res._33 *= v;  res._34 *= v;
  res._41 *= v;  res._42 *= v;  res._43 *= v;  res._44 *= v;

  return res;
}
//-------------------------------------------------------------------------
TVector4_4 TVector4_4::operator / ( float v) const
{
  TVector4_4 res = *this;
  res._11 /= v;  res._12 /= v;  res._13 /= v;  res._14 /= v;
  res._21 /= v;  res._22 /= v;  res._23 /= v;  res._24 /= v;
  res._31 /= v;  res._32 /= v;  res._33 /= v;  res._34 /= v;
  res._41 /= v;  res._42 /= v;  res._43 /= v;  res._44 /= v;

  return res;
}
//-------------------------------------------------------------------------
bool TVector4_4::operator == ( const TVector4_4& v) const
{
  if(
    (_11 == v._11) && (_12 == v._12) && (_13 == v._13) && (_14 == v._14) &&
    (_21 == v._21) && (_22 == v._22) && (_23 == v._23) && (_24 == v._24) &&
    (_31 == v._31) && (_32 == v._32) && (_33 == v._33) && (_34 == v._34) &&
    (_41 == v._41) && (_42 == v._42) && (_43 == v._43) && (_44 == v._44) ) 
    return true;

  return false;
}
//-------------------------------------------------------------------------
bool TVector4_4::operator != ( const TVector4_4& v) const
{
  if(
    (_11 != v._11) || (_12 != v._12) || (_13 != v._13) || (_14 != v._14) ||
    (_21 != v._21) || (_22 != v._22) || (_23 != v._23) || (_24 != v._24) ||
    (_31 != v._31) || (_32 != v._32) || (_33 != v._33) || (_34 != v._34) ||
    (_41 != v._41) || (_42 != v._42) || (_43 != v._43) || (_44 != v._44) ) 
    return true;

  return false;
}
//-------------------------------------------------------------------------
TVector4_4& TVector4_4::operator *= ( const TVector4_4& v)
{
#ifdef WIN32
  D3DXMATRIX In1,In2,Out;
  SET_MATRIX16_P_M(this,In1)
  SET_MATRIX16_M_M(v,In2)

  D3DXMatrixMultiply(&Out,&In1,&In2);
  SET_MATRIX16_M_P(Out,this)
#endif
  return *this;
}
//-------------------------------------------------------------------------
TVector4_4 TVector4_4::operator * ( const TVector4_4& v) const
{
#ifdef WIN32
  TVector4_4 res;
  D3DXMATRIX In1,In2,Out;
  SET_MATRIX16_P_M(this,In1)
  SET_MATRIX16_M_M(v,In2)

  D3DXMatrixMultiply(&Out,&In1,&In2);
  SET_MATRIX16_M_M(Out,res)
#endif
  return res;
}
//-------------------------------------------------------------------------

