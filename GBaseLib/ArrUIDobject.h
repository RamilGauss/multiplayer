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
