#include "ArrUIDobject.h"
#include "BL_Debug.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TArrUIDobject::TArrUIDobject() : inherited()
{
}
//-----------------------------------------------------------------------------
TArrUIDobject::TArrUIDobject( TCmpFunc Cmp, int s ) :inherited( Cmp, s )
{
}
//-----------------------------------------------------------------------------
//TArrUIDobject::TArrUIDobject( RArrUIDobject arr ) : inherited( arr.CmpFunc )
//{
//}
//-----------------------------------------------------------------------------
int TArrUIDobject::DelByUID( guint32 uid )
{
  int index = FindIndexUID( uid );
  Delete( index );
  return index;
}
//-----------------------------------------------------------------------------
// Поиск в массиве объекта с заданным уникальным идентификатором
// Если массив сортированн по TUIDobject::SortUID - по алгоритму быстрого поиска,
// иначе поиск производится последовательным перебором
int TArrUIDobject::FindIndexUID( guint32 uid ) const
{
  if( IsSortBy( TUIDobject::SortUID ) )
    return FastSearch( (void const*)(size_t)uid, NULL, TUIDobject::FindUID );
  else                                                    
  {
    PUIDobject* ptr = (PUIDobject*)baseItem();
    int n = Count();
    while( n-- )
    {
      PUIDobject p = *ptr++;
      if( p && p->uid == uid )
        return Count() - n - 1;
    }
    return -1;
  }
}
//-----------------------------------------------------------------------------
// Проверка уникальности(отсутствие дублей) UID-полей объектов в массиве
// Необязательные выходные параметры:
// *pInd1, *pInd2 - индексы указателей-дублей
// Результат: уникальность элементов массива
bool TArrUIDobject::CheckUniqueUID( int *pInd1, int *pInd2 )
{
  int cnt = count;
  PUIDobject *pp = (PUIDobject*)baseItem();
  while( cnt-- )
  {
    guint32 uid = (*pp++)->uid;

    int cntCheck = cnt;
    PUIDobject *ppCheck = pp;
    while( cntCheck-- )
      if( (*ppCheck++)->uid == uid )
      {
        if( pInd1 ) *pInd1 = count - cnt - 1;
        if( pInd2 ) *pInd2 = count - cntCheck - 1;
        return false;
      }
  }
  return true;
}
//---------------------------------------------------------------------------
void TArrUIDobject::MarkAsSyncro()
{
  for( int i = 0; i < Count();  )
  {
    if( ((PUIDobject)Item(i))->IsDeleted() )
      Delete( i );
    else
    {
      ((PUIDobject)Item(i))->MarkAsSyncro();
      i++;
    }
  }
}
//---------------------------------------------------------------------------
bool TArrUIDobject::CopyChanges( TArrUIDobject &source )
{
  bool hasChange = false;
  for( int i = 0; i < source.Count(); i++ )
  {
    PUIDobject pSrc = source[i];
    switch( pSrc->TypeSynchro() )
    {
      case TStateRecord::tsNONE: break;   // ничего не надо
      case TStateRecord::tsINSERT: // INSERT
      {
        PObject pCopy = pSrc->dubl();
        pCopy->Assign( *pSrc );
        Add( pCopy );
        hasChange = true;
        break;
      }
      case TStateRecord::tsUPDATE: // UPDATE
      {
        PUIDobject pDest = FindByUID( pSrc->uid );
        if( pDest )
          pDest->Assign( *pSrc );
        else
          BL_FIX_BUG();
        hasChange = true;
        break;
      }
      case TStateRecord::tsDELETE:  // DELETE
      {
        PUIDobject pDest = FindByUID( pSrc->uid );
        pDest->MarkDeleted();
        hasChange = true;
        break;
      }
    }
  }
  for( int i = 0; i < Count(); i++ )
  {
    PUIDobject p = Get( i );
    if( !source.FindByUID( p->uid ) )
    {
      p->MarkDeleted();
      hasChange = true;
    }
  }
  return hasChange;
}
//---------------------------------------------------------------------------
bool TArrUIDobject::MoveChanges( TArrUIDobject &source )
{
  bool hasChange = false;
  for( int i = 0; i < source.Count(); i++ )
  {
    PUIDobject pSrc = source[i];
    switch( pSrc->TypeSynchro() )
    {
      case TStateRecord::tsNONE: break;   // ничего не надо
      case TStateRecord::tsINSERT: // INSERT
      {
        Add( pSrc );
        source.Set( i, 0 );
        hasChange = true;
        break;
      }
      case TStateRecord::tsUPDATE: // UPDATE
      {
        PUIDobject pDest = FindByUID( pSrc->uid );
        if( pDest )
          pDest->Assign( *pSrc );
        else
          BL_FIX_BUG();
        hasChange = true;
        break;
      }
      case TStateRecord::tsDELETE:  // DELETE
        BL_FIX_BUG();
    }
  }
  for( int i = 0; i < Count(); i++ )
  {
    PUIDobject p = (PUIDobject)Item(i);
    if( (p->TypeSynchro() == TStateRecord::tsNONE) && source.FindByUID( p->uid ) )
    {
      p->MarkDeleted();
      hasChange = true;
    }
  }
  return hasChange;
}
//---------------------------------------------------------------------------
void TArrUIDobject::UnlinkChild( int type )
{
  PUIDobject* ptr = (PUIDobject*)baseItem();
  int n = Count();
  while( n-- )
  {
    PUIDobject p = *ptr++;
    if( p )
      p->UnlinkChild( type );
  }
}
//---------------------------------------------------------------------------
