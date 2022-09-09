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


#ifndef Struct3DH
#define Struct3DH

#define SET_MATRIX16_M_M(In,Out) \
Out._11 = In._11;  Out._12 = In._12;  Out._13 = In._13;  Out._14 = In._14; \
Out._21 = In._21;  Out._22 = In._22;  Out._23 = In._23;  Out._24 = In._24; \
Out._31 = In._31;  Out._32 = In._32;  Out._33 = In._33;  Out._34 = In._34; \
Out._41 = In._41;  Out._42 = In._42;  Out._43 = In._43;  Out._44 = In._44;

#define SET_MATRIX16_M_P(In,pOut) \
pOut->_11 = In._11;  pOut->_12 = In._12;  pOut->_13 = In._13;  pOut->_14 = In._14; \
pOut->_21 = In._21;  pOut->_22 = In._22;  pOut->_23 = In._23;  pOut->_24 = In._24; \
pOut->_31 = In._31;  pOut->_32 = In._32;  pOut->_33 = In._33;  pOut->_34 = In._34; \
pOut->_41 = In._41;  pOut->_42 = In._42;  pOut->_43 = In._43;  pOut->_44 = In._44;

#define SET_MATRIX16_P_M(pIn,Out) \
Out._11 = pIn->_11;  Out._12 = pIn->_12;  Out._13 = pIn->_13;  Out._14 = pIn->_14; \
Out._21 = pIn->_21;  Out._22 = pIn->_22;  Out._23 = pIn->_23;  Out._24 = pIn->_24; \
Out._31 = pIn->_31;  Out._32 = pIn->_32;  Out._33 = pIn->_33;  Out._34 = pIn->_34; \
Out._41 = pIn->_41;  Out._42 = pIn->_42;  Out._43 = pIn->_43;  Out._44 = pIn->_44;



namespace nsStruct3D
{

#pragma pack(push, 1)

class TVector3
{
  float x;
  float y;
  float z;
};
//-----------------------------------------------------------------
class TVector4
{
  float x;
  float y;
  float z;
  float k;
};
//-----------------------------------------------------------------
class TVector3_3
{
  union {
    struct {
      float _11, _12, _13;
      float _21, _22, _23;
      float _31, _32, _33;
    };
    float m[3][3];
  };
};
//-----------------------------------------------------------------
class TVector4_4
{
public:
  union {
    struct {
      float _11, _12, _13, _14;
      float _21, _22, _23, _24;
      float _31, _32, _33, _34;
      float _41, _42, _43, _44;
    };
    float m[4][4];
  };

  // assignment operators
  TVector4_4& operator += ( const TVector4_4& );
  TVector4_4& operator -= ( const TVector4_4& );
  TVector4_4& operator *= ( float );
  TVector4_4& operator /= ( float );

  TVector4_4& operator *= ( const TVector4_4& );
  //TVector4_4& operator /= ( const TVector4_4& );

  // unary operators
  TVector4_4 operator + () const;
  TVector4_4 operator - () const;

  // binary operators
  TVector4_4 operator + ( const TVector4_4& ) const;
  TVector4_4 operator - ( const TVector4_4& ) const;
  
  TVector4_4 operator * ( const TVector4_4& ) const;
  //TVector4_4 operator / ( const TVector4_4& ) const;

  TVector4_4 operator * ( float ) const;
  TVector4_4 operator / ( float ) const;

  //friend TVector4_4 operator * ( float, const TVector4_4& );

  bool operator == ( const TVector4_4& ) const;
  bool operator != ( const TVector4_4& ) const;
};

#pragma pack(pop)

}

extern void VectorIdentity(nsStruct3D::TVector4_4* pV);
extern void VectorIdentity(nsStruct3D::TVector3_3* pV);


#endif
