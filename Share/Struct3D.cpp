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
void SetMatrixIdentity(TMatrix16* pV)
{
  MATRIX16_OP_P(pV,0.0f,=)
  
  pV->_11 = 1.0f;  pV->_22 = 1.0f;  pV->_33 = 1.0f; pV->_44 = 1.0f;
}
//-------------------------------------------------------------------------
void SetMatrixIdentity(TMatrix9* pV)
{
  pV->_11 = 1.0f;  pV->_12 = 0.0f;  pV->_13 = 0.0f;
  pV->_21 = 0.0f;  pV->_22 = 1.0f;  pV->_23 = 0.0f;
  pV->_31 = 0.0f;  pV->_32 = 0.0f;  pV->_33 = 1.0f;
}
//-------------------------------------------------------------------------
void SetMatrixRotateX(TMatrix16* pV, float ugol)
{
#ifdef WIN32
  SET_MATRIX_ROTATE_WIN(pV,ugol,X)
#else
#endif
}
//-------------------------------------------------------------------------
void SetMatrixRotateY(TMatrix16* pV, float ugol)
{
#ifdef WIN32
  SET_MATRIX_ROTATE_WIN(pV,ugol,Y)
#else
#endif
}
//-------------------------------------------------------------------------
void SetMatrixRotateZ(TMatrix16* pV, float ugol)
{
#ifdef WIN32
  SET_MATRIX_ROTATE_WIN(pV,ugol,Z)
#else
#endif
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
  D3DXMATRIX In1,In2,Out;
  
  MATRIX16_EQUAL_M_P(In1,this)
  MATRIX16_EQUAL_M_M(In2,v)

  D3DXMatrixMultiply(&Out,&In1,&In2);
  MATRIX16_EQUAL_P_M(this,Out)
#else
#endif
  return *this;
}
//-------------------------------------------------------------------------
TMatrix16 TMatrix16::operator * ( const TMatrix16& v) const
{
#ifdef WIN32
  TMatrix16 res;
  D3DXMATRIX In1,In2,Out;
  MATRIX16_EQUAL_M_P(In1,this)
  MATRIX16_EQUAL_M_M(In2,v)

  D3DXMatrixMultiply(&Out,&In1,&In2);
  MATRIX16_EQUAL_M_M(res,Out)
#else
#endif
  return res;
}
//-------------------------------------------------------------------------

