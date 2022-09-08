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
// Количество элементов в списке
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
// Добавить элемент в начало списка
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
// Добавить элемент в конец списка
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
// Исключить первый элемент из списка
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
// Исключить последний элемент из списка
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
// Исключить все элементы списка
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
// Исключить все элементы из списка
template< class TItem >
inline void TDLL_ListT<TItem>::removeAll()
{
  mFirst = mLast = NULL;
}
//--------------------------------------------------------------------------
// Добавить элемент <item> в список после заданного элемента <pos>
template< class TItem >
inline void TDLL_ListT<TItem>::insertAfter( TItem* pos, TItem* item )
{
  if( pos->mNext )     // вставка в середину списка
  {
    item->mNext = pos->mNext;
    item->mPrev = pos;

    pos->mNext->mPrev = item;
    pos->mNext = item;
  }
  else                 // добавление в конец списка
  {
    item->mNext = NULL;
    item->mPrev = mLast;
    mLast->mNext = item;
    mLast = item;
  }
}
//--------------------------------------------------------------------------
// Добавить элемент <item> в список перед заданным элементом <pos>
template< class TItem >
inline void TDLL_ListT<TItem>::insertBefore( TItem* pos, TItem* item )
{
  if( pos->mPrev )     // вставка в середину списка
  {
    item->mPrev = pos->mPrev; 
    item->mNext = pos; 
    
    pos->mPrev->mNext = item;
    pos->mPrev = item;
  }
  else                // вставка в начало списка
  {
    item->mPrev = NULL;
    item->mNext = mFirst; 
    mFirst->mPrev = item;
    mFirst = item; 
  }
}
//--------------------------------------------------------------------------
// Добавить элемент в списка в соответствии с порядком сортировки
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
// Скорректировать позицию элемента в списке в соответствии с определенным порядком сортировки
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
