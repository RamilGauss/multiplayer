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


#ifndef Struct3DH
#define Struct3DH

#include "TypeDef.h"

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
// 
#define MATRIX16_EQUAL_M_M(a,b) \
CopyMatrix16((float*)&b,(float*)&a);

#define MATRIX16_EQUAL_M_P(a,b) \
CopyMatrix16((float*)b,(float*)&a);

#define MATRIX16_EQUAL_P_M(a,b) \
CopyMatrix16((float*)&b,(float*)a);

#define MATRIX16_EQUAL_P_P(a,b) \
CopyMatrix16((float*)b,(float*)a);
//----------------------------------------------------------------------

namespace nsStruct3D
{

#pragma pack(push, 1)
//----------------------------------------------------------------------------------------
class SHARE_EI TPoint2
{
public:
  unsigned int x;
  unsigned int y;
  TPoint2(){x=0;y=0;}
};
//----------------------------------------------------------------------------------------
class SHARE_EI TVector2 
{
public:
  TVector2() {};
  TVector2( float _x, float _y ){x=_x;y=_y;};

  // casting
  operator float* ();
  operator const float* () const;

  // assignment operators
  TVector2& operator += ( const TVector2& );
  TVector2& operator -= ( const TVector2& );
  TVector2& operator *= ( float );
  TVector2& operator /= ( float );

  // unary operators
  TVector2 operator + () const;
  TVector2 operator - () const;

  // binary operators
  TVector2 operator + ( const TVector2& ) const;
  TVector2 operator - ( const TVector2& ) const;
  TVector2 operator * ( float ) const;
  TVector2 operator / ( float ) const;

  bool operator == ( const TVector2& ) const;
  bool operator != ( const TVector2& ) const;

  bool operator > ( const TVector2& ) const;
  bool operator < ( const TVector2& ) const;

public:
  float x, y;
};
//-------------------------------------------------------------------
class SHARE_EI TVector3
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

  // assignment operators
  TVector3& operator += ( const TVector3& );
  TVector3& operator -= ( const TVector3& );
  TVector3& operator *= ( float );
  TVector3& operator /= ( float );

  // unary operators
  TVector3 operator + () const;
  TVector3 operator - () const;

  // binary operators
  TVector3 operator + ( const TVector3& ) const;
  TVector3 operator - ( const TVector3& ) const;
  TVector3 operator * ( float ) const;
  TVector3 operator / ( float ) const;

  bool operator == ( const TVector3& ) const;
  bool operator != ( const TVector3& ) const;

  bool operator > ( const TVector3& ) const;
  bool operator < ( const TVector3& ) const;
};
//-----------------------------------------------------------------
class SHARE_EI TVector4
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
class SHARE_EI TMatrix9
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
class SHARE_EI TMatrix16
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
  TMatrix16(){};
  TMatrix16( float _11, float _12, float _13, float _14,
             float _21, float _22, float _23, float _24,
             float _31, float _32, float _33, float _34,
             float _41, float _42, float _43, float _44 );

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

  bool operator == ( const TMatrix16& ) const;
  bool operator != ( const TMatrix16& ) const;
};
//----------------------------------------------------------------------------------------
class TPlane
{
public:
  TPlane() {}
  TPlane( const float* );
  TPlane( float a, float b, float c, float d );

  // casting
  operator float* ();
  operator const float* () const;

  // assignment operators
  TPlane& operator *= ( float );
  TPlane& operator /= ( float );

  // unary operators
  TPlane operator + () const;
  TPlane operator - () const;

  // binary operators
  TPlane operator * ( float ) const;
  TPlane operator / ( float ) const;

  //friend TPlane operator * ( float, const TPlane& );

  bool operator == ( const TPlane& ) const;
  bool operator != ( const TPlane& ) const;

  float a, b, c, d;
};
//----------------------------------------------------------------------------------------
class TLine
{
  enum{
      eX,// a = ky, b = by, c = kz, d = bz
      eY,// a = kx, b = bx, c = kz, d = bz
      eZ,// a = kx, b = bx, c = ky, d = by
      eUndef, 
  };

  int mType;
  float a,b,c,d;

public:
  TLine(){mType=eUndef;}

  // сформировать уравнение прямой при пересечении 2-ух плоскостей
  bool FindAndSetIntersect(TPlane* pP1,TPlane* pP2);
  // найти 2 вектора(нормальные) на прямой от точки
  bool FindVector(TVector3* pOut1, TVector3* pOut2, bool do_normal = false);

protected:
  void SetType(int v){mType=v;}
  void Calc(float arg, float& res1, float& res2);
};
class TQuaternion
{

public:
  float x, y, z, w;

  TQuaternion() {}
  TQuaternion( const float * );
  TQuaternion( float x, float y, float z, float w );

  // casting
  operator float* ();
  operator const float* () const;

  // assignment operators
  TQuaternion& operator += ( const TQuaternion& );
  TQuaternion& operator -= ( const TQuaternion& );
  TQuaternion& operator *= ( const TQuaternion& );
  TQuaternion& operator *= ( float );
  TQuaternion& operator /= ( float );

  // unary operators
  TQuaternion  operator + () const;
  TQuaternion  operator - () const;

  // binary operators
  TQuaternion operator + ( const TQuaternion& ) const;
  TQuaternion operator - ( const TQuaternion& ) const;
  TQuaternion operator * ( const TQuaternion& ) const;
  TQuaternion operator * ( float ) const;
  TQuaternion operator / ( float ) const;

  //friend TQuaternion operator * (FLOAT, CONST D3DXQUATERNION& );

  bool operator == ( const TQuaternion& ) const;
  bool operator != ( const TQuaternion& ) const;

};
#pragma pack(pop)
}
//----------------------------------------------------------------------------------------

SHARE_EI extern void CopyMatrix16(float* pSrc, float* pDst);

SHARE_EI extern nsStruct3D::TMatrix16* SetMatrixIdentity(nsStruct3D::TMatrix16* pV);
SHARE_EI extern nsStruct3D::TMatrix9* SetMatrixIdentity(nsStruct3D::TMatrix9* pV);

SHARE_EI extern nsStruct3D::TMatrix16* SetMatrixRotateX(nsStruct3D::TMatrix16* pV, float ugol);
SHARE_EI extern nsStruct3D::TMatrix16* SetMatrixRotateY(nsStruct3D::TMatrix16* pV, float ugol);
SHARE_EI extern nsStruct3D::TMatrix16* SetMatrixRotateZ(nsStruct3D::TMatrix16* pV, float ugol);

SHARE_EI extern nsStruct3D::TMatrix16* SetMatrixRotationYawPitchRoll(nsStruct3D::TMatrix16* pV,
                                                                     float Yaw,
                                                                     float Pitch,
                                                                     float Roll );

SHARE_EI extern nsStruct3D::TMatrix16* SetMatrixRotationAxis(nsStruct3D::TMatrix16 *pOut,
                                                             const nsStruct3D::TVector3 *pV,
                                                             float Angle);

SHARE_EI extern nsStruct3D::TMatrix16* SetMatrixMultiply(nsStruct3D::TMatrix16 *pOut,
                                                         const nsStruct3D::TMatrix16 *pM1,
                                                         const nsStruct3D::TMatrix16 *pM2);

SHARE_EI extern nsStruct3D::TMatrix16* SetMatrixPerspectiveFovLH( nsStruct3D::TMatrix16* pOut, 
                                                                 float fovy, 
                                                                 float Aspect, 
                                                                 float zn, float zf );

SHARE_EI extern nsStruct3D::TMatrix16* SetMatrixInverse(nsStruct3D::TMatrix16* pOut,
                                                        float* pDeterminant,
                                                        const nsStruct3D::TMatrix16* pM );

SHARE_EI extern nsStruct3D::TVector3*  SetVec3TransformCoord(nsStruct3D::TVector3* pOut,
                                                             const nsStruct3D::TVector3* pV,
                                                             const nsStruct3D::TMatrix16* pM);

SHARE_EI extern nsStruct3D::TVector3*  SetVec3Cross( nsStruct3D::TVector3* pOut,
                                                     const nsStruct3D::TVector3* pV1,
                                                     const nsStruct3D::TVector3* pV2);

SHARE_EI extern float SetVec3Dot( const nsStruct3D::TVector3* pV1,
                                  const nsStruct3D::TVector3* pV2);

SHARE_EI extern nsStruct3D::TVector3* SetVec3Normalize(nsStruct3D::TVector3* pOut,
                                                       const nsStruct3D::TVector3* pV);

SHARE_EI extern nsStruct3D::TPlane* SetPlaneFromPointNormal(nsStruct3D::TPlane* pOut,
                                                            const nsStruct3D::TVector3* pPoint,
                                                            const nsStruct3D::TVector3* pNormal);

SHARE_EI extern nsStruct3D::TPlane* SetPlaneFromPoints( nsStruct3D::TPlane* pOut,
                                                        const nsStruct3D::TVector3* pV1,
                                                        const nsStruct3D::TVector3* pV2,
                                                        const nsStruct3D::TVector3* pV3);

SHARE_EI extern nsStruct3D::TVector3* SetPlaneIntersectLine( nsStruct3D::TVector3*       pOut,
                                                             const nsStruct3D::TPlane*   pP,
                                                             const nsStruct3D::TVector3* pV1,
                                                             const nsStruct3D::TVector3* pV2);

SHARE_EI extern nsStruct3D::TPlane* SetPlaneTransform( nsStruct3D::TPlane*       pOut,
                                                       const nsStruct3D::TPlane*    pP,
                                                       const nsStruct3D::TMatrix16* pM);

SHARE_EI extern bool SetBoxBoundProbe(const nsStruct3D::TVector3 *pMin,
                                      const nsStruct3D::TVector3 *pMax,
                                      const nsStruct3D::TVector3 *pRayPosition,
                                      const nsStruct3D::TVector3 *pRayDirection );

SHARE_EI extern bool SetComputeBoundingBox(const nsStruct3D::TVector3 *pFirstPosition,
                                           unsigned int NumVertices,
                                           unsigned int dwStride,
                                           nsStruct3D::TVector3 *pMin,
                                           nsStruct3D::TVector3 *pMax);

SHARE_EI extern bool SetIntersectTri(const nsStruct3D::TVector3 *p0,
                                     const nsStruct3D::TVector3 *p1,
                                     const nsStruct3D::TVector3 *p2,
                                     const nsStruct3D::TVector3 *pRayPos,
                                     const nsStruct3D::TVector3 *pRayDir,
                                     float *pU,
                                     float *pV,
                                     float *pDist);

SHARE_EI extern bool SetComputeBoundingSphere(const nsStruct3D::TVector3 *pFirstPosition,
                                              unsigned int NumVertices,
                                              unsigned int dwStride,
                                              nsStruct3D::TVector3 *pCenter,
                                              float *pRadius);

SHARE_EI extern bool SetSphereBoundProbe(const nsStruct3D::TVector3 *pCenter,
                                         float Radius,
                                         const nsStruct3D::TVector3 *pRayPosition,
                                         const nsStruct3D::TVector3 *pRayDirection);

SHARE_EI extern nsStruct3D::TQuaternion* SetQuaternionMultiply(nsStruct3D::TQuaternion *pOut,
                                                               const nsStruct3D::TQuaternion *pQ1,
                                                               const nsStruct3D::TQuaternion *pQ2);

SHARE_EI extern nsStruct3D::TQuaternion* SetQuaternionIdentity(nsStruct3D::TQuaternion *pOut);

SHARE_EI extern nsStruct3D::TQuaternion* SetQuaternionRotationAxis(nsStruct3D::TQuaternion* pOut,
                                                                   const nsStruct3D::TVector3 *pV,
                                                                   float Angle);

SHARE_EI extern void SetQuaternionToAxisAngle(const nsStruct3D::TQuaternion *pQ,
                                              nsStruct3D::TVector3 *pAxis,
                                              float *pAngle);

#endif
