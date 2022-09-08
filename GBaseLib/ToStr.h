/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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


#ifndef ToStrH
#define ToStrH

#include "TypeDef.h"

//------------------------------------------------------------------------------
// ����� ����������� ������������� �������� �������� � ����������� �� ������ (�� 3 �������)
class GBASELIB_EI TN2A
{
  char thousands_sep;	// ������-����������� ����� �� ��������
  char decimal_point;	// ������-����������� ������� ����� �����
  bool isInit;	// ���������� ����������������
  
  inline void init() { if( !isInit ) getSystemParam(); }
public:  
  // �������� ��������� ��������� ������������
  void getSystemParam();

  // �������� ������-����������� ����� ���������� �������� ����� �� ������
  // ������ ������������ � ��������� ���������
  char thousandsSep();

  // ���������� ������-����������� ����� � �������� �����
  // ��������� - ���������� ��������
  char thousandsSep( char value );

  // �������� ������-����������� ������� ����� �����
  char floatSep();

  // ���������� ������-����������� ������� ����� �����
  // ��������� - ���������� ��������
  char floatSep( char value );

  // �������� �������� ������������ �� ������
  // src - �������� ������
  // dst - ����� ��� �������������� ������. 
  //       ���� �� �����, �� �������������� �������� ������. 
  // ��������� - "���������" �������������� ������
  char* clear( const char *src, char *dst = NULL );

public:

  // ������������� �������� �������� � ����������� �� ������
  // st  - ����� ��� ������
  // val - �������� ��������
  // ���������: ������ � ����������� (=st)
  char* conv( char* st, int val );
  char* conv( char* st, guint32 val );
  char* conv( char* st, gint64 val );
  char* conv( char* st, guint64 val );
  // cntFloat - ���-�� ������� ��������
  char* conv( char* st, double val, int cntFloat );

};
//------------------------------------------------------------------------------
// ���������� ������ �������������� �����
extern GBASELIB_EI TN2A N2A;

//------------------------------------------------------------------------------
// �������� ������-����������� ����� ���������� �������� ����� �� �������
// ������ ������������ � ��������� ���������
inline char ThousandsSep_Get()              { return N2A.thousandsSep(); }

// ���������� ������-����������� ����� � �������� �����
// ��������� - ���������� ��������
inline char ThousandsSep_Set( char value )  { return N2A.thousandsSep( value ); }

// �������� ������-����������� ������� ����� �����
inline char FloatSep_Get()                  { return N2A.floatSep(); }

// ���������� ������-����������� ������� ����� �����
// ��������� - ���������� ��������
inline char FloatSep_Set( char value )      { return N2A.floatSep(value); }

// �������� �������� ������������ �� ������
// src - �������� ������
// dst - ����� ��� �������������� ������. 
//       ���� �� �����, �� �������������� �������� ������. 
// ��������� - "���������"  ������
inline char* ThousandsSep_Clear( const char *src, char *dst = NULL ) { return N2A.clear(src, dst); }

//------------------------------------------------------------------------------
// ������������� �������� �������� � ����������� �� ������
// st  - ����� ��� ������
// val - �������� ��������
// ���������: ������ � ����������� (=st)
inline char* n2a( char* st, int val )       { return N2A.conv( st, val ); }
inline char* n2a( char* st, guint32 val )   { return N2A.conv( st, val ); }
inline char* n2a( char* st, gint64 val )    { return N2A.conv( st, val ); }
inline char* n2a( char* st, guint64 val )   { return N2A.conv( st, val ); }
// cntFloat - ���-�� ������� ��������
inline char* n2a( char* st, double val, int cntFloat )  { return N2A.conv( st, val, cntFloat ); }
//------------------------------------------------------------------------------

#endif
