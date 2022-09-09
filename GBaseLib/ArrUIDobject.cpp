/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
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
