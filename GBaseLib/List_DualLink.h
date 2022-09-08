#ifndef List_DualLinkH
#define List_DualLinkH

//--------------------------------------------------------------------------
// ������� ����������� ������
template< typename TItem >
class TDLL_ItemT
{
public:
  TItem* mPrev;
  TItem* mNext;
  TDLL_ItemT(void) { mPrev = mNext = 0; }
  
  TItem* first() const;   // ��������� ������� ������
  TItem* last() const;    // ��������� ������� ������

};
//--------------------------------------------------------------------------
// ����������  ������, ��������������� �� ����������� ����������/�������� ��������� 
// � ������������ ������� ������
template< class TItem >
class TDLL_ListT
{
  TItem* mFirst;
  TItem* mLast;
public:

  TDLL_ListT(void) { mFirst = mLast = 0; }

  // ��� �������� ������� - �������� ������
  //~TSLL_ListT()           { deleteAll(); }

  // ���������� ��������� � ������
  int size() const;

  // ������ - ����
  bool empty() const       { return mFirst == 0; }

  // ��������� ������� ������
  TItem* first() const     { return mFirst; }

  // ��������� ������� ������
  TItem* last() const      { return mLast; }

  // ������� ��� �������� ������
  void deleteAll();

  // �������� ������� � ������ ������
  void prepend( TItem* item );

  // �������� ������� � ����� ������
  void append( TItem* item );

  // ��������� ������ ������� �� ������
  // ���������: ����������� �������
  TItem* removeFirst();

  // ��������� ��������� ������� �� ������
  // ���������: ����������� �������
  TItem* removeLast();

  // ��������� �� ������ ������� <item>
  void removeOne( TItem* item );

  // ��������� ��� �������� �� ������
  void removeAll();

  // �������� ������� <item> � ������ ����� ��������� �������� <pos>
  void insertAfter( TItem* pos, TItem* item );

  // �������� ������� <item> � ������ ����� �������� ��������� <pos>
  void insertBefore( TItem* pos, TItem* item );

  // �������� ������� � ������ � ������������ � �������� ����������
  void insert( TItem* item );

  // ��������������� ������� �������� � ������ � ������������ � �������� ����������
  void checkSortPos( TItem* item );
};
//--------------------------------------------------------------------------

#include "List_DualLink_p.h"

#endif // List_DualLinkH
