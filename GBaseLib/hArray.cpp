#include <string.h>
#include "hArray.h"
#include <stdlib.h>

#include "BL_Debug.h"
#include <glib.h>

#define AH_START_SIZE   8
//-----------------------------------------------------------------------------
TArrayHeap::TArrayHeap( TCmpFunc _CmpFunc, int _sizeElem, int _size )
{
  size = 0;
  count = 0;
  sizeElem = _sizeElem;
  BL_ASSERT( sizeElem );
  pointer = NULL;
  trimEnabled = true;
  if( !_CmpFunc ) sorted = false;
  else {
    sorted = true;
    CmpFunc = _CmpFunc;
  }
  if( _size ) reserv( _size );
}
//-----------------------------------------------------------------------------
TArrayHeap::TArrayHeap( const TArrayHeap& src ):TObject(src)
{
  size = 0;
  count = 0;
  sizeElem = src.sizeElem;
  pointer = NULL;
  trimEnabled = src.trimEnabled;
  if( !src.CmpFunc ) sorted = false;
  else {
    sorted = true;
    CmpFunc = src.CmpFunc;
  }
  
  if( src.Count() && reserv( src.size ) )
    memcpy( pointer, src.pointer, src.SizeObj() );
}
//-----------------------------------------------------------------------------
TArrayHeap::~TArrayHeap()
{
  Done();
}
//-----------------------------------------------------------------------------
void TArrayHeap::Done()
{
  trimEnabled = true;
  if( count ) Clear();
  if( pointer )
  {
    g_free( pointer );
    pointer = NULL;
    size = 0;
  }
}
//-----------------------------------------------------------------------------
bool TArrayHeap::reserv( int needSize )
{
  bool res = false;
  int newSize;
  if( needSize < size )   // ������ ������
  {
    if( !needSize )
    {
      g_free( pointer );
      pointer = NULL;
      newSize = 0;
      res = true;
    }
    else
    {
      newSize = needSize;
      int fullSize = newSize * sizeElem;
      void* p = g_try_realloc( pointer, fullSize );
      if( p )
      {
        pointer = p;
        res = true;
      }
    }
  }
  else                    // ���������� ������
  {
    newSize = size ? size : AH_START_SIZE;
    while( newSize < needSize ) newSize *= 2;
    int fullSize = newSize * sizeElem;
    if( pointer )
    {
      void* p = g_try_realloc( pointer, fullSize );
      if( p )
      {
        pointer = p;
        res = true;
      }
    }
    else
    {
      pointer = g_try_malloc( fullSize );
      if( pointer ) res = true;
    }
  }
  if( res ) size = newSize;
  return res;

}
//-----------------------------------------------------------------------------
// ���������� ����� ��������� �������
bool TArrayHeap::SetCount( int cnt )
{
  if( (unsigned)size >= (unsigned)cnt || reserv( cnt ) )
  {
    count = cnt;
    return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
// ��������� ������ ������������� �� �������� "Cmp"
// � ������������� ���� "doSort"
void TArrayHeap::Sort( TCmpFunc Cmp, bool doSort )
{
  //if( Cmp ) 
  CmpFunc = Cmp;
  if( CmpFunc )
  {
    // ���� ������� ����������� � ���� ������ - ���������
    if( doSort && pointer )
      qsort( pointer, count, sizeElem, CmpFunc );

    // � ����� ������ - ������� ������ �������������
    sorted = true;
  }
  else sorted = false;
}

//-----------------------------------------------------------------------------
// �����������
//bool TArrayHeap::operator=( Robject o )
bool TArrayHeap::Assign( const TObject& obj )
{
  Clear();
  RArrayHeap ra = (RArrayHeap)obj;
  sizeElem  = ra.sizeElem; // ������ ��������
  sorted    = ra.sorted;   // ������� ��������������� �������
  CmpFunc   = ra.CmpFunc;  // ������� ���������� �������
  return AppendArray( ra );
}
//-----------------------------------------------------------------------------
bool TArrayHeap::AppendArray( const TArrayHeap& ra )
{
  if( SetCount( count + ra.count ) )
  {
    if( pointer && ra.pointer )
      memcpy( pointer, ra.pointer, ra.SizeObj() );
    return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
void TArrayHeap::Clear()
{
  count = 0;
  softTrim();
}
//-----------------------------------------------------------------------------
bool TArrayHeap::save( TIOStream& od )  const
{
  if( inherited::save( od ) &&
      od.put( &count, sizeof( count ) ) &&
      od.put( &sizeElem, sizeof( sizeElem ) ) )
  {
    if( !pointer || od.put( pointer, SizeObj() ) )
      return true;
  }
 return false;
}
//-----------------------------------------------------------------------------
bool TArrayHeap::load( TIOStream& od )
{
  int cnt, si;
  if( inherited::load( od ) &&
      od.get( &cnt, sizeof( cnt ) ) &&
      od.get( &si, sizeof( si ) ) &&
      SetCount( cnt ) )
  {
    if( !count || od.get( pointer, SizeObj() ) )
      return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
int TArrayHeap::length() const
{
  return inherited::length() +
         sizeof( count ) +
         sizeof( sizeElem ) +
         SizeObj();
}
//-----------------------------------------------------------------------------
PObject TArrayHeap::dubl()  const
{
  return NULL;
}
//-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////
// ����� ������� ����������� ������� � ������������� �������
// key [in] - ��������(�������) ��������
// cmp [in] - ������� ���������. ���� �� ������ - ������� ����� ������������������
// ���������: ������ ���������� �������� �������, ����������� ���������. ��� NO_DWORD
// ins [out] - ������� ��� ������� � ������ ��������, ����������� ���������. ��� NO_DWORD
//             (�������������� ��������)
int TArrayHeap::FastSearch( void const * const key, int* ins, TCmpFunc Cmp ) const
{
  int res = -1;
  // ������� ������� ��������� �� ������
  if( !Cmp )
    // ���� ���� ��� ������������������ - ���������� ��
    if( sorted ) Cmp = CmpFunc;
    else
    {
      // ����� - "�� ������"
      if( ins ) *ins = -1;
      return -1;
    }
  if( pointer )
    res = bsearch( pointer, count, sizeElem, key, Cmp, ins );
  else
    if( ins ) *ins = 0;
  return res;
}
/////////////////////////////////////////////////////////////////////
// ���������������� �����
int TArrayHeap::StepSearch( const void* p, TCmpFunc Cmp ) const
{
  if( Cmp )
  {
    guint8* pbyte = (guint8*)pointer;
    if( pbyte )
    {
      for( int i = 0; i < count; i ++, pbyte += sizeElem )
        if( Cmp( pbyte, p ) == 0 )
          return i;
    }
  }
  return -1;
}
/////////////////////////////////////////////////////////////////////
// ��������� �������� ���������� �������� <func> �� ���� ��������� �������
// param - ������������ �������� ���������� ��� ������
void TArrayHeap::ForAny( const TForFunc func, void* param ) const
{
  guint8* pbyte = (guint8*)pointer;
  if( pbyte )
  {
    int i = count;
    while( i-- )
    {
      func( pbyte, param );
      pbyte += sizeElem;
    }
  }
}
/////////////////////////////////////////////////////////////////////
// ��������� �� ���� ��������� ������� <func> � ���������� <param>
// ��������� ���� ��������� <func> == true
// ���������: ��� ���������� <func> == true
bool TArrayHeap::Apply( const TApplyFunc func, void* param ) const
{
  guint8* pbyte = (guint8*)pointer;
  if( pbyte )
  {
    int i = count;
    while( i-- )
    {
      if( !func( pbyte, param ) ) return false;
      pbyte += sizeElem;
    }
  }
  return true;
}
////////////////////////////////////////////////////////////////////////////////
// ������� ���-�� ����� ���������� (������������ ������� a[i-1] <= a[i]
// pError - [out] ������ ������� ����������� ������� ���������� (����� ���� NULL)
// ���������: ���-�� �����
int TArrayHeap::CountErrSort( int *pError ) const
{
  int err = 0;
  if( sorted && CmpFunc )
  {
    guint8* ptr = (guint8*)base();
    int cnt = count;
    if( cnt-- > 1 )
    {
      while( cnt-- )
      {
        if( CmpFunc( ptr, ptr+sizeElem ) > 0 )
        {
          err++;
          if( pError )
            *pError = count - cnt - 2;
        }
        ptr += sizeElem;
      }
    }
  }
  return err;
}
////////////////////////////////////////////////////////////////////////////////
// ����������� ����������� ������� � ������� �������� ����������
// index - ������ ����������� �������
// bufElem - ��������������� ����� �������� � ������� ������ (=sizeElem)
// ���������: ����� ������� ������� � ������. ������ ������������ �� ��������
// ���������� ����������� �������� ����������.
int TArrayHeap::CheckChange( int index, void* bufElem )
{
  guint8* ptr = (guint8*)base();
  int count = Count();
  // ������ ������� ���������� � ������� ���������� ������ ������������ ��������
  if( sorted && ((unsigned)index < (unsigned)count) )
  {
    // �������� �� ��������� ����� ������������ �������
    memcpy( bufElem, ptr + index * sizeElem, sizeElem );                      // p = ptr[index]

    int insert;

    // ����� �������� � ������ �� ���� �������
    if( (index > 0) && (CmpFunc( ptr + (index - 1)*sizeElem, bufElem ) > 0 ) )
    {
      // ����� �������� � ������ �� ��� �������
      if( (index > 1) && (CmpFunc( ptr + (index - 2)*sizeElem, bufElem ) > 0 ) )
      {
        // ������������ ������� ������� � ���������� �
        // ��� ��������� ��������� �������� �����
        bsearch( ptr, index - 2, sizeElem, bufElem, CmpFunc, &insert );
        // ������������ �� ����� �������
        memmove( ptr + (insert + 1)*sizeElem, ptr + insert*sizeElem, (index - insert) * sizeElem );
        memcpy( ptr + insert*sizeElem, bufElem, sizeElem );                   // ptr[insert] = p;
        return insert;
      }
      else
      {
        // �������� � ������ �� ���� �������
        memcpy( ptr + index*sizeElem, ptr + (index-1)*sizeElem, sizeElem );   // ptr[index] = ptr[index-1];
        memcpy( ptr + (index-1)*sizeElem, bufElem, sizeElem );                // ptr[index-1] = p;
        return index - 1;
      }
    }
    else
    { // ����� �������� � ����� �� ���� �������
      if( ((unsigned)index < (unsigned)count-1) && (CmpFunc( ptr + (index + 1)*sizeElem, bufElem ) < 0 ) )
      {
        // ����� �������� � ����� �� ��� �������
        if( ((unsigned)index < (unsigned)count-2) && (CmpFunc( ptr + (index + 2)*sizeElem, bufElem ) < 0 ) )
        {
          // ������������ ������� ������� � ���������� �
          // ��� ��������� ��������� �������� �����
          bsearch( ptr + (index + 3)*sizeElem, count - index - 3, sizeElem, bufElem, CmpFunc, &insert );
          // ������������ �� ����� �������
          memmove( ptr + index*sizeElem, ptr + (index + 1)*sizeElem, (insert + 3 - 1)*sizeElem );
          insert += index + 3 - 1;
          memcpy( ptr + insert*sizeElem, bufElem, sizeElem );                 // ptr[insert] = p;
          return insert;
        }
        else
        {
          // �������� � ������ �� ���� �������
          memcpy( ptr + index*sizeElem, ptr + (index+1)*sizeElem, sizeElem ); // ptr[index] = ptr[index+1];
          memcpy( ptr + (index+1)*sizeElem, bufElem, sizeElem );              // ptr[index+1] = p;
          return index + 1;
        }
      }
	}
  }
  return index;
}

//=============================================================================
//=============================================================================
//=============================================================================
// ������� ��� ������� �������, ������� � ������� ����������������� ���������
void TArrayObject::DeleteObjects()
{
  PObject* tPtr = baseItem();
  if( tPtr )
  {
    int i = count;
    while( i -- )
    {
      if( *tPtr ) delete *tPtr;
      tPtr++;
    }
  }
}
//-----------------------------------------------------------------------------
// �������� ������
void TArrayObject::Clear()
{
  DeleteObjects();
  inherited::Clear();
}
//-----------------------------------------------------------------------------
// ������� �������� �� <cnt> ���������, ������� � <first>
void TArrayObject::Delete( int first, int cnt )
{
  if( (unsigned)first < (unsigned)count )
  {
    if( first + cnt > count ) cnt = count - first;
    PObject* tPtr = baseItem() + first;
    int i = cnt;
    while( i-- )
    {
      if( *tPtr ) delete *tPtr;
      tPtr++;
    }
    inherited::Delete( first, cnt );
  }
}
//-----------------------------------------------------------------------------
// ���������� ����� ��������� �������
bool TArrayObject::SetCount( int cnt )
{
  int i;
  if( (unsigned)cnt < (unsigned)count )
  {
    PObject* ptr = baseItem() + cnt;
    i = count - cnt;
    while( i-- ) {
      if( *ptr ) delete *ptr;
      ptr++;
    }
    return inherited::SetCount( cnt );
  }
  else
  {
    int oldCount = count;
    if( inherited::SetCount(cnt) )
    {
      PObject* ptr = baseItem() + oldCount;
      i = count - oldCount;
      while( i-- ) *ptr++ = NULL;
      return true;
    }
  }
  return false;
}
//-----------------------------------------------------------------------------
bool TArrayObject::save( TIOStream& od ) const
{
  int cnt = count;
  if( TObject::save( od ) &&
      od.put( &cnt, sizeof( cnt ) ) )
  {
    if( !count ) return true;
    PObject* pp = baseItem();
    int i = count;
    while( i -- )
    {
      if( !*pp || !(*pp)->save( od ) ) return false;
      pp++;
    }
    return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
// �������� �������
// ����������� ������ ������ ��������� ������� ������ ����
// ����� ��������� ������ �������������� ���� �� ����
// �������, ����� �� ��� ������� ������� ����� ������.

bool TArrayObject::load( TIOStream& od )
{
  int cnt;
  if( TObject::load( od ) &&
      od.get( &cnt, sizeof( cnt ) ) )
  {
    if( !cnt ) { Clear(); return true; }
    if( count )
    {
      PObject pTmp = baseItem()[0]->dubl();
      if( pTmp )
      {
        Clear();
        if( reserv( cnt ) )
        {
          PObject* padd = baseItem();
          while( cnt -- )
          {
            PObject pTmp1 = pTmp->dubl();
            if( pTmp1 )
            {
              if( pTmp1->load( od ) )
              {
                *padd++ = pTmp1;
                count++;
                continue;
              }
            }
            delete pTmp;
            return false;
          }
        }
        delete pTmp;
        return true;
      }
    }
  }
  return false;
}
//-----------------------------------------------------------------------------
int TArrayObject::length() const
{
  int size = TObject::length() +
              sizeof( count );
  if( count )
  {
    PObject* pp = baseItem();
    int i = count;
    while( i -- )
    {
      if( *pp ) size += (*pp)->length();
      pp++;
    }
  }
  return size;
}
//-----------------------------------------------------------------------------
//bool TArrayObject::Assign( Robject obj )
//{
//  DeleteObjects();
//  count = 0;
//  return AddObjects( (RArrayObject)obj );
//}
//-----------------------------------------------------------------------------
// �������� ����� �������� �� ������� <pa> � ������ ��������� ����������
bool TArrayObject::AddObjects( const TArrayObject &pa )
{
  if( reserv( count + pa.count ) )
  {
    PObject* pFrom  = pa.baseItem();
    for( int i = 0; i < pa.count; i++, pFrom++ )
    {
      PObject tmp = (*pFrom)->dubl();
      if( tmp && tmp->Assign( **pFrom ) )
      {
        if( Add( tmp ) == -1 ) return false;
      }
      else return false;
    }
    return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
// �������� ����� �������� �� ������� <pa> � ����� �������
bool TArrayObject::AppendArray( const TArrayHeap& ra )
{
  RArrayObject ro = (RArrayObject)ra;
  if( reserv( count + ro.Count() ) )
  {

    PObject* pTo    = baseItem() + count;
    PObject* pFrom  = ro.baseItem();
    for( int i = 0; i < ro.count; i++, pFrom++, pTo++ )
    {
      PObject tmp = (*pFrom)->dubl();
      if( tmp && tmp->Assign( **pFrom ) )
      {
        *pTo = tmp;
        count++;
      }
      else return false;
    }
    return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
//bool TArrayObject::operator = ( Robject o )
//{
//  Clear();
//  return AddObjects( (RArrayObject)o );
//}
////-----------------------------------------------------------------------------
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

int bsearch( const void *base, int count, int width, const void *key, TCmpFunc cmp, int* ins )
{
  BL_ASSERT( width );
  if( !count ) {
    if( ins ) *ins = 0;
    return -1;
  }
  int lower = 0;
  int upper = count - 1;
  int pos;
  void *cur;
  int res;
  while( (unsigned)lower < (unsigned)upper && upper != -1 )
  {
    int middle = (lower + (upper - lower) / 2);
    cur = (guint8*)base + middle * width;
    res = cmp( cur, key );
    if( res == 0 ) {
      pos = middle;
      goto l_Found;
    }
    if( res < 0 ) lower = middle + 1;
    else upper = middle - 1;
  }// while
  cur = (guint8*)base + lower * width;
  res = cmp( cur, key );
  if( ( lower == upper ) && res == 0 )
  {
    pos = lower;
    goto l_Found;
  }
  // ���������� �� ����������, ���������� ������� �������:
  if( ins )
  {
    cur = (guint8*)base + lower * width;
    if( cmp( cur, key ) < 0 ) *ins = lower + 1;
    else *ins = lower;
  }
  return -1;

l_Found: // ������� � ������� ��������:
  while( pos && cmp( (guint8*)base + (pos - 1)*width, key ) == 0 ) pos--;
  if( ins ) *ins = pos;
  return pos;
}
//=============================================================================
//=============================================================================
//=============================================================================

