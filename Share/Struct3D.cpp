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
TMatrix16* SetMatrixInverse(TMatrix16* pOut,
                            float* pDeterminant,
                            const TMatrix16* pM_ )
{
#ifdef WIN32
  D3DXMATRIX* InOut = (D3DXMATRIX*)pOut;
  const D3DXMATRIX* pM = (const D3DXMATRIX*)pM_;
  D3DXMatrixInverse(InOut,pDeterminant,pM);
#else
#endif
  return pOut;
}
//-------------------------------------------------------------------------
TPlane* SetPlaneFromPointNormal(TPlane* pOut,
                                const TVector3* _pPoint,
                                const TVector3* _pNormal)
{
#ifdef WIN32
  D3DXPLANE* InOut = (D3DXPLANE*)pOut; 
  const D3DXVECTOR3 *pPoint  = (const D3DXVECTOR3*)_pPoint;
  const D3DXVECTOR3 *pNormal = (const D3DXVECTOR3*)_pNormal;

  D3DXPlaneFromPointNormal(InOut,pPoint,pNormal);
#else
#endif
  return pOut;
}
//-------------------------------------------------------------------------
TPlane* SetPlaneFromPoints( TPlane* pOut,
                            const TVector3* _pV1,
                            const TVector3* _pV2,
                            const TVector3* _pV3)
{
#ifdef WIN32
  D3DXPLANE* InOut = (D3DXPLANE*)pOut; 
  const D3DXVECTOR3* pV1 = (const D3DXVECTOR3*)_pV1;
  const D3DXVECTOR3* pV2 = (const D3DXVECTOR3*)_pV2;
  const D3DXVECTOR3* pV3 = (const D3DXVECTOR3*)_pV3;
  D3DXPlaneFromPoints(InOut,pV1,pV2,pV3);
#else
#endif
  return pOut;
}
//-------------------------------------------------------------------------
TVector3* SetPlaneIntersectLine( TVector3* pOut,
                                 const TPlane*   _pP,
                                 const TVector3* _pV1,
                                 const TVector3* _pV2)
{
#ifdef WIN32
  D3DXVECTOR3* InOut = (D3DXVECTOR3*)pOut; 
  const D3DXPLANE*   pP  = (const D3DXPLANE*)_pP;
  const D3DXVECTOR3 *pV1 = (const D3DXVECTOR3 *)_pV1;
  const D3DXVECTOR3 *pV2 = (const D3DXVECTOR3 *)_pV2;
  D3DXPlaneIntersectLine(InOut,pP,pV1,pV2);
#else
#endif
  return pOut;
}
//-------------------------------------------------------------------------
TPlane* SetPlaneTransform( TPlane* pOut,
                            const TPlane*    _pP,
                            const TMatrix16* _pM)
{
#ifdef WIN32
  D3DXPLANE* InOut = (D3DXPLANE*)pOut;
  const D3DXPLANE *pP  = (const D3DXPLANE*) _pP;
  const D3DXMATRIX *pM = (const D3DXMATRIX*)_pM;
  D3DXPlaneTransform(InOut,pP,pM);
#else
#endif
  return pOut;
}
//-------------------------------------------------------------------------
TQuaternion * SetQuaternionMultiply(TQuaternion *pOut,
                                    const TQuaternion *_pQ1,
                                    const TQuaternion *_pQ2)
{
#ifdef WIN32
  D3DXQUATERNION* InOut = (D3DXQUATERNION*)pOut;
  const D3DXQUATERNION* pQ1 = (const D3DXQUATERNION*)_pQ1;
  const D3DXQUATERNION* pQ2 = (const D3DXQUATERNION*)_pQ2;

  D3DXQuaternionMultiply(InOut,pQ1,pQ2);
#else
#endif
  return pOut;
}
//-------------------------------------------------------------------------
TQuaternion * SetQuaternionIdentity(TQuaternion *pOut)
{
#ifdef WIN32
  D3DXQUATERNION* InOut = (D3DXQUATERNION*)pOut;
  D3DXQuaternionIdentity(InOut);
#else
#endif
  return pOut;
}
//-------------------------------------------------------------------------
TQuaternion* SetQuaternionRotationAxis(TQuaternion* pOut,
                                       const TVector3 *_pV,
                                       float Angle)
{
#ifdef WIN32
  D3DXQUATERNION* InOut = (D3DXQUATERNION*)pOut;
  const D3DXVECTOR3* pV = (const D3DXVECTOR3*)_pV;
  D3DXQuaternionRotationAxis(InOut, pV, Angle);
#else
#endif
  return pOut;
}
//-------------------------------------------------------------------------
void SetQuaternionToAxisAngle(const TQuaternion *pQ,
                              TVector3 *pAxis,
                              float *pAngle)
{
#ifdef WIN32
  const D3DXQUATERNION* InOut = (const D3DXQUATERNION*)pQ;
  D3DXVECTOR3* pV = (D3DXVECTOR3*)pAxis;
  
  D3DXQuaternionToAxisAngle(InOut, pV, pAngle);
#else
#endif
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
TVector2::operator float* ()
{
  return (float*)this;
}
//-------------------------------------------------------------------------
TVector2::operator const float* () const
{
  return (const float*)this;
}
//-------------------------------------------------------------------------
TVector2& TVector2::operator += ( const TVector2& v)
{
  x += v.x;
  y += v.y;
  return *this;
}
//-------------------------------------------------------------------------
TVector2& TVector2::operator -= ( const TVector2& v)
{
  x -= v.x;
  y -= v.y;
  return *this;
}
//-------------------------------------------------------------------------
TVector2& TVector2::operator *= ( float v)
{
  x *= v;
  y *= v;
  return *this;
}
//-------------------------------------------------------------------------
TVector2& TVector2::operator /= ( float v)
{
  x /= v;
  y /= v;
  return *this;
}
//-------------------------------------------------------------------------
TVector2 TVector2::operator + () const
{
  return *this;
}
//-------------------------------------------------------------------------
TVector2 TVector2::operator - () const
{
  return *this;
}
//-------------------------------------------------------------------------
TVector2 TVector2::operator + ( const TVector2& v) const
{
  TVector2 res;
  res.x = x + v.x;
  res.y = y + v.y;
  return res;
}
//-------------------------------------------------------------------------
TVector2 TVector2::operator - ( const TVector2& v) const
{
  TVector2 res;
  res.x = x - v.x;
  res.y = y - v.y;
  return res;
}
//-------------------------------------------------------------------------
TVector2 TVector2::operator * ( float v) const
{
  TVector2 res;
  res.x = x * v;
  res.y = y * v;
  return res;
}
//-------------------------------------------------------------------------
TVector2 TVector2::operator / ( float v) const
{
  TVector2 res;
  res.x = x / v;
  res.y = y / v;
  return res;
}
//-------------------------------------------------------------------------
bool TVector2::operator == ( const TVector2& v) const
{
  return ((v.x==x)&&(v.y==y));
}
//-------------------------------------------------------------------------
bool TVector2::operator != ( const TVector2& v) const
{
  return ((v.x!=x)||(v.y!=y));
}
//-------------------------------------------------------------------------
bool TVector2::operator > ( const TVector2& v) const
{
  if(x>v.x) return true;// высший
  else if(x<v.x) return false;
  //
  if(y>v.y) 
    return true;// низший

  return false;
}
//-------------------------------------------------------------------------
bool TVector2::operator < ( const TVector2& v) const
{
  if(x<v.x) return true;// высший
  else if(x>v.x) return false;
  //
  if(y<v.y) 
    return true;// низший

  return false;
}
//-------------------------------------------------------------------------
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
bool TVector3::operator > ( const TVector3& v) const
{
  if(x>v.x) return true; // высший разряд
  else if(x<v.x) return false;
  //---------------------------------
  if(y>v.y) return true;
  else if(y<v.y) return false;
  //---------------------------------
  if(z>v.z)// низший разряд
    return true;
  return false;
}
//-------------------------------------------------------------------------
bool TVector3::operator < ( const TVector3& v) const
{
  if(x<v.x) return true; // высший разряд
  else if(x>v.x) return false;
  //---------------------------------
  if(y<v.y) return true;
  else if(y>v.y) return false;
  //---------------------------------
  if(z<v.z)// низший разряд
    return true;
  return false;
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
TPlane::TPlane( const float* pF)
{
  a = pF[0];
  b = pF[1];
  c = pF[2];
  d = pF[3];
}
//-------------------------------------------------------------------------
TPlane::TPlane( float _a, float _b, float _c, float _d )
{
  a = _a;
  b = _b;
  c = _c;
  d = _d;
}
//-------------------------------------------------------------------------
TPlane::operator float* ()
{
  return (float*)this;
}
//-------------------------------------------------------------------------
TPlane::operator const float* () const
{
  return (const float*)this;
}
//-------------------------------------------------------------------------
TPlane& TPlane::operator *= ( float v)
{
  a *= v;
  b *= v;
  c *= v;
  d *= v;
  return *this;
}
//-------------------------------------------------------------------------
TPlane& TPlane::operator /= ( float v)
{
  a /= v;
  b /= v;
  c /= v;
  d /= v;
  return *this;
}
//-------------------------------------------------------------------------
TPlane TPlane::operator + () const
{
  TPlane res = *this;
  res.a += a;
  res.b += b;
  res.c += c;
  res.d += d;
  return res;
}
//-------------------------------------------------------------------------
TPlane TPlane::operator - () const
{
  TPlane res = *this;
  res.a -= a;
  res.b -= b;
  res.c -= c;
  res.d -= d;
  return res;
}
//-------------------------------------------------------------------------
TPlane TPlane::operator * ( float v) const
{
  TPlane res;
  res.a *= v;
  res.b *= v;
  res.c *= v;
  res.d *= v;
  return res;
}
//-------------------------------------------------------------------------
TPlane TPlane::operator / ( float v) const
{
  TPlane res;
  res.a /= v;
  res.b /= v;
  res.c /= v;
  res.d /= v;
  return res;
}
//-------------------------------------------------------------------------
bool TPlane::operator == ( const TPlane& p) const
{
  return ((p.a==a)&&(p.b==b)&&(p.c==c)&&(p.d==d));
}
//-------------------------------------------------------------------------
bool TPlane::operator != ( const TPlane& p) const
{
  return ((p.a!=a)||(p.b!=b)||(p.c!=c)||(p.d!=d));
}
//-------------------------------------------------------------------------
// сформировать уравнение прямой при пересечении 2-ух плоскостей
bool TLine::FindAndSetIntersect(TPlane* pP1,TPlane* pP2)
{
  //1 найдем тип прямой (зависит от того равны ли коэффициенты плоскости нулю)


  if((*pP1==*pP2)           ||// линия выродилась в плоскость
     (*pP1==TPlane(0,0,0,0))||// вырожденное уравнение
     (*pP2==TPlane(0,0,0,0)))
  {
    SetType(eUndef);
    return false;
  }
  //  через Х
  if(pP1->c*pP2->b!=pP2->c*pP1->b)
  {
    float div = pP1->c*pP2->b - pP1->b*pP2->c;// c1*b2-b1*c2
    a = (pP1->a*pP2->c - pP1->c*pP2->a)/div;// a1*c2-c1*a2
    b = (pP1->d*pP2->c - pP1->c*pP2->d)/div;// d1*c2-d2*c1
    c = (pP1->b*pP2->a - pP1->a*pP2->b)/div;// b1*a2-b2*a1
    d = (pP1->b*pP2->d - pP1->d*pP2->b)/div;// b1*d2-b2*d1
    SetType(eX);
  }// через Z
  else if(pP1->a*pP2->b!=pP2->a*pP1->b)
  {
    float div = pP1->b*pP2->a - pP1->a*pP2->b;// b1*a2-a1*b2 
    a = (pP1->c*pP2->b - pP1->b*pP2->c)/div;// c1*b2-b1*c2 
    b = (pP1->d*pP2->b - pP1->b*pP2->d)/div;// d1*b2-d2*b1
    div *= -1.0f;;// b2*a1-a2*b1 
    c = (pP1->c*pP2->a - pP1->a*pP2->c)/div;// c1*a2-a1*c2
    d = (pP1->d*pP2->a - pP1->a*pP2->d)/div;// d1*a2-a1*d2
    SetType(eZ);
  }// через Y
  else
  {
    float div = pP1->a*pP2->c - pP1->c*pP2->a;// a1*c2-c1*a2
    a = (pP1->c*pP2->b - pP1->b*pP2->c)/div;// c1*b2-b1*c2
    b = (pP1->c*pP2->d - pP1->d*pP2->c)/div;// c1*d2-d1*c2
    div *= -1.0f;// c1*a2-a1*c2
    c = (pP1->a*pP2->b - pP1->b*pP2->a)/div;// a1*b2-b1*a2
    d = (pP1->a*pP2->d - pP1->d*pP2->a)/div;// a1*d2-d1*a2
    SetType(eY);    
  }
  return true;
}
//-------------------------------------------------------------------------
// найти 2 вектора(нормальные) на прямой от точки
bool TLine::FindVector(TVector3* pOut1, TVector3* pOut2, bool do_normal)
{
  if(mType==eUndef)
    return false;

  TVector3 betweenPoint;
  switch(mType)
  {
    case eX:
      betweenPoint.x = 0.0f;
      Calc(betweenPoint.x,betweenPoint.y,betweenPoint.z);

      pOut1->x = 1.0f;
      Calc(pOut1->x,pOut1->y,pOut1->z);
      pOut2->x = -1.0f;
      Calc(pOut2->x,pOut2->y,pOut2->z);
      break;
    case eY:
      betweenPoint.y = 0.0f;
      Calc(betweenPoint.y,betweenPoint.x,betweenPoint.z);

      pOut1->y = 1.0f;
      Calc(pOut1->y,pOut1->x,pOut1->z);
      pOut2->y = -1.0f;
      Calc(pOut2->y,pOut2->x,pOut2->z);
      break;
    case eZ:
      betweenPoint.z = 0.0f;
      Calc(betweenPoint.z,betweenPoint.x,betweenPoint.y);

      pOut1->z = 1.0f;
      Calc(pOut1->z,pOut1->x,pOut1->y);
      pOut2->z = -1.0f;
      Calc(pOut2->z,pOut2->x,pOut2->y);
      break;
  }

  *pOut1 -= betweenPoint;
  *pOut2 -= betweenPoint;

  if(do_normal)
  {
    SetVec3Normalize(pOut1,pOut1);
    SetVec3Normalize(pOut2,pOut2);
  }
  return true;
}
//-------------------------------------------------------------------------
void TLine::Calc(float arg, float& res1, float& res2)
{
  res1 = a*arg + b;
  res2 = c*arg + d;
}
//-------------------------------------------------------------------------
TQuaternion::TQuaternion( const float * pF)
{
  x = pF[0];
  y = pF[1];
  z = pF[2];
  w = pF[3];
}
//-------------------------------------------------------------------------
TQuaternion::TQuaternion( float _x, float _y, float _z, float _w )
{
  x = _x;
  y = _y;
  z = _z;
  w = _w;
}
//-------------------------------------------------------------------------
TQuaternion::operator float* ()
{
  return (float*)this;
}
//-------------------------------------------------------------------------
TQuaternion::operator const float* () const
{
  return (const float*)this;
}
//-------------------------------------------------------------------------
TQuaternion& TQuaternion::operator += ( const TQuaternion& q)
{
  *this = *this + q;
  return *this;
}
//-------------------------------------------------------------------------
TQuaternion& TQuaternion::operator -= ( const TQuaternion& q)
{
  *this = *this - q;
  return *this;
}
//-------------------------------------------------------------------------
TQuaternion& TQuaternion::operator *= ( const TQuaternion& q)
{
  *this = *this * q;
  return *this;
}
//-------------------------------------------------------------------------
TQuaternion& TQuaternion::operator *= ( float v)
{
  *this = *this * v;
  return *this;
}
//-------------------------------------------------------------------------
TQuaternion& TQuaternion::operator /= ( float v)
{
  *this = *this / v;
  return *this;
}
//-------------------------------------------------------------------------
TQuaternion  TQuaternion::operator + () const
{
  return *this;
}
//-------------------------------------------------------------------------
TQuaternion TQuaternion::operator - () const
{
  return *this;
}
//-------------------------------------------------------------------------
TQuaternion TQuaternion::operator + ( const TQuaternion& q) const
{
  TQuaternion res;
  res.x = x + q.x;
  res.y = y + q.y;
  res.z = z + q.z;
  res.w = w + q.w;
  return res;
}
//-------------------------------------------------------------------------
TQuaternion TQuaternion::operator - ( const TQuaternion& q) const
{
  TQuaternion res;
  res.x = x - q.x;
  res.y = y - q.y;
  res.z = z - q.z;
  res.w = w - q.w;
  return res;
}
//-------------------------------------------------------------------------
TQuaternion TQuaternion::operator * ( const TQuaternion& q) const
{
  TQuaternion res;
  SetQuaternionMultiply(&res,this,&q);
  return res;
}
//-------------------------------------------------------------------------
TQuaternion TQuaternion::operator * ( float v) const
{
  TQuaternion res;
  res.x = x * v;
  res.y = y * v;
  res.z = z * v;
  res.w = w * v;
  return res;
}
//-------------------------------------------------------------------------
TQuaternion TQuaternion::operator / ( float v) const
{
  TQuaternion res;
  res.x = x / v;
  res.y = y / v;
  res.z = z / v;
  res.w = w / v;
  return res;
}
//-------------------------------------------------------------------------
bool TQuaternion::operator == ( const TQuaternion& q) const
{
  return ((q.x==x)&&(q.y==y)&&(q.z==z)&&(q.w==w));
}
//-------------------------------------------------------------------------
bool TQuaternion::operator != ( const TQuaternion& q) const
{
  return ((q.x!=x)||(q.y!=y)||(q.z!=z)||(q.w!=w));
}
//-------------------------------------------------------------------------
