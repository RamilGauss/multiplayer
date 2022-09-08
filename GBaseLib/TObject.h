#ifndef TObjectH
#define TObjectH

#include "BL_IOStream.h"

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

DECLARE_CLASS( Object )

// ������� ����� ��������
class GBASELIB_EI TObject
{
public:
  TObject() {}
  TObject( const TObject& ) {}
  virtual ~TObject() {}

  // �������� ���� �� ��������� ������� <obj>
  virtual bool operator==( const TObject& ) const
    { return false; }

  // ��������� ������ � �����
  virtual bool save( TIOStream& ) const
    { return true; }

  // ��������� ������ �� ������
  virtual bool load( TIOStream& )
    { return true; }

  // ��������� ������ ����������� ����� �������
  // ���������: ���-�� ����
  virtual int length() const
    { return 0; }

  // ������� ��������� ������� ������ ����
  virtual PObject dubl() const
    { return new TObject (); }

  // ����������� � ���� ������ <obj>
  virtual bool Assign( const TObject& )
    { return true; }

};

#endif
