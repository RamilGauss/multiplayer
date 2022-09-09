/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef List_DualLink_pH
#define List_DualLink_pH


//--------------------------------------------------------------------------
//---------------               TDLL_ItemT          ------------------------
//--------------------------------------------------------------------------
template< class TItem >
TItem* TDLL_ItemT<TItem>::last() const
{
  TItem* p = this;
  TItem* pn;
  while( (pn = p->mNext) != NULL ) p = pn;
  return p;
}
//--------------------------------------------------------------------------
template< class TItem >
TItem* TDLL_ItemT<TItem>::first() const
{
  TItem* p = this;
  TItem* pn;
  while( (pn = p->mPrev) != NULL ) p = pn;
  return p;
}
//--------------------------------------------------------------------------
//---------------               TDLL_ListT          ------------------------
//--------------------------------------------------------------------------
// ���������� ��������� � ������
template< class TItem >
int TDLL_ListT<TItem>::size() const
{
  int n = 0;
  TItem *p = mFirst;
  while( p ) { p = p->mNext; n++; }
  return n;
}
//--------------------------------------------------------------------------
template< class TItem >
void TDLL_ListT<TItem>::deleteAll()
{
  if( !mFirst ) return;

  TItem* p = mFirst;
  while( p )
  {
    TItem* pn = p->mNext;
    delete p;
    p = pn;
  }
  mFirst = mLast = NULL;
}
//--------------------------------------------------------------------------
// �������� ������� � ������ ������
template< class TItem >
inline void TDLL_ListT<TItem>::prepend( TItem* item )   
{ 
  item->mPrev = NULL;
  item->mNext = mFirst; 
  if( mFirst )
    mFirst->mPrev = item;
  else mLast = item;

  mFirst = item; 
}
//--------------------------------------------------------------------------
// �������� ������� � ����� ������
template< class TItem >
inline void TDLL_ListT<TItem>::append( TItem* item )   
{ 
  item->mNext = NULL; 
  if( !mFirst ) 
  {
    item->mPrev = NULL;
    mFirst = mLast = item; 
  }
  else          
  {
    mLast->mNext = item;
    item->mPrev = mLast;
    mLast = item;
  }
}
//--------------------------------------------------------------------------
// ��������� ������ ������� �� ������
template< class TItem >
inline TItem* TDLL_ListT<TItem>::removeFirst()
{
  if( mFirst ) 
  { 
    TItem* p = mFirst;
    mFirst = mFirst->mNext;
    if( mFirst ) 
      mFirst->mPrev = NULL;
    else
      mLast = NULL;
    return p;
  }
  return NULL;
}
//--------------------------------------------------------------------------
// ��������� ��������� ������� �� ������
template< class TItem >
inline TItem* TDLL_ListT<TItem>::removeLast()
{
  if( mLast ) 
  { 
    TItem* p = mLast;
    mLast = mLast->mPrev;
    if( mLast )
      mLast->mNext = NULL;
    else
      mFirst = NULL;
    return p;
  }
  return NULL;
}
//--------------------------------------------------------------------------
// ��������� ��� �������� ������
template< class TItem >
void TDLL_ListT<TItem>::removeOne( TItem* item )
{
  if( mFirst == item )
  {
    if( mLast == item )
      mFirst = mLast = NULL;
    else
    {
      mFirst = item->mNext;
      mFirst->mPrev = NULL;
    }
  }
  else
  {
    if( mLast == item )
    {
      mLast = item->mPrev;  
      mLast->mNext = NULL;
    }
    else
    {
      TItem* next = item->mNext;
      TItem* prev = item->mPrev;  // != NULL

      next->mPrev = prev;
      prev->mNext = next;
    }
  }
}
//--------------------------------------------------------------------------
// ��������� ��� �������� �� ������
template< class TItem >
inline void TDLL_ListT<TItem>::removeAll()
{
  mFirst = mLast = NULL;
}
//--------------------------------------------------------------------------
// �������� ������� <item> � ������ ����� ��������� �������� <pos>
template< class TItem >
inline void TDLL_ListT<TItem>::insertAfter( TItem* pos, TItem* item )
{
  if( pos->mNext )     // ������� � �������� ������
  {
    item->mNext = pos->mNext;
    item->mPrev = pos;

    pos->mNext->mPrev = item;
    pos->mNext = item;
  }
  else                 // ���������� � ����� ������
  {
    item->mNext = NULL;
    item->mPrev = mLast;
    mLast->mNext = item;
    mLast = item;
  }
}
//--------------------------------------------------------------------------
// �������� ������� <item> � ������ ����� �������� ��������� <pos>
template< class TItem >
inline void TDLL_ListT<TItem>::insertBefore( TItem* pos, TItem* item )
{
  if( pos->mPrev )     // ������� � �������� ������
  {
    item->mPrev = pos->mPrev; 
    item->mNext = pos; 
    
    pos->mPrev->mNext = item;
    pos->mPrev = item;
  }
  else                // ������� � ������ ������
  {
    item->mPrev = NULL;
    item->mNext = mFirst; 
    mFirst->mPrev = item;
    mFirst = item; 
  }
}
//--------------------------------------------------------------------------
// �������� ������� � ������ � ������������ � �������� ����������
template< class TItem >
inline void TDLL_ListT<TItem>::insert( TItem* item )
{
  TItem* cur = mFirst;
  while( cur && (*cur < *item) )
    cur = cur->mNext;
  if( !cur ) append( item );
  else       insertBefore( cur, item );
}
//--------------------------------------------------------------------------
// ��������������� ������� �������� � ������ � ������������ � ������������ �������� ����������
template< class TItem >
void TDLL_ListT<TItem>::checkSortPos( TItem* item )
{
  TItem* pos = item->mPrev;
  if( pos && !(*pos < *item) )
  {
    do { pos = pos->mPrev; }
    while( pos && !(*pos < *item) );
    removeOne( item );
    if( pos ) insertAfter( pos, item );
    else      prepend( item );
  }
  else
  {
    pos = item->mNext;
    if( pos && (*pos < *item) )
    {
      do { pos = pos->mNext; }
      while( pos && (*pos < *item) );
      
      removeOne( item );
      if( pos ) insertBefore( pos, item );
      else      append( item );
    }

  }
}
//==============================================================================
//==============================================================================
//==============================================================================


#endif // List_DualLink_pH
