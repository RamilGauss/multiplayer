#ifndef List_SingleLinkH
#define List_SingleLinkH

#include "BL_IOStream.h"
//--------------------------------------------------------------------------
// Элемент односвязного списка
template< typename TItem >
class TSLL_ItemT
{
public:
  TItem* mNext;
  TSLL_ItemT(void) { mNext = 0; }

  // Последний элемент списка
  TItem* last() const;
};
//--------------------------------------------------------------------------
// Односвязный список, ориентированный на эффективное добавление элементов в начало списка
template< class TItem >
class TSLL_ListT
{
protected:
  TItem* mFirst;
public:

  TSLL_ListT(void) { mFirst = 0; }

  // При удалении объекта - очистить список
  //~TSLL_ListT()           { deleteAll(); }

  // Список - пуст
  bool empty() const       { return mFirst == 0; }
  
  // Количество элементов в списке
  int size() const;

  // Первый элемент списка
  TItem* first() const     { return mFirst; }
  
  // Последний элемент списка
  TItem* last() const      { return mFirst ? mFirst->last() : NULL; }

  // Удалить все элементы списка
  void deleteAll();

  // Добавить элемент в начало списка
  void prepend( TItem* item );

  // Добавить в начало списка элементы из другого списка
  void prepend( TSLL_ListT* list );
  
  // Исключить первый элемент из списка
  // Результат: исключенный элемент
  TItem* removeFirst();

  // Исключить из списка элемент <item>
  void removeOne( TItem* item );

  // Исключить все элементы из списка
  void removeAll();

  // Сохранить объект в поток
  bool save( TIOStream& od ) const;

  // Загрузить объект из потока
  bool load( TIOStream& od );
};
//--------------------------------------------------------------------------
// Односвязный список, ориентированный на эффективное добавление элементов 
// как в начало списка, так и в конец
template< class TItem >
class TSLL_ListET : public TSLL_ListT<TItem>
{
  typedef TSLL_ListT<TItem> inherited;
protected:
  TItem* mLast;
public:

  TSLL_ListET(void)   { mLast = 0; }

  // Последний элемент списка
  TItem* last()  const        { return mLast; }

  // Удалить все элементы списка
  void deleteAll();

  // Добавить элемент в начало списка
  void prepend( TItem* item );

  // Добавить элемент в конец списка
  void append( TItem* item );

  // Добавить список в конец списка
  void appendList( TItem* item );
  void appendList( TSLL_ListET<TItem>& list );

  // Исключить первый элемент из списка
  // Результат: исключенный элемент
  TItem* removeFirst();

  // Исключить из списка элемент <item>
  void removeOne( TItem* item );

  // Исключить все элементы из списка
  void removeAll();

  // Сохранить объект в поток
  //bool save( ROutData od ) const; -  идентичен родительскому методу

  // Загрузить объект из потока
  bool load( TIOStream& od );
};
//--------------------------------------------------------------------------

#include "List_SingleLink_p.h"

#endif
