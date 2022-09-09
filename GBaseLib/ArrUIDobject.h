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


#ifndef ArrUIDobjectH
#define ArrUIDobjectH

#include "UIDobject.h"
#include "hArray.h"

//=============================================================================
//=============================================================================
//=============================================================================
DECLARE_CLASS( ArrUIDobject )

class GBASELIB_EI TArrUIDobject : public TArrayItemObject<TUIDobject>
{
  typedef TArrayItemObject<TUIDobject> inherited;

public:
  TArrUIDobject();
  TArrUIDobject( TCmpFunc Cmp, int s = 0 );
//  TArrUIDobject( RArrUIDobject arr );

  int DelByUID( guint32 uid );

  // Поиск в массиве объекта с заданным уникальным идентификатором
  // Если массив сортированн по TUIDobject::SortUID - по алгоритму быстрого поиска,
  // иначе поиск производится последовательным перебором
  int FindIndexUID( guint32 uid ) const;
  PUIDobject FindByUID( guint32 uid ) const
    {
      int index = FindIndexUID( uid );
      return (PUIDobject)operator[]( index );
    }

  bool  CheckUniqueUID( int *pInd1, int *pInd2 );

  PUIDobject operator [] ( int i ) const     // Возвратить элемент
    { return (PUIDobject)inherited::operator[]( i ); }

  // Пометить объекты массива как синхронизированные с БД
  void MarkAsSyncro();

  // Сопоставить объекты своего массива с объектами из <source>, и СКОПИРОВАТЬ
  // произведенные изменения в свой масива, выставив для объектов своего массива
  // соответствующее TStateRecord::tsXXX
  // Результат: изменение объектов своего массива были произведены, source не изменен
  bool CopyChanges( TArrUIDobject &source );

  // Сопоставить объекты своего массива с объектами из <source>, и ПЕРЕНЕСТИ
  // произведенные изменения в свой масива, выставив для объектов своего массива
  // соответствующее TStateRecord::tsXXX
  // Результат: изменение объектов своего массива были произведены, source - пуст
  bool MoveChanges( TArrUIDobject &source );

  void UnlinkChild( int type );
};
//=============================================================================
//=============================================================================
//=============================================================================
#endif
