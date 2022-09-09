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

#define INDEX_I(M,k,n,I) \
M I m[k][n]

// M1 и M2 - TMatrix16 или D3DXMATRIX
// OP - операция, например -=, +
// I1 и I2 - как происходит адресация, через . или ->
// k и n - индексы в матрице
#define BASE_MATRIX_OP(M1,M2,OP,I1,I2,k,n) \
INDEX_I(M1,k,n,I1) OP INDEX_I(M2,k,n,I2)

#define MATRIX_ALL(M1,M2,OP,I1,I2,Ck,Cn) \
for(int counter_k = 0 ; counter_k < Ck ; counter_k++) \
for(int counter_n = 0 ; counter_n < Cn ; counter_n++) \
BASE_MATRIX_OP(M1,M2,OP,I1,I2,counter_k,counter_n);

#define MATRIX16_OP(M1,M2,OP,I1,I2) \
MATRIX_ALL(M1,M2,OP,I1,I2,4,4)

//----------------------------------------------------------------------
#define BASE_MATRIX_OP_VALUE(M,VALUE,OP,I,k,n) \
INDEX_I(M,k,n,I) OP VALUE

#define MATRIX_ALL_VALUE(M,VALUE,OP,I,Ck,Cn) \
for(int k = 0 ; k < Ck ; k++) \
for(int n = 0 ; n < Cn ; n++) \
BASE_MATRIX_OP_VALUE(M,VALUE,OP,I,k,n);

#define MATRIX16_OP_V(M,VALUE,OP,I) \
MATRIX_ALL_VALUE(M,VALUE,OP,I,4,4)

//----------------------------------------------------------------------
// Макросы для использования
// OP - операция, например, a *= b; M1 = "a", M2 = "b", OP = "*="
// MATRIX16_OP_M_M( a, b, *= )
//----------------------------------------------------------------------
// матрица - матрица
#define MATRIX16_OP_M_M(M1,M2,OP) \
MATRIX16_OP(M1,M2,OP, . , .)

#define MATRIX16_OP_M_P(M1,M2,OP) \
MATRIX16_OP(M1,M2,OP, . , ->)

#define MATRIX16_OP_P_M(M1,M2,OP) \
MATRIX16_OP(M1,M2,OP, -> , .)

#define MATRIX16_OP_P_P(M1,M2,OP) \
MATRIX16_OP(M1,M2,OP, -> , ->)
//----------------------------------------------------------------------
// матрица - float/double
#define MATRIX16_OP_M(M,VALUE,OP) \
MATRIX16_OP_V(M,VALUE,OP, . )

#define MATRIX16_OP_P(M,VALUE,OP) \
MATRIX16_OP_V(M,VALUE,OP, ->)
//----------------------------------------------------------------------
// проверка на равенство двух матриц
#define MATRIX16_EQUAL_M_M(a,b) \
MATRIX16_OP_M_M(a,b,=)

#define MATRIX16_EQUAL_M_P(a,b) \
MATRIX16_OP_M_P(a,b,=)

#define MATRIX16_EQUAL_P_M(a,b) \
MATRIX16_OP_P_M(a,b,=)

#define MATRIX16_EQUAL_P_P(a,b) \
MATRIX16_OP_P_P(a,b,=)
//----------------------------------------------------------------------
#define SET_MATRIX_ROTATE_WIN(pV,ugol,AXE) \
D3DXMATRIX InOut; \
MATRIX16_EQUAL_M_P(InOut,pV) \
D3DXMatrixRotation##AXE(&InOut,ugol); \
MATRIX16_EQUAL_P_M(pV,InOut)
//----------------------------------------------------------------------

namespace nsStruct3D
{

#pragma pack(push, 1)

class TPoint2
{
public:
  unsigned int x;
  unsigned int y;
  TPoint2(){x=0;y=0;}
};

class TVector3
{
public:
  float x;
  float y;
  float z;
  TVector3(float _x,float _y,float _z)
  {
    x=_x;y=_y;z=_z;
  }
  TVector3()
  {
    x=0;y=0;z=0;
  }
};
//-----------------------------------------------------------------
class TVector4
{
public:
  float x;
  float y;
  float z;
  float w;
  TVector4(float _x,float _y,float _z,float _w)
  {
    x=_x;y=_y;z=_z;w=_w;
  }
  TVector4()
  {
    x=0;y=0;z=0;w=0;
  }

};
//-----------------------------------------------------------------
class TMatrix9
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
class TMatrix16
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
  TMatrix16& operator += ( const TMatrix16& );
  TMatrix16& operator -= ( const TMatrix16& );
  TMatrix16& operator *= ( float );
  TMatrix16& operator /= ( float );

  TMatrix16& operator *= ( const TMatrix16& );
  //TMatrix16& operator /= ( const TMatrix16& );

  // unary operators
  TMatrix16 operator + () const;
  TMatrix16 operator - () const;

  // binary operators
  TMatrix16 operator + ( const TMatrix16& ) const;
  TMatrix16 operator - ( const TMatrix16& ) const;
  
  TMatrix16 operator * ( const TMatrix16& ) const;
  //TMatrix16 operator / ( const TMatrix16& ) const;

  TMatrix16 operator * ( float ) const;
  TMatrix16 operator / ( float ) const;

  //friend TMatrix16 operator * ( float, const TMatrix16& );

  bool operator == ( const TMatrix16& ) const;
  bool operator != ( const TMatrix16& ) const;
};

#pragma pack(pop)

}

extern void SetMatrixIdentity(nsStruct3D::TMatrix16* pV);
extern void SetMatrixIdentity(nsStruct3D::TMatrix9* pV);

extern void SetMatrixRotateX(nsStruct3D::TMatrix16* pV, float ugol);
extern void SetMatrixRotateY(nsStruct3D::TMatrix16* pV, float ugol);
extern void SetMatrixRotateZ(nsStruct3D::TMatrix16* pV, float ugol);

#endif
