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
the "Tanks" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef hArrayH
#define hArrayH

#include "TObject.h"

//-----------------------------------------------------------------------------
// �������� ������� ����������
// p1 � p2 - ��������� �� �������� ������� ���������� ���������
// ��������� ���������:
// <0: p1 < p2
// =0: p1 == p2
// >0: p1 > p2
typedef  int (*TCmpFunc)( const void* p1, const void* p2 );
//-----------------------------------------------------------------------------
// �������� ������� ���������� �������� �� ���� ��������� �������
// pElem - ��������� �� ������� ������� �������
// param - ������������ �������� ���������� ��� ������
typedef  void (*TForFunc)( void* pElem, void* param );
//-----------------------------------------------------------------------------
// �������� ������� ����������������� ���������� �������� � ��������� �������
// ���� ������� �� ������ ������������� ���������
// pElem - ��������� �� ������� ������� �������
// param - ������������ �������� ���������� ��� ������
typedef  bool (*TApplyFunc)( void* pElem, void* param );
//=============================================================================
//=============================================================================
//=============================================================================
//
// ������� ����� ������������ ��������
//
//-----------------------------------------------------------------------------
DECLARE_CLASS( ArrayHeap )

class GBASELIB_EI TArrayHeap : public TObject
{
  typedef TObject inherited;

protected:
  void*     pointer;        // ��������� �� ������
  int       sizeElem;       // ������ ��������
  int       count;          // ���������� ���������
  int       size;           // ���������� ����� ������
  bool      sorted;         // ������� ��������������� �������
  TCmpFunc  CmpFunc;        // ������� ���������� �������
  bool      trimEnabled;    // ����������� ������ ��� ���������� ���-�� ��������� �������

  bool reserv( int s );   // �������� ������ ��� _s_ ���������
  void softTrim();        // ���������� ���������� ������ ��-��� ������ �������� ������� 

  // ����������� ����������� ������� � ������� �������� ����������
  int CheckChange( int index, void* bufElem );

public:

  TArrayHeap( TCmpFunc _CmpFunc = NULL, int _sizeElem = sizeof(char), int _size = 0 );
  TArrayHeap( const TArrayHeap& src );  // ����������� �����������
  virtual ~TArrayHeap();

  virtual void Done();            // ����������� ����������

  int     Count() const;  // ���������� ���������
  void*   base() const;   // ������� ����� ������
  int     SizeObj() const;// ������ ������� � ������

  // ���������� ������� ��������� ��� ���������� ���-�� ��������� �������
  // true  - ����������� ������(  �� ��������� )
  // false - ��������� ������ �����������������
  void    setTrimEnabled( bool enabled )   { trimEnabled = enabled; }

  // ���������� ������ ��-��� ������ �������� �������
  void    Trim();         

  // ��������� ������ ������������� �� �������� _Cmp_
  // � ������������� ���� _doSort_
  void  Sort( TCmpFunc Cmp, bool doSort = true );
  // ��������� �������� ����� ���������������
  void  SetSorted( bool sort ) { sorted = sort; }
  TCmpFunc  GetCmpFunc() const;             // ����������� ������� ����������
  bool  IsSortBy( TCmpFunc cmpFunc ) const; // �������� ��������������� ������� �� ��������� �������

  virtual void  Clear();                    // �������� ���
  virtual bool  SetCount( int cnt );// ���������� ����� ��������� �������

  // �������� ������ �� ������� <ra> � ����� �������
  virtual bool AppendArray( const TArrayHeap& ra );
 
  // ��������� "��������" ����������� ������� � ��� ���������
  TArrayHeap& operator = ( const TArrayHeap& src ) { Assign( (TArrayHeap&)src ); return *this; }

  // ����������� �������
  virtual bool    save( TIOStream& od ) const;
  virtual bool    load( TIOStream& od );
  virtual int     length() const;
  virtual bool    Assign( const TObject& obj );
  virtual PObject dubl() const;

  //////////////////////////////////////////////////////////
  // ����� � �������
  int Search( const void* p, TCmpFunc Cmp = NULL ) const;
  //////////////////////////////////////////////////////////
  // ������� ����� � ������������� �������
  int FastSearch( void const * const p, int* ins = NULL, TCmpFunc Cmp = NULL ) const;
  //////////////////////////////////////////////////////////
  // ���������������� �����
  int StepSearch( const void* p, TCmpFunc Cmp = NULL ) const;
  //////////////////////////////////////////////////////////
  // ��������� �������� ���������� �������� <func> ��������������� �� ���� ��������� �������
  void ForAny( const TForFunc func, void* param = NULL ) const;
  // ��������� �������� ���������� �������� <func> ��������������� � ��������� �������
  // ���� ������� <func> �� ������ ������������� ���������
  bool Apply( const TApplyFunc func, void* param = NULL ) const;

  // ������� ���-�� ����� ���������� (������������ ������� a[i-1] <= a[i]
  int CountErrSort( int *pError = NULL ) const;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// ����������� ������� ����������
inline TCmpFunc TArrayHeap::GetCmpFunc() const
{
  return CmpFunc;
}
//-----------------------------------------------------------------------------
// �������� ��������������� ������� �� ��������� �������
inline bool TArrayHeap::IsSortBy( TCmpFunc cmpFunc ) const
{
  return sorted && (CmpFunc == cmpFunc );
}
//-----------------------------------------------------------------------------
// ������� ����� ������
inline void* TArrayHeap::base() const
{
  return pointer;
}
//-----------------------------------------------------------------------------
// ���������� ���������
inline int TArrayHeap::Count() const
{
  return count;
}
//-----------------------------------------------------------------------------
// ���������� ������ ��-��� ������ �������� �������
inline void TArrayHeap::Trim()
{
  reserv( count );
}
//-----------------------------------------------------------------------------
// �������� ������ �������� �������
inline void TArrayHeap::softTrim()
{
  if( trimEnabled && (size > count*2) )
    reserv( size/2 );
}
//-----------------------------------------------------------------------------
// ������ ������� � ������
inline int TArrayHeap::SizeObj() const
{
  return sizeElem * count;
}
//-----------------------------------------------------------------------------
inline int TArrayHeap::Search( const void* p, TCmpFunc Cmp ) const
{
  return Cmp ? StepSearch( p, Cmp ) : FastSearch( p, NULL, NULL );
}
//=============================================================================
//=============================================================================
//=============================================================================

template< class TItem > 
class TArrayVALUE : public TArrayHeap
{
  typedef TArrayHeap inherited;
public:
  TArrayVALUE( TCmpFunc Cmp = NULL, int s = 0 )
    : TArrayHeap( Cmp, sizeof(TItem), s ) { }

  TArrayVALUE( const TArrayVALUE<TItem>& src )
  : TArrayHeap( src ) { }

  bool operator==( const TArrayVALUE<TItem>& src ) const;

  TItem Delete( const int d );      // ������� �������
  TItem DeleteLast();               // �������� ���������� ��������
  // ������� �������� �� <cnt> ���������, ������� � <first>
  void  Delete( int first, int cnt );
  int   Append( const TItem t );      // �������� ������� � ����� �������
  TItem*Append();                    // ���������� ����� ��� �������
  int   Add( const TItem t );         // �������� �������
  // �������� �������� �� ������� <pa>
  bool  AppendValues( const TArrayVALUE<TItem> &pa ); // � ����� �������
  bool  AddValues( const TArrayVALUE<TItem> &pa );    // � ������ ��������� ����������
  bool  AddFilteredValues( const TArrayVALUE<TItem> &pa, TCmpFunc filter, void*param ); // � �����������

  bool  Insert( int j, const TItem t ); // �������� �������
  TItem Get( int i ) const;             // ���������� �������
  TItem operator[]( int i ) const;      // ���������� �������
  bool  Set( int i, const TItem t );    // ����� �������� ��������
  TItem*baseItem() const;                      // ������� ����� ������
  // ���������� ������� ������� ��� �������� �� ��������� � ��������
  TItem Item( int i ) const { return baseItem()[i]; }
  // ���������� ��������� �������
  TItem Last() const { return operator[](count-1); }

  bool  Swap( int, int );        // ������������ ���������
  virtual PObject dubl();

  // ����������� ����������� ������� � ������� �������� ����������
  int   CheckChange( int index );

  // ����� ������ ������� �������� �� �������� ����������� � ��������
  int FindValue( const TItem item ) const;
  int FindValue( const TItem item, int start, const bool revers = false ) const;
  // ��������� ������� �������� � �������
  bool  HasValue( const TItem item  ) const { return FindValue( item ) != -1; }

  // �������� ������������ (���������� ����������) �������� � �������
  // �������������� �������� ���������:
  // *pInd1, *pInd2 - ������� ��������-������
  // ���������: ������������ ��������� �������
  bool CheckUnique( int * const pInd1, int * const pInd2 ) const;

  // ������� ���������� �� �����������
  // ��������� ���������:   -1: p1 < p2; 0: p1 == p2; 1: p1 > p2
  static int SortUp( const void* p1, const void* p2 );

  // ������� ���������� �� ��������
  // ��������� ���������:   1: p1 < p2; 0: p1 == p2; -1: p1 > p2
  static int SortDown( const void* p1, const void* p2 );
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template< class TItem > 
class TArrayVAR : public TArrayHeap
{
	typedef TArrayHeap inherited;
public:
  TArrayVAR( TCmpFunc Cmp = NULL, int s = 0 )
    : TArrayHeap( Cmp, sizeof(TItem), s ) { }

  TArrayVAR( const TArrayVAR<TItem>& src )
    : TArrayHeap( src ) { }

  bool operator==( const TArrayVAR<TItem>& src ) const;
  bool operator!=( const TArrayVAR<TItem>& src ) const { return !operator==( src ); }

  void  Delete( int d );                  // ������� �������
  int   Append( const TItem &t );         // �������� ������� � ����� �������
  int   Add( const TItem &t );            // �������� �������
  bool  Insert( int j, const TItem &t );  // �������� �������
  TItem*Insert( int j );                  // �������� �������
  bool  elem( int i, TItem &t ) const;    // ���������� �������
  bool  Set( int i, const TItem& t );     // ����� �������� ��������
  TItem* Get( int i );                    // ���������� �������
  const TItem* Get( int i ) const;        // ���������� �������
  TItem* operator[]( int i );             // ���������� �������
  const TItem* operator[]( int i ) const; // ���������� �������
  TItem* baseItem() const;                // ������� ����� ������
  // ���������� ������� ������� ��� �������� �� ��������� � ��������
  TItem*  Item( int i )             { return baseItem() + i; }
  const TItem*  Item( int i ) const { return baseItem() + i; }

  bool  Swap( int , int );      // ������������ ���������
  virtual PObject dubl() const;

  // ����� ������ ������� �������� �� �������� ����������� � ��������
  int FindValue( const TItem &item  ) const;
  // ��������� ������� �������� � �������
  bool  HasValue( const TItem &item  ) const { return FindValue( item ) != -1; }

  // ����������� ����������� ������� � ������� �������� ����������
  int     CheckChange( int index );
  TItem*  Append();                   // ���������� ����� ��� �������
  void    DeleteLast();               // �������� ���������� ��������
  TItem*  Last();                     // ���������� ��������� �������
  const TItem* Last() const;          // ���������� ��������� �������
  TItem*  New();                      // ���������� ������� � �������
  /*
  � ������� ������ ���� ����� ����������� �� ��������� � �������������� ������� new:
  1) TItem()
  2) void * operator new(size_t, void * buf) { return buf; }
  */
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template< class TItem >
bool TArrayVALUE<TItem>::operator==( const TArrayVALUE<TItem>& src ) const
{
  if( count != src.count ) return false;
  for( int i = 0; i < count; i++ )
    if( !(*Item(i) == *src.Item(i)) )
      return false;
  return true;  
}
//-----------------------------------------------------------------------------
// ������� ������� ��� �������� <d>
// ���������: ��������� �������
template< class TItem >
TItem TArrayVALUE<TItem>::Delete( int d )
{
  if( (unsigned)d < (unsigned)count )
  {
    TItem* tPtr = baseItem() + d;
    TItem retVal = *tPtr;
    int i = count - d - 1;
    while( i-- )
    {
      *tPtr = *(tPtr + 1);
      tPtr++;
    }
    count--;
    softTrim();
    return retVal;
  }
  return 0;
}
//-----------------------------------------------------------------------------
// �������� ���������� ��������
template<class TItem>
TItem TArrayVALUE<TItem>::DeleteLast()
{
  return Delete( count - 1 );
}
//-----------------------------------------------------------------------------
// ������� �������� �� <cnt> ���������, ������� � <first>
template<class TItem>
void TArrayVALUE<TItem>::Delete( int first, int cnt )
{
  if( (unsigned)first < (unsigned)count )
  {
    if( first + cnt > count ) cnt = count - first;
    TItem* tDst = baseItem() + first;
    TItem* tSrc = tDst + cnt;
    int i = count - first - cnt;
    while( i-- )
      *tDst++ = *tSrc++;
    count -= cnt;
    softTrim();
  }
}
//-----------------------------------------------------------------------------
// �������� ������� � ����� �������
template<class TItem>
int TArrayVALUE<TItem>::Append( const TItem t )
{
  if( (count == size) && !reserv( size + 1) ) return -1;
  baseItem()[count] = t;
  return count++;
}
//-----------------------------------------------------------------------------
// ���������� ����� ��� �������
template<class TItem>
TItem* TArrayVALUE<TItem>::Append()
{
  if( (count == size) && !reserv( size + 1 ) ) return NULL;
  count++;
  return ((TItem*)pointer) + count - 1;
}
//-----------------------------------------------------------------------------
// �������� �������
template<class TItem>
int TArrayVALUE<TItem>::Add( const TItem t )
{
  if( !sorted ) return Append( t );
  int ins;
  FastSearch( &t, &ins );
  return Insert( ins, t ) ? ins : -1;
}
//-----------------------------------------------------------------------------
// �������� �������
template<class TItem>
bool TArrayVALUE<TItem>::Insert( int j, const TItem t )
{
  if( (unsigned)j > (unsigned)count ||
    ( (count == size) &&
    !reserv(size + 1) ) ) return false;
  TItem* tPtr = baseItem() + count;
  int i = count - j;
  while( i-- )
  {
    *tPtr = *(tPtr - 1);
    tPtr--;
  }
  baseItem()[ j ] = t;
  count++;
  return true;
}

//-----------------------------------------------------------------------------
// ���������� �������
template<class TItem>
TItem TArrayVALUE<TItem>::Get( int i ) const
{
  return ( (unsigned)i < (unsigned)count ) ? baseItem()[i] : 0;
}
//-----------------------------------------------------------------------------
// ���������� �������
template<class TItem>
TItem TArrayVALUE<TItem>::operator[]( int i ) const { return Get( i ); }
//-----------------------------------------------------------------------------
// ����� �������� ��������
template<class TItem>
bool TArrayVALUE<TItem>::Set( int i, const TItem t )
{
  if( (unsigned)i < (unsigned)count )
  {
    baseItem()[ i ] = t;
    return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
// ������� ����� ������
template<class TItem>
TItem* TArrayVALUE<TItem>::baseItem() const
{ return (TItem*)base(); }

//-----------------------------------------------------------------------------
// �������� �������� �� ������� <pa> � ����� �������
template< class TItem >
bool TArrayVALUE<TItem>::AppendValues( const TArrayVALUE<TItem> &pa )
{
  if( reserv( count + pa.count ) )
  {
    int i = pa.count;
    const TItem* pSrc = pa.baseItem();
    TItem* pDst = baseItem() + count;
    while( i-- )
      *pDst++ = *pSrc++;
    count += pa.count;
    return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
// �������� �������� �� ������� <pa> � ������ ��������� ����������
template< class TItem >
bool TArrayVALUE<TItem>::AddValues( const TArrayVALUE<TItem> &pa )
{
  if( reserv( count + pa.count ) )
  {
    int i = pa.count;
    const TItem* pSrc = pa.baseItem();
    while( i-- )
      if( Add( *pSrc++ ) == -1 )
        return false;
    return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
// �������� �������� �� ������������ ������� <pa> � ������ ��������� ����������
template< class TItem >
bool TArrayVALUE<TItem>::AddFilteredValues( const TArrayVALUE<TItem> &pa, TCmpFunc filterFunc, void*param )
{
  if( reserv( count + pa.count ) )
  {
    int i = pa.count;
    const TItem* pSrc = pa.baseItem();
    while( i-- )
    {
      const TItem p = *pSrc++;
      if( filterFunc( p, param ) == 0 )
        if( Add( p ) == -1 )
          return false;
    }
    return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
// ������������ ���������
template<class TItem>
bool TArrayVALUE<TItem>::Swap( int i, int j )
{
  if( (unsigned)i < (unsigned)count && (unsigned)j < (unsigned)count )
  {
    if( i != j )
    {
      TItem* pArr = baseItem();
      TItem buf = pArr[i];
      pArr[i] = pArr[j];
      pArr[j] = buf;
    }
    return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
template<class TItem>
PObject TArrayVALUE<TItem>::dubl()
{
  return new TArrayVALUE<TItem>;
}
//-----------------------------------------------------------------------------
// ����������� ����������� ������� � ������� �������� ����������
template<class TItem>
int TArrayVALUE<TItem>::CheckChange( int index )
{
  TItem buf;
  return inherited::CheckChange( index, &buf );
}
//-----------------------------------------------------------------------------
// ����� ������ ������� �������� �� �������� ����������� � ��������
// p - ������� ��������
// ���������: ������ ������� ��� -1
template<class TItem>
int TArrayVALUE<TItem>::FindValue( const TItem item  ) const
{
  int cnt = count;
  const TItem* ptr = baseItem();
  while( cnt-- )
    if( *ptr++ == item )
      return count - cnt - 1;
  return -1;
}
//-----------------------------------------------------------------------------
// ����� ������ ������� �������� �� �������� ����������� � ��������
// p - ������� ��������
// start - ������ ������ ������
// revers - ����������� ������: false - � ������, true - � ����� �������
// ���������: ������ ������� ��� -1
template<class TItem>
int TArrayVALUE<TItem>::FindValue( const TItem item, int start, const bool revers ) const
{
  if( revers )
  {
    if( (start == -1) || ((unsigned)start >= (unsigned)count) ) start = count - 1;
    int cnt = start + 1;

    TItem* ptr = baseItem() + start;
    while( cnt-- )
      if( *ptr-- == item )
        return cnt;
  }
  else
  {
    if( start == -1 ) start = 0;
    else
    {
	  if( (unsigned)start >= (unsigned)count ) 
		return -1;
	}
    int cnt = count - start;

    TItem* ptr = baseItem() + start;
    while( cnt-- )
      if( *ptr++ == item )
        return count - cnt - 1;
  }
  return -1;
}
//------------------------------------------------------------------------------
// �������� ������������ (���������� ����������) �������� � �������
// �������������� �������� ���������:
// *pInd1, *pInd2 - ������� ��������-������
// ���������: ������������ ��������� �������
template<class TItem>
bool TArrayVALUE<TItem>::CheckUnique( int * const pInd1, int* const pInd2 ) const
{
  int cnt = count;
  TItem *pp = baseItem();
  while( cnt-- )
  {
    TItem p = *pp++;

    int cntCheck = cnt;
    TItem *ppCheck = pp;
    while( cntCheck-- )
      if( *ppCheck++ == p )
      {
        if( pInd1 ) *pInd1 = count - cnt - 1;
        if( pInd2 ) *pInd2 = count - cntCheck - 1;
        return false;
      }
  }
  return true;
}
//------------------------------------------------------------------------------
// ������� ���������� �� �����������
// ��������� ���������:   -1: p1 < p2; 0: p1 == p2; 1: p1 > p2
template<class TItem>
int TArrayVALUE<TItem>::SortUp( const void* p1, const void* p2 )
{
  return ( *(TItem*)p1 < *(TItem*)p2 ) ? -1 :
         ( *(TItem*)p1 > *(TItem*)p2 ) ? 1 : 0;
}
//------------------------------------------------------------------------------
// ������� ���������� �� ��������
// ��������� ���������:   1: p1 < p2; 0: p1 == p2; -1: p1 > p2
template<class TItem>
int TArrayVALUE<TItem>::SortDown( const void* p1, const void* p2 )
{
  return ( *(TItem*)p1 < *(TItem*)p2 ) ? 1 :
         ( *(TItem*)p1 > *(TItem*)p2 ) ? -1 : 0;
}
//=============================================================================
//=============================================================================
//=============================================================================

template< class TItem >
bool TArrayVAR<TItem>::operator==( const TArrayVAR<TItem>& src ) const
{
  if( count != src.count ) return false;
  for( int i = 0; i < count; i++ )
    if( !(*Item(i) == *src.Item(i)) )
      return false;
  return true;  
}
//-----------------------------------------------------------------------------
template< class TItem >
void TArrayVAR<TItem>::Delete( int d )
{
  if( (unsigned)d < (unsigned)count )
  {
    TItem* tPtr = baseItem();
    memmove( tPtr + d, tPtr + d + 1, sizeof(TItem) * (count - 1 - d) );
    count--;
    softTrim();
  }
}

//-----------------------------------------------------------------------------
// �������� ������� � ����� �������
template<class TItem>
int TArrayVAR<TItem>::Append( const TItem& t )
{
  if( (count == size) && !reserv( size + 1 ) ) return -1;

  TItem* tPtr = baseItem();
  tPtr[ count ] = t;
  return count++;
}
//-----------------------------------------------------------------------------
// �������� �������
template<class TItem>
int TArrayVAR<TItem>::Add( const TItem &t )
{
  if( !sorted ) return Append( t );
  int ins;
  FastSearch( &t ,&ins );
  return Insert( ins, t ) ? ins : -1;
}
//-----------------------------------------------------------------------------
// �������� �������
template<class TItem>
TItem* TArrayVAR<TItem>::Insert( int j )
{
  if( j > count ||
    ( (count == size) &&
    !reserv(size + 1) ) ) return NULL;

  TItem* tPtr = baseItem();
  memmove( tPtr + j + 1, tPtr + j, sizeof(TItem) * (count-j) );
  count++;
  return tPtr + j;
}
//-----------------------------------------------------------------------------
// �������� �������
template<class TItem>
bool TArrayVAR<TItem>::Insert( int j, const TItem &t )
{
  TItem* p = Insert( j );
  if( p )
  {
    *p = t;
    return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
// ���������� �������
template<class TItem>
bool TArrayVAR<TItem>::elem( int i, TItem &t ) const
{
  if( (unsigned)i < (unsigned)count )
  {
    t = baseItem()[ i ];
    return true;
  }
  return false;
}

//-----------------------------------------------------------------------------
// ������� ����� ������
template<class TItem>
TItem* TArrayVAR<TItem>::baseItem() const
{
  return (TItem*)base();
}

//-----------------------------------------------------------------------------
// ���������� �������
template<class TItem>
TItem* TArrayVAR<TItem>::operator[]( int i )
{
  return ( (unsigned)i < (unsigned)count ) ? baseItem() + i : NULL;
}

//-----------------------------------------------------------------------------
// ���������� �������
template<class TItem>
const TItem* TArrayVAR<TItem>::operator[]( int i ) const
{
  return ( (unsigned)i < (unsigned)count ) ? baseItem() + i : NULL;
}

//-----------------------------------------------------------------------------
// ���������� �������
template<class TItem>
TItem* TArrayVAR<TItem>::Get( int i )
{
  return ( (unsigned)i < (unsigned)count ) ? baseItem() + i : NULL;
}

//-----------------------------------------------------------------------------
// ���������� �������
template<class TItem>
const TItem* TArrayVAR<TItem>::Get( int i ) const
{
  return ( (unsigned)i < (unsigned)count ) ? baseItem() + i : NULL;
}

//-----------------------------------------------------------------------------
// ����� �������� ��������
template<class TItem>
bool TArrayVAR<TItem>::Set( int i, const TItem &t )
{
  if( i < count )
  {
    baseItem()[ i ] = t;
    return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
// ������������ ���������
template<class TItem>
bool TArrayVAR<TItem>::Swap( int i, int j )
{
  if( (unsigned)i < (unsigned)count && (unsigned)j < (unsigned)count )
  {
    if( i != j )
    {
      TItem* pArr = baseItem();
      TItem buf = pArr[i];
      pArr[i] = pArr[j];
      pArr[j] = buf;
    }
    return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
template<class TItem>
PObject TArrayVAR<TItem>::dubl() const
{
  return new TArrayVAR<TItem>( CmpFunc );
}
//-----------------------------------------------------------------------------
// ����� ������ ������� �������� �� �������� ����������� � ��������
// p - ������� ��������
// ���������: ������ ������� ��� NO_DWORD
template<class TItem>
int TArrayVAR<TItem>::FindValue( const TItem &item  ) const
{
  int cnt = count;
  const TItem* ptr = baseItem();
  while( cnt-- )
  {
    if( *ptr++ == item )
      return count - cnt - 1;
  }
  return -1;
}
//-----------------------------------------------------------------------------
// ����������� ����������� ������� � ������� �������� ����������
template<class TItem>
int TArrayVAR<TItem>::CheckChange( int index )
{
  TItem buf;
  return inherited::CheckChange( index, &buf );
}
//-----------------------------------------------------------------------------
// ���������� ����� ��� �������
template<class TItem>
TItem* TArrayVAR<TItem>::Append()
{
  if( (count == size) && !reserv( size + 1 ) ) return NULL;
  count++;
  return ((TItem*)pointer) + count - 1;
}
//-----------------------------------------------------------------------------
// ���������� ����� ��� �������
template<class TItem>
TItem* TArrayVAR<TItem>::New()
{
  if( (count == size) && !reserv( size + 1 ) ) return NULL;
  count++;
  return new(baseItem() + count - 1) TItem;
}

//-----------------------------------------------------------------------------
// �������� ���������� ��������
template<class TItem>
void TArrayVAR<TItem>::DeleteLast()
{
  Delete( count - 1 );
}
//-----------------------------------------------------------------------------
// ���������� ��������� �������
template<class TItem>
TItem* TArrayVAR<TItem>::Last()
{
  return this->operator[]( count - 1 );
}
//-----------------------------------------------------------------------------
// ���������� ��������� �������
template<class TItem>
const TItem* TArrayVAR<TItem>::Last() const
{
  return this->operator[]( count - 1 );
}

//=============================================================================
//=============================================================================
//=============================================================================

DECLARE_CLASS( ArrayObject )

class GBASELIB_EI TArrayObject : public TArrayVALUE<PObject>
{
  typedef TArrayVALUE<PObject> inherited;
  TArrayHeap& operator = ( const TArrayHeap& ) { return *this; }
protected:
  void DeleteObjects();
public:
  TArrayObject( TCmpFunc Cmp = NULL, int s = 0 )
    : inherited( Cmp, s )
  {      }

  virtual ~TArrayObject() { Done(); }

  void  Delete( int d );    // ������� �������
  // ������� �������� �� <cnt> ���������, ������� � <first>
  void  Delete( int first, int cnt );

  virtual void Clear();             // ������� �������
  virtual bool SetCount( int cnt ); // ���������� ���������� ��������� � �������

  PObject Unlink( int n );  // ��������� ������� �� ������� ��� ��������
  // ��������� ��� �������� �������� �� <cnt> ���������, ������� � <first>
  void  Unlink( int first, int cnt );
  void  Unlink(void);         // ��������� ��� �������� �� ������� ��� ��������

  // �������� ����� �������� �� ������� <pa> � ������ ��������� ����������
  bool AddObjects( const TArrayObject &pa );

  // �������� ������(����� ��������) �� ������� <ra> � ����� �������
  virtual bool AppendArray( const TArrayHeap& obj );

  //  bool AppendObjects( const RArrayObject pa );

//  const TArrayObject& operator=( const TArrayObject& src );   // �����������

  // ����������� �������
  virtual bool  save(TIOStream&) const;
  virtual bool  load(TIOStream&);
  virtual int   length() const;
//  virtual bool  Assign( Robject obj );
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// ������� �������
inline void TArrayObject::Delete( int d )
{
  PObject p = inherited::Delete( d );
  if( p ) delete p;
}
//-----------------------------------------------------------------------------
// ��������� ��� �������� �������� �� <cnt> ���������, ������� � <first>
inline void TArrayObject::Unlink( int first, int cnt )
{
  inherited::Delete( first, cnt );
}
//-----------------------------------------------------------------------------
// ��������� ��� �������� �� ������� ��� ��������
inline void TArrayObject::Unlink(void)
{
  inherited::Clear();
}
//-----------------------------------------------------------------------------
// ��������� ������� �� ������� ��� ��������
// ���������: �������� ��������
inline PObject TArrayObject::Unlink( int d )
{
  return inherited::Delete( d );
}
//-----------------------------------------------------------------------------

//=============================================================================
//=============================================================================
//=============================================================================
template <class TItem > class TArrayItemObject : public TArrayObject
{
  typedef TArrayObject inherited;
public:
  TArrayItemObject( TCmpFunc Cmp = NULL, int s = 0 )
    : inherited( Cmp, s )
  {      }

  virtual ~TArrayItemObject() { Done(); }

  // ������� �������
  void Delete( int d )                { inherited::Delete( d ); }
  // ������� �������� �� <cnt> ���������, ������� � <first>
  void  Delete( int first, int cnt )  { inherited::Delete( first, cnt ); }

  // ��������� ������� �� ������� ��� ��������
  TItem* Unlink( int n )    { return (TItem*)inherited::Unlink( n ); }
  // ��������� ��� �������� �������� �� <cnt> ���������, ������� � <first>
  void  Unlink( int first, int cnt ) { inherited::Unlink( first, cnt ); }
  // ��������� ��� �������� �� ������� ��� ��������
  void  Unlink(void)                 { inherited::Unlink(); }

  // ���������� ����� ��� �������
  TItem* Append()                   { return (TItem*)inherited::Append(); }
  // �������� ������� � ����� �������
  int Append( TItem* t )            { return inherited::Append( t ); }

  // ���������� �������
  TItem* Get( int i ) const { return (TItem*)inherited::Get( i ); }

  // ���������� �������
  TItem* operator[]( int i ) const { return Get( i ); }

  // ������� ����� ������
  TItem** baseItem() const           { return (TItem**)inherited::baseItem(); }

  // ���������� ������� ������� ��� �������� �� ��������� � ��������
  TItem*  Item( int i ) const       { return baseItem()[i]; }

  // ���������� ��������� �������
  TItem*  Last() const              { return Get( count-1 ); }
};
//=============================================================================
//=============================================================================
//=============================================================================

typedef TArrayVALUE<gint8>    TArrayINT8;     DECLARE_TYPE( ArrayINT8 )
typedef TArrayVALUE<gint16>   TArrayINT16;    DECLARE_TYPE( ArrayINT16 )
typedef TArrayVALUE<guint16>  TArrayUINT16;   DECLARE_TYPE( ArrayUINT16 )
typedef TArrayVALUE<gint32>   TArrayINT32;    DECLARE_TYPE( ArrayINT32 )
typedef TArrayVALUE<guint32>  TArrayUINT32;   DECLARE_TYPE( ArrayUINT32 )
typedef TArrayVALUE<gint64>   TArrayINT64;    DECLARE_TYPE( ArrayINT64 )
typedef TArrayVALUE<guint64>  TArrayUINT64;   DECLARE_TYPE( ArrayUINT64 )

typedef TArrayVALUE<float>    TArrayFLOAT;    DECLARE_TYPE( ArrayFLOAT )
typedef TArrayVALUE<double>   TArrayDOUBLE;   DECLARE_TYPE( ArrayDOUBLE )

//=============================================================================
//=============================================================================
//=============================================================================

// ����� ������� ����������� ������� � ������������� �������
// base [in] - ����� �������,
// count [in] - ���-�� ��������� � �������
// width [in] - ������ ��������� � �������
// key [in] - ��������(�������) ��������
// cmp [in] - ������� ���������
// ���������: ������ ���������� �������� �������, ����������� ���������. ��� NO_DWORD
// ins [out] - ������� ��� ������� � ������ ��������, ����������� ���������
//             (�������������� ��������)
int GBASELIB_EI bsearch( const void *base, int count, int width, const void *key,
                      TCmpFunc cmp, int* ins = NULL );

//-----------------------------------------------------------------------------

//void BASELIB hArray_CheckClearedHeap();


#endif

