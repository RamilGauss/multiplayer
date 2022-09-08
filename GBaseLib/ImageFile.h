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
the "Tanks" Source Code.  If not, please request a copy in writing from id Software at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef ImageFileH
#define ImageFileH

#include "BL_Debug.h"
#include "ArrUIDobject.h"
#include "IImageNotify.h"
#include "FileShare.h"
#include "glib/gatomic.h"

//=============================================================================
//=============================================================================
//=============================================================================

DECLARE_CLASS( FileShare )
DECLARE_CLASS( FileDB )
DECLARE_CLASS( ImageFile )

// набор данных для просмотра/редактирования
class GBASELIB_EI TImageFile : public TObject
{
#ifdef _DEBUG
  int cntBlock;
#endif
  // уведомлять себя об произведенных собою-же операциях (insert, delete, ... )
  bool selfNotifyChange;

  volatile bool mHasChange;        // наличие изменений в наборе данных
public:

#ifdef _DEBUG
  bool IsBlock() { return cntBlock > 0; }
#endif
  enum TDependence
  {
    depend_full,  // полная зависимость набора от базового
    independ_arr, // независимость набора от базового, но на базовых объектах
    independ_subArr,// независимость набора от базового, подмножество базовых объектов
    //independ_obj, // независимость и набора и объектов набора от базовых
    depend_count
  };
protected:
  TDependence   depend;     // Степень зависимости образа данных от базового набора
  IImageNotify  *mNotify;   // Объект рассылки уведомлений об изменении состояния

  PArrUIDobject pArray;     // указатель на набор данных
  int          indexRes;    // Индекс последней добавленной/удаленной/измененной записи
  int          indexOldChange; // Индекс последней измененной записи до пересортировки

  TCmpFunc      filterFunc; // Функция фильтрации исходного массива
  void*         filterParam;// Параметр фильтрации

  TCmpFunc      CmpFunc;    // Функция сортировки массива (дубликат pArray->CmpFunc)
  char          name[255];  // условное имя (для отладки)

  // Набор данных для обработчика события добавления/вставки объекта в файл
  struct TFileInsert {
    PUIDobject p;       // добавляемый объект
    PImageFile pImage;  // образ - инициатор операции
    int indexBase;      // индекс, в который производиться вставка объекта в базовом наборе
    int indexImage;     // индекс, в который производиться вставка объекта в образе - инициаторе
  };

  // Набор данных для обработчика события удаления объекта из файла
  struct TFileDel {
    PUIDobject p;       // удаляемый объект
    PImageFile pImage;  // образ - инициатор операции
    int indexImage;     // индекс, из которого производиться удаление объекта в образе - инициаторе
    int indexBase;      // индекс, из которого было произведено удаление объекта в базовом наборе
  };
protected:
  struct TFileDelArray {
    TFileShare::TArrayRDA  arrRec;
    PImageFile pImage;  // образ - инициатор операции
    TFileDelArray( int size ): arrRec( NULL, size ) {pImage=NULL;}
  };

  // Набор данных для обработчика события изменения объекта из файла
  struct TFileChange {
    PUIDobject p;       // указатель на измененный объект
    PImageFile pImage;  // образ - инициатор операции
    int   indexImage;   // индекс объекта в образе - инициаторе
    int   indexBaseOld; // старый индекс объекта в базовом наборе
    int   indexBaseNew; // новый индекс объекта в базовом наборе
  };

  // Функция-обработчик события открытия файла
  static void fileOpen( PImageFile* ppImage, PArrUIDobject parr );
  // Функция-обработчик события закрытия файла
  static void fileClose( PImageFile* ppImage, void* );
  // Функция-обработчик события добавления/вставки объекта в файла
  static void fileInsert( PImageFile* ppImage, TFileInsert* def );
  // Функция-обработчик события удаления объекта из файла
  static void fileDelete( PImageFile* ppImage, TFileDel* def );
  // Функция-обработчик события удаления массива объектов из файла
  static void fileDeleteArray( PImageFile* ppImage, TFileDelArray* def );
  // Функция-обработчик события изменения объекта из файла
  static void fileChange( PImageFile* ppImage, TFileChange* def );

  // Оповещение о событии добавления объекта
  virtual void doNotifyInsert( TImageFile* imageSrc, int index, PUIDobject p );
  // Оповещение о событии удаления объекта
  virtual void doNotifyDelete( TImageFile* imageSrc, int index, PUIDobject p );
  // Оповещение о событии удаления объекта
  virtual void doNotifyDeleteArray( TImageFile* imageSrc );
  // Оповещение о событии изменения объекта
  virtual void doNotifyChange( TImageFile* imageSrc, int oldIndex, int index, PUIDobject p );

  PFileShare pFile;     // указатель на базовый объект файла
  friend class TFileShare;

  bool Init( PArrUIDobject parr, PFileShare pfile );
  void Done();

public:

  ///////////////////////////////////////////////////////////////
  // Функции, общие для всех режимов зависимости
  //

  TImageFile();
  virtual ~TImageFile();

  // Образ зарегистрирован
  bool IsRegistered();
  void Unregister();

  // Настройка режима доступа и оповещения
  void Config( const char* name, IImageNotify *notify, TDependence _depend, TCmpFunc cmpFunc = NULL );

  // Настройка режима фильтрации
  // filterFunc - функция фильтрации. Возвращает 0 в случае выполнения условий фильтра
  // filterParam - пользовательский параметр фильтра
  void ConfigFilter( TCmpFunc filterFunc, void* filterParam );

  // Смена параметра фильтрации
  void ChangeFilterParam( void* filterParam );

  // Проверить флаг наличия изменений в наборе данных (begin/end необязательны)
  // reset - сбросить флаг после проверки 
  bool IsChanged( bool reset = true );

  // Сбросить флаг наличия изменений 
  void ResetChanged();

  // уведомлять себя об произведенных собою-же операциях (insert, delete, ... )
  void SelfNotifyChange( bool flag );

  // Быстрый поиск в сортированном массиве
  // Описание см. в hArray.cpp
  int FastSearch( const void * p, int * ins = NULL, TCmpFunc Cmp = NULL );

  // Поиск в массиве объекта с заданным уникальным
  int FindIndexUID( guint32 uid );
  PUIDobject FindUID( guint32 uid );  // Поиск элемента

  // Поиск в массиве заданного объекта ( последовательным перебором ! )
  int FindPtr( void* p );
  int FindPtr( void* p, int start, bool revers = false );

  void  Sort( TCmpFunc Cmp, bool doSort = true );
  
  // Проверка сортированности массива по указанной Функции
  bool  IsSortBy( TCmpFunc cmpFunc ) const { return CmpFunc == cmpFunc; }

  // Получить копию объекта
  // uid - идентификатор объекта
  // r   - буферный объект
  bool GetCopy( guint32 uid, TUIDobject& r );
  
  // Заменить значение объекта
  // r - новое состояние объекта с прежним uid
  bool Replace( TUIDobject& r );
  
  // Количество элементов
  // Обрамление вызова функциями begin/end:
  int Count();     // необходимо
  int CountBE();   // не требуется

  // Доступ к объекту с проверкой на попадание в диапазон
  PUIDobject Item( int index );
  PUIDobject operator[]( int index ) { return Item( index ); }

  // Получение указателя для прямого доступа к элементам массива
  // Быстрый, но не защищенный доступ.
  // Указатель недостоверен после операций добавления/удаления объектов.
  PUIDobject* baseItem();

  // Подсчет кол-ва сбоев сортировки (невыполнения условия a[i-1] <= a[i] <= a[i+1]
  int CountErrSort( int *pError );

  ///////////////////////////////////////////////////////////////
  // -------------Функции, для режимов <depend_full> и <independ_arr>
  //

  // Средства синхронизации доступа к данным.
  // Обязательны для работы со всеми ниже приведенными функциями
  void begin();
  void end();

  // Функции для поддержания корректного состояния отладочного счетчика
  // <cntBlock> в случаях неявного захвата родительских файлов через захват дочерних
  // задаваемого функцией TFileShare::SetOwner( )
  #ifdef _DEBUG
  void FALSEbegin() { g_atomic_int_inc( &cntBlock ); }
  void FALSEend()   { BL_ASSERT( g_atomic_int_exchange_and_add( &cntBlock, -1) > 0 ); }
  #else
  void FALSEbegin() {}
  void FALSEend()   {}
  #endif

  // Добавить объект
  // p - добавляемый объект
  // Результат: позиция вставки элемента в образ
  int Add( PUIDobject p );

  // Вставить объект
  // p - добавляемый объект
  // index - позиция вставки
  // Результат: успех
  bool  Insert( int index, PUIDobject p );

  // Удалить объект
  // index - индекс объекта в образе
  // p - указатель на объект
  // uid - идентификатор объекта
  void  Delete( int index );   // самый быстрый
  void  Delete( PUIDobject p );
  void  DeleteUID( guint32 uid );  // самый медленный, если образ не отсортирован по uid

  // Удалить набор объектов по их идентификаторам
  void  Delete( int cntUID, guint32* arrUID );

  // Уведомить образ об изменении объекта
  // Проверить порядок сортировки после проведенной коррекции объекта
  // Результат: новая позиция скорректированного объекта (может остаться и прежней).
  // index - индекс объекта в образе
  // p - указатель на объект
  int CheckChange( int index );    // самый быстрый
  int CheckChange( PUIDobject p );

  // Уведомить образ об изменении объекта, не сохраняя объект
  void  WithoutFile_Change( PUIDobject p, int index );

  ///////////////////////////////////////////////////////////////

  #ifdef _DEBUG
  // Проверка корректности вызова методов класса требующих блокировки
  bool isCorrectLock()
    { return pArray && (cntBlock > 0); }
  #endif

  // Проверка уникальности(отсутствие дублей) UID-полей объектов в массиве
  // Необязательные выходные параметры:
  // *pInd1, *pInd2 - индексы указателей-дублей
  // Результат: уникальность элементов массива
  bool CheckUniqueUID( int *pInd1, int *pInd2 );
};

//=============================================================================
//=============================================================================
//=============================================================================

inline void TImageFile::begin()
{
  BL_ASSERT( cntBlock >= 0 );
  if( pFile ) pFile->Begin( name );
  BL_DEBUG( g_atomic_int_inc( &cntBlock ); )
}
//-----------------------------------------------------------------------------
inline void TImageFile::end()
{
  BL_ASSERT( g_atomic_int_exchange_and_add( &cntBlock, -1) > 0 );
  if( pFile ) pFile->End();
}
//-----------------------------------------------------------------------------
// Быстрый поиск в сортированном массиве
inline int TImageFile::FastSearch( const void * p, int * ins, TCmpFunc Cmp )
{
  BL_ASSERT( isCorrectLock() );
  return pArray ? pArray->FastSearch( p, ins, Cmp ) : -1;
}
//-----------------------------------------------------------------------------
// Поиск в массиве объекта с заданным уникальным идентификатором
inline int TImageFile::FindIndexUID( guint32 uid )
{
  return pArray ? pArray->FindIndexUID( uid ) : -1;
}
//-----------------------------------------------------------------------------
// Поиск элемента
inline PUIDobject TImageFile::FindUID( guint32 uid )
{
  BL_ASSERT( isCorrectLock() );
  int index = FindIndexUID( uid );
  return Item( index );
}
//-----------------------------------------------------------------------------
inline int TImageFile::FindPtr( void* p )
{
  BL_ASSERT( isCorrectLock() );
  if( !pArray )
    return -1;
  return pArray->FindValue( (PObject)p );
}
//-----------------------------------------------------------------------------
inline int TImageFile::FindPtr( void* p, int start, bool revers )
{
  BL_ASSERT( isCorrectLock() );
  return pArray->FindValue( (PObject)p, start, revers );
}
//-----------------------------------------------------------------------------
inline PUIDobject TImageFile::Item( int index )
{
  BL_ASSERT( isCorrectLock() );
  return pArray ? (PUIDobject)(*pArray)[ index ] : NULL;
}
//-----------------------------------------------------------------------------
inline PUIDobject* TImageFile::baseItem()
{
  BL_ASSERT( isCorrectLock() );
  return pArray ? (PUIDobject*)pArray->baseItem() : NULL;
}
//-----------------------------------------------------------------------------
// Количество элементов
// Обрамление вызова функциями begin/end не требуется
inline int TImageFile::CountBE()
{
  begin();
  int cnt = pArray->Count();
  end();
  return cnt;
}
//-----------------------------------------------------------------------------
// Количество элементов
// Обрамление вызова функциями begin/end необходимо
inline int TImageFile::Count()
{
  BL_ASSERT( isCorrectLock() );
  return pArray->Count();
}
//-----------------------------------------------------------------------------
inline bool TImageFile::IsRegistered()
{
  return pArray != NULL;
}
//-----------------------------------------------------------------------------
// Проверить флаг наличия изменений в наборе данных
inline bool TImageFile::IsChanged( bool reset )
{
  bool res = mHasChange;
  if( reset ) mHasChange = false;
  return res;
}
//-----------------------------------------------------------------------------
// Сбросить флаг наличия изменений
inline void TImageFile::ResetChanged()
{
  mHasChange = false;
}
//-----------------------------------------------------------------------------
// Проверка уникальности(отсутствие дублей) UID-полей объектов в массиве
inline bool TImageFile::CheckUniqueUID( int *pInd1, int *pInd2 )
{
  BL_ASSERT( isCorrectLock() );
  return pArray->CheckUniqueUID( pInd1, pInd2 );
}
//-----------------------------------------------------------------------------
#endif
