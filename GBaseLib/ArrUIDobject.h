#ifndef ArrUIDobjectH
#define ArrUIDobjectH

#include "UIDobject.h"
#include "hArray.h"

//=============================================================================
//=============================================================================
//=============================================================================
DECLARE_CLASS( ArrUIDobject )

class GBASELIB_EI TArrUIDobject : public TArrayItemObject<TUIDobject>
{
  typedef TArrayItemObject<TUIDobject> inherited;

public:
  TArrUIDobject();
  TArrUIDobject( TCmpFunc Cmp, int s = 0 );
//  TArrUIDobject( RArrUIDobject arr );

  int DelByUID( guint32 uid );

  // ����� � ������� ������� � �������� ���������� ���������������
  // ���� ������ ����������� �� TUIDobject::SortUID - �� ��������� �������� ������,
  // ����� ����� ������������ ���������������� ���������
  int FindIndexUID( guint32 uid ) const;
  PUIDobject FindByUID( guint32 uid ) const
    {
      int index = FindIndexUID( uid );
      return (PUIDobject)operator[]( index );
    }

  bool  CheckUniqueUID( int *pInd1, int *pInd2 );

  PUIDobject operator [] ( int i ) const     // ���������� �������
    { return (PUIDobject)inherited::operator[]( i ); }

  // �������� ������� ������� ��� ������������������ � ��
  void MarkAsSyncro();

  // ����������� ������� ������ ������� � ��������� �� <source>, � �����������
  // ������������� ��������� � ���� ������, �������� ��� �������� ������ �������
  // ��������������� TStateRecord::tsXXX
  // ���������: ��������� �������� ������ ������� ���� �����������, source �� �������
  bool CopyChanges( TArrUIDobject &source );

  // ����������� ������� ������ ������� � ��������� �� <source>, � ���������
  // ������������� ��������� � ���� ������, �������� ��� �������� ������ �������
  // ��������������� TStateRecord::tsXXX
  // ���������: ��������� �������� ������ ������� ���� �����������, source - ����
  bool MoveChanges( TArrUIDobject &source );

  void UnlinkChild( int type );
};
//=============================================================================
//=============================================================================
//=============================================================================
#endif
