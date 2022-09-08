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
