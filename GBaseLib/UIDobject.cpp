#include "UIDobject.h"

//-----------------------------------------------------------------------------
// Проверка - синхронизированна ли запись с БД
bool TStateRecord::IsSyncro()
{
  if( state & inactive )
    { if( state & database ) return false; }
  else
    { if( !(state & database) || (state & changed) ) return false; }
  return true;
}
//-----------------------------------------------------------------------------
// Тип необходимой операции для синхронизацией с БД
int TStateRecord::TypeSynchro()
{
  if( state & inactive )    // неактивна (удалена из набора)
  {
    if( state & database )  // существует в БД
      return tsDELETE;      // удалить из БД
    else
      return tsNONE;        // ничего не надо
  }
  else                      // активна (в наборе)
  {
    if( state & database )  // существует в БД
    {
      if( state & changed ) // изменена
        return tsUPDATE;    // изменить в БД
      else
        return tsNONE;      // ничего не надо
    }
    else return tsINSERT;   // добавить в БД
  }
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//#define cntCallSort_ENABLE
/*static*/ int TUIDobject::cntCallSort;

/*static*/int TUIDobject::SortUID( const void* po1, const void* po2 )
{
#ifdef cntCallSort_ENABLE
  cntCallSort++;
#endif
  guint32 uid1 = (*(PUIDobject*)po1)->uid;
  guint32 uid2 = (*(PUIDobject*)po2)->uid;
  if( uid1 < uid2 ) return -1;
  if( uid1 > uid2 ) return 1;
  return 0;
}
//-----------------------------------------------------------------------------
/*static*/int TUIDobject::FindUID( const void* po, const void* uid )
{
  guint32 UID = (*(PUIDobject*)po)->uid;
  if( UID < (guint32)(size_t)uid ) return -1;
  if( UID > (guint32)(size_t)uid ) return 1;
  return 0;
}
//-----------------------------------------------------------------------------
bool TUIDobject::save( TIOStream& od ) const
{
  return od.put( &uid, sizeof( uid ) ) &&
         od.put( &state, sizeof( state ) );
}
//-----------------------------------------------------------------------------
bool TUIDobject::load( TIOStream& od )
{
  return od.get( &uid, sizeof( uid ) ) &&
         od.get( &state, sizeof( state ) );
}
//-----------------------------------------------------------------------------
int TUIDobject::length() const
{
  return sizeof( uid ) + sizeof( state );
}
//-----------------------------------------------------------------------------
/*bool TUIDobject::Assign( RObject o )
{
  if( inherited::Assign( o ) )
  {
    uid = ((PUIDobject)&o)->uid;
    state = ((PUIDobject)&o)->state;
    return true;
  }
  return false;
}*/
//-----------------------------------------------------------------------------
