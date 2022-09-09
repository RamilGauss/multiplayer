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


#ifndef List_DualLinkH
#define List_DualLinkH

//--------------------------------------------------------------------------
// Элемент двусвязного списка
template< typename TItem >
class TDLL_ItemT
{
public:
  TItem* mPrev;
  TItem* mNext;
  TDLL_ItemT(void) { mPrev = mNext = 0; }
  
  TItem* first() const;   // Последний элемент списка
  TItem* last() const;    // Последний элемент списка

};
//--------------------------------------------------------------------------
// Двусвязный  список, ориентированный на эффективное добавление/удаление элементов 
// в произвольной позиции списка
template< class TItem >
class TDLL_ListT
{
  TItem* mFirst;
  TItem* mLast;
public:

  TDLL_ListT(void) { mFirst = mLast = 0; }

  // При удалении объекта - очистить список
  //~TSLL_ListT()           { deleteAll(); }

  // Количество элементов в списке
  int size() const;

  // Список - пуст
  bool empty() const       { return mFirst == 0; }

  // Последний элемент списка
  TItem* first() const     { return mFirst; }

  // Последний элемент списка
  TItem* last() const      { return mLast; }

  // Удалить все элементы списка
  void deleteAll();

  // Добавить элемент в начало списка
  void prepend( TItem* item );

  // Добавить элемент в конец списка
  void append( TItem* item );

  // Исключить первый элемент из списка
  // Результат: исключенный элемент
  TItem* removeFirst();

  // Исключить последний элемент из списка
  // Результат: исключенный элемент
  TItem* removeLast();

  // Исключить из списка элемент <item>
  void removeOne( TItem* item );

  // Исключить все элементы из списка
  void removeAll();

  // Добавить элемент <item> в список после заданного элемента <pos>
  void insertAfter( TItem* pos, TItem* item );

  // Добавить элемент <item> в список перед заданным элементом <pos>
  void insertBefore( TItem* pos, TItem* item );

  // Добавить элемент в списка в соответствии с порядком сортировки
  void insert( TItem* item );

  // Скорректировать позицию элемента в списке в соответствии с порядком сортировки
  void checkSortPos( TItem* item );
};
//--------------------------------------------------------------------------

#include "List_DualLink_p.h"

#endif // List_DualLinkH
