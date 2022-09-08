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
