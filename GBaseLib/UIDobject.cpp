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
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 


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
