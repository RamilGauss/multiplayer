/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
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
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef List_SingleLink_pH
#define List_SingleLink_pH


//--------------------------------------------------------------------------
//---------------               TSLL_ItemT          ------------------------
//--------------------------------------------------------------------------
template< class TItem >
TItem* TSLL_ItemT<TItem>::last() const
{
  TItem* p = this;
  TItem* pn;
  while( (pn = p->mNext) != NULL ) p = pn;
  return p;
}
//--------------------------------------------------------------------------
//---------------               TSLL_ListT          ------------------------
//--------------------------------------------------------------------------
// Количество элементов в списке
template< class TItem >
int TSLL_ListT<TItem>::size() const
{
  int n = 0;
  TItem *p = mFirst;
  while( p ) { p = p->mNext; n++; }
  return n;
}
//--------------------------------------------------------------------------
template< class TItem >
void TSLL_ListT<TItem>::deleteAll()
{
  if( !mFirst ) return;
  
  TItem* p = mFirst;
  while( p )
  {
    TItem* pn = p->mNext;
    delete p;
    p = pn;
  }
  mFirst = NULL;
}
//--------------------------------------------------------------------------
// Добавить элемент в начало списка
template< class TItem >
inline void TSLL_ListT<TItem>::prepend( TItem* item )   
{ 
  item->mNext = mFirst; 
  mFirst = item; 
}
//--------------------------------------------------------------------------
// Добавить в начало списка элементы из другого списка
template< class TItem >
inline void TSLL_ListT<TItem>::prepend( TSLL_ListT* list )
{
  TItem* item;
  while( (item = list->removeFirst()) )
    prepend( item );
}
//--------------------------------------------------------------------------
// Исключить первый элемент из списка
template< class TItem >
inline TItem* TSLL_ListT<TItem>::removeFirst()
{
  if( mFirst ) 
  { 
    TItem* p = mFirst;
    mFirst = mFirst->mNext;
    return p;
  }
  return NULL;
}
//--------------------------------------------------------------------------
// Исключить все элементы списка
template< class TItem >
void TSLL_ListT<TItem>::removeOne( TItem* item )
{
  if( !mFirst || !item ) return;

  if( mFirst == item )
  {
    if( mFirst->mNext == 0 )
      mFirst = NULL;
    else
      mFirst = mFirst->mNext;
  }
  else
  {
    TItem* cur = mFirst;
    TItem* newCur = mFirst->mNext;
    while( newCur )
    {
      if( newCur == item )
      {
        cur->mNext = newCur->mNext;
        return;
      }
      else
      {
        cur = newCur;
        newCur = newCur->mNext;
      }
    }
  }
}
//--------------------------------------------------------------------------
// Исключить все элементы из списка
template< class TItem >
inline void TSLL_ListT<TItem>::removeAll()
{
  mFirst = NULL;
}
//--------------------------------------------------------------------------
// Сохранить объект в поток
template< class TItem >
bool TSLL_ListT<TItem>::save( TIOStream& od ) const
{
  int sz = size();
  if( !od.put( &sz, sizeof(sz) ) ) return false;

  TItem* p = mFirst;
  while( p )
  {
    if( !p->save( od ) ) return false;
    p = p->mNext;
  }
  return true;
}
//--------------------------------------------------------------------------
// Загрузить объект из потока
template< class TItem >
bool TSLL_ListT<TItem>::load( TIOStream& od )
{
  int sz;
  if( !od.get( &sz, sizeof(sz) ) ) return false;
  
  TItem* pLast = NULL;
  for( int i = 0; i < sz; i++ )
  {
    TItem* p = new TItem;
    if( !p->load( od ) ) return false;
    if( !pLast )
      mFirst = pLast = p;
    else 
    {
      pLast->mNext = p;
      pLast = p;
    }
  }
  return true;
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//---------------               TSLL_ListET         ------------------------
//--------------------------------------------------------------------------
template< class TItem >
void TSLL_ListET<TItem>::deleteAll()
{
  TSLL_ListT<TItem>::deleteAll();
  mLast = NULL;
}
//--------------------------------------------------------------------------
// Добавить элемент в начало списка
template< class TItem >
inline void TSLL_ListET<TItem>::prepend( TItem* item )   
{ 
  if( (item->mNext = inherited::mFirst) == 0 )
    mLast = item; 
  inherited::mFirst = item; 
}
//--------------------------------------------------------------------------
// Добавить элемент в конец списка
template< class TItem >
inline void TSLL_ListET<TItem>::append( TItem* item )   
{ 
  item->mNext = NULL; // ???
  if( !inherited::mFirst ) inherited::mFirst = mLast = item; 
  else          
  {
    mLast->mNext = item;
    mLast = item;
  }
}
//--------------------------------------------------------------------------
// Добавить список в конец списка
template< class TItem >
inline void TSLL_ListET<TItem>::appendList( TItem* item )   
{
  if( !inherited::mFirst ) inherited::mFirst = item;
  mLast = item->last();  
}
//--------------------------------------------------------------------------
// Добавить список в конец списка
template< class TItem >
inline void TSLL_ListET<TItem>::appendList( TSLL_ListET<TItem>& list )
{
  if( !inherited::mFirst )
  {
    inherited::mFirst = list.inherited::mFirst;
    mLast = list.mLast;
  }
  else
    if( list.inherited::mFirst )
    {
      mLast->mNext = list.inherited::mFirst;  
      mLast = list.mLast;  
    }
}
//--------------------------------------------------------------------------
// Исключить первый элемент из списка
template< class TItem >
inline TItem* TSLL_ListET<TItem>::removeFirst()
{
  if( inherited::mFirst ) 
  { 
    TItem* p = inherited::mFirst;
    inherited::mFirst = inherited::mFirst->mNext;
    if( !inherited::mFirst ) 
      mLast = 0;
    return p;
  }
  return NULL;
}
//--------------------------------------------------------------------------
// Исключить все элементы списка
template< class TItem >
void TSLL_ListET<TItem>::removeOne( TItem* item )
{
  if( !inherited::mFirst || !item ) return;

  if( inherited::mFirst == item )
  {
    if( inherited::mFirst->mNext == 0 )
      inherited::mFirst = NULL;
    else
      inherited::mFirst = inherited::mFirst->mNext;
  }
  else
  {
    TItem* cur = inherited::mFirst;
    TItem* newCur = inherited::mFirst->mNext;
    while( newCur )
    {
      if( newCur == item )
      {
        if( (cur->mNext = newCur->mNext) == NULL )
          mLast = cur;
        return;
      }
      else
      {
        cur = newCur;
        newCur = newCur->mNext;
      }
    }
  }
}
//--------------------------------------------------------------------------
// Исключить все элементы из списка
template< class TItem >
inline void TSLL_ListET<TItem>::removeAll()
{
  inherited::mFirst = mLast = NULL;
}
//--------------------------------------------------------------------------
// Загрузить объект из потока
template< class TItem >
bool TSLL_ListET<TItem>::load( TIOStream& od )
{
  int sz;
  if( !od.get( &sz, sizeof(sz) ) ) return false;

  mLast = NULL;
  for( int i = 0; i < sz; i++ )
  {
    TItem* p = new TItem;
    if( !p->load( od ) ) return false;
    if( !inherited::pLast )
      inherited::mFirst = inherited::mLast = p;
    else 
    {
      mLast->mNext = p;
      mLast = p;
    }
  }
  return true;
}
//==============================================================================
//==============================================================================
//==============================================================================


#endif
