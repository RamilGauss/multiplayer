#ifndef List_SingleLinkH
#define List_SingleLinkH

#include "BL_IOStream.h"
//--------------------------------------------------------------------------
// ������� ������������ ������
template< typename TItem >
class TSLL_ItemT
{
public:
  TItem* mNext;
  TSLL_ItemT(void) { mNext = 0; }

  // ��������� ������� ������
  TItem* last() const;
};
//--------------------------------------------------------------------------
// ����������� ������, ��������������� �� ����������� ���������� ��������� � ������ ������
template< class TItem >
class TSLL_ListT
{
protected:
  TItem* mFirst;
public:

  TSLL_ListT(void) { mFirst = 0; }

  // ��� �������� ������� - �������� ������
  //~TSLL_ListT()           { deleteAll(); }

  // ������ - ����
  bool empty() const       { return mFirst == 0; }
  
  // ���������� ��������� � ������
  int size() const;

  // ������ ������� ������
  TItem* first() const     { return mFirst; }
  
  // ��������� ������� ������
  TItem* last() const      { return mFirst ? mFirst->last() : NULL; }

  // ������� ��� �������� ������
  void deleteAll();

  // �������� ������� � ������ ������
  void prepend( TItem* item );

  // �������� � ������ ������ �������� �� ������� ������
  void prepend( TSLL_ListT* list );
  
  // ��������� ������ ������� �� ������
  // ���������: ����������� �������
  TItem* removeFirst();

  // ��������� �� ������ ������� <item>
  void removeOne( TItem* item );

  // ��������� ��� �������� �� ������
  void removeAll();

  // ��������� ������ � �����
  bool save( TIOStream& od ) const;

  // ��������� ������ �� ������
  bool load( TIOStream& od );
};
//--------------------------------------------------------------------------
// ����������� ������, ��������������� �� ����������� ���������� ��������� 
// ��� � ������ ������, ��� � � �����
template< class TItem >
class TSLL_ListET : public TSLL_ListT<TItem>
{
  typedef TSLL_ListT<TItem> inherited;
protected:
  TItem* mLast;
public:

  TSLL_ListET(void)   { mLast = 0; }

  // ��������� ������� ������
  TItem* last()  const        { return mLast; }

  // ������� ��� �������� ������
  void deleteAll();

  // �������� ������� � ������ ������
  void prepend( TItem* item );

  // �������� ������� � ����� ������
  void append( TItem* item );

  // �������� ������ � ����� ������
  void appendList( TItem* item );
  void appendList( TSLL_ListET<TItem>& list );

  // ��������� ������ ������� �� ������
  // ���������: ����������� �������
  TItem* removeFirst();

  // ��������� �� ������ ������� <item>
  void removeOne( TItem* item );

  // ��������� ��� �������� �� ������
  void removeAll();

  // ��������� ������ � �����
  //bool save( ROutData od ) const; -  ��������� ������������� ������

  // ��������� ������ �� ������
  bool load( TIOStream& od );
};
//--------------------------------------------------------------------------

#include "List_SingleLink_p.h"

#endif
