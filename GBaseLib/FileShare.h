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


#ifndef FileShareH
#define FileShareH

#include "FileError.h"
#include "GCS.h"
#include "BL_Debug.h"
#include "ArrUIDobject.h"


//=============================================================================
//=============================================================================
//=============================================================================
//-----------------------------------------------------------------------------
class TImageFile;
DECLARE_CLASS( FileShare )

class GBASELIB_EI TFileShare : protected GCS, public TFileError
{
  typedef GCS inherited;
  friend class TImageFile;

protected:
  TArrayObject  arrImage;   // список зарегистрированных образов файла
  TArrUIDobject arr;        // массив разделяемых данных

  //.......................................................
  //       Задание родительского файла и дочерних
  //.......................................................
protected:
  // Указание файла, чьи элементы являются родительскими по отношению к элементам данного файла
  // Термин "родитель" в данном случае подразумевает включение в себя в каком либо
  // виде ссылок на "дочерние" элементы.
  TFileShare*     mParentFile;
  // Свой номер в списке родительского файла
  int             numToParent;

  // Указание списка "дочерних" файлов
  PFileShare*     arrChildFiles;
  int             cntChildFiles;

  // Указание родительского файла и свой тип в нем
  void SetParent( PFileShare _mParentFile, int num )
    {
      mParentFile = _mParentFile;
      numToParent = num;
    }
public:
  // Указание списка "дочерних" файлов
  void SetChilds( PFileShare* childFiles, int cntChilds );
  //.......................................................
protected:
  //............................................................................
  // Средство борьбы со взаимоблокировками доступа.
  // При захвате данного файла захватываются также и все вспомогательные файлы ("субфайлы").
  // Важен порядок задания "субфайлов" - единая последовательность для всего приложения.
  //............................................................................
  struct TRecLock
  {
    PFileShare  pFile;
    bool        exclusive;// блокировать исключительно файл, без его зависимостей
  };
  enum { l_MaxCount = 10 };
  TRecLock  arrLock[l_MaxCount];  // список "субфайлов", на которые есть ссылки
  int cntLock;                    // и их кол-во

  const char* ownerLock;          // владелец блокировки
public:

  // Вставить вспомогательный файл в начало списка
  void PushFirstLock( PFileShare pFile, bool exclusive = false );
  // Удалить из списка вспомогательный файл
  void DelLock( PFileShare pFile );
  // Очистить список вспомогательный файлов
  void ClearLock();

  void Begin( const char* _ownerLock ); // Захват файла
  void End();   // Освобождение файла
  //............................................................................

public:
  TFileShare( const char* _name );

  virtual void OnAfterOpen();
  virtual void Close();
  virtual void Done();
  virtual ~TFileShare() { Done(); }

  // Назначить функцию сортировки массиву разделяемых данных
  void  Sort( TCmpFunc Cmp, bool doSort = true )  { arr.Sort( Cmp, doSort ); }

  // Зарегистрировать объект нового образа разделяемого набора данных
  bool  RegisterImage( TImageFile* pView );
  // Снять регистрацию
  void  UnregisterImage( TImageFile* pView );

  // Принудительно отключить все зарегистрированные образы
  // Предназначена для операций массированного добавления данных.
  // Функция восстановления списка зарегистрированных образов не предусмотрена!
  void  UnlinkAllImages()    { arrImage.Unlink(); }
  //.................................................................
  //         Операции над набором данных с сохранением в файл
  //.................................................................
  // Вставить объект в файл
  // index  - позиция вставки в массив образа, инициатора добавления или
  //          -1, если позицию вставки еще надо определять
  // p      - добавляемый объект
  // pImage - образ, инициатор добавления (или NULL)
  bool Insert( int index, PUIDobject p, TImageFile* pImage );

  // Удалить объект из файла
  // index  - позиция удаления в массиве образа, инициатора удаления (или -1)
  // p      - удаляемый объект
  // pImage - образ, инициатор удаления (или NULL)
  void Delete( PUIDobject p, int index, TImageFile* pImage );

  // Удалить объект из файла 
  // uid    - uid удаляемого объекта
  // pImage - образ, инициатор удаления (или NULL)
  void  DeleteUID( guint32 uid, TImageFile* pImage );

  // Удалить набор объектов по их идентификаторам
  // cntDel - кол-во объектов
  // arrDel - массив идентификаторов
  // pImage - образ, инициатор удаления (или NULL)
  virtual void DeleteArray( int cntDel, guint32* arrDel, TImageFile* pImage );

  // Уведомление от образа данных, что его владелец изменил поля объекта
  // p - измененный объект
  // index - индекс объекта в образе
  // pImage - образ, инициатор изменения (или NULL)
  bool CheckChange( PUIDobject p, int index, TImageFile* pImage );

protected:
  bool AppendFromLoad( PUIDobject p );
  //.................................................................
  //        Операции на набором данных без сохранения в файл
  //.................................................................
  bool WithoutFile_Add( PUIDobject p );
  void WithoutFile_DeleteUID( guint32 uid, bool notifyParent );
  void WithoutFile_Delete( int index, bool notifyParent );
  void WithoutFile_Change( PUIDobject p, int index, TImageFile* pImage, guint32 id_parent = UID_NULL );
  void WithoutFile_Change( guint32 uid, TImageFile* pImage = NULL );
  //.................................................................

  //.................................................................
  //     Операции с дочерними элементами объектов набора данных
  //.................................................................
  // Добавление дочернего объекта к элементу массива
  // uid - идентификатор элемента массива (родителя)
  // pChild - дочерний элемент
  // type - тип дочернего элемента
  // Результат: родитель найден и дочка добавлена
  bool AddChild        ( guint32 uid, PUIDobject pChild, int type );
  bool AddChildFromLoad( guint32 uid, PUIDobject pChild, int type );

  // Изменение дочернего объекта элемента массива
  // uid - идентификатор элемента массива (родителя)
  // pChild - дочерний элемент
  // type - тип дочернего элемента
  virtual void ChangeChild( guint32 uid, PUIDobject pChild, int type );

  // Удаление дочернего объекта из элемента массива
  // uid - идентификатор элемента массива (родителя)
  // uidChild - дочерний элемент
  // type - тип дочернего элемента
  void DelChild( guint32 uid, guint32 uidChild, int type );
  //.................................................................

  // Сохранить объект в файле и настроить его uid
  virtual bool SaveObject( void* p ) = 0;
  // Удалить объект из файла
  virtual bool DeleteObject( void* p ) = 0;

public:
  // Набор данных для обработчика события удаления массива объектов из файла
  struct TRecDelArray {
    PUIDobject p;       // удаляемый объект
    int indexBase;      // индекс, из которого было произведено удаление объекта в базовом наборе
    // Функция сортировки по возрастанию значения поля indexBase
    static int SortIndexBaseUp( const void* p1, const void* p2 );
    // Функция сортировки по возрастанию значения поля p->uid
    static int SortPUID( const void* p1, const void* p2 );
    // Функция поиска значения поля p->uid
    static int FindPUID( const void* p1, const void* p2 );
  };
  typedef TArrayVAR<TRecDelArray> TArrayRDA;

  // Удалить набор объектов из файла
  // arrDel отсортирован по возрастанию значения поля indexBase
  virtual void DeleteObjects( TArrayRDA& arrDel );

  // Применить к БД изменения, произведенные на всем массив
  //virtual bool Apply( PArrUIDobject pArr, PImageFile pImage ) = 0;

  //.................................................................
  //....           Для служебного использования    ..................
  //....        Вызывать внутри критических секций   ................
  //.................................................................
public:
  // Кол-во объектов в файле
  // Обрамление вызова функциями begin/end:
  int Count() const;  // необходимо
  int CountBE();      // не требуется

  // Массив разделяемых данных.
  const TArrUIDobject& getArray() const { return arr; }

  // Поиск в массиве объекта с заданным уникальным
  int FindIndexUID( guint32 uid ) const;
  PUIDobject FindUID( guint32 uid ) const;  // Поиск элемента

  // Быстрый поиск в сортированном массиве
  int FastSearch( const void* p, int * ins, TCmpFunc Cmp ) const;
  // Последовательный поиск в массиве
  int StepSearch( const void* p, TCmpFunc Cmp ) const;

  // Получить элемент массива
  PUIDobject Item( int index ) { return arr[index]; }
};
//=============================================================================
//=============================================================================
//=============================================================================
// Поиск в массиве объекта с заданным уникальным идентификатором
inline int TFileShare::FindIndexUID( guint32 uid ) const
{
  return arr.FindIndexUID( uid );
}
//-----------------------------------------------------------------------------
inline PUIDobject TFileShare::FindUID( guint32 uid ) const
{
  return (PUIDobject)arr[ FindIndexUID( uid ) ];
}
//-----------------------------------------------------------------------------
// Быстрый поиск в сортированном массиве
inline int TFileShare::FastSearch( const void* p, int* ins, TCmpFunc Cmp ) const
{
  return arr.FastSearch( p, ins, Cmp );
}
//-----------------------------------------------------------------------------
// Последовательный поиск в массиве
inline int TFileShare::StepSearch( const void* p, TCmpFunc Cmp ) const
{
  return arr.StepSearch( p, Cmp );
}
//-----------------------------------------------------------------------------
// Кол-во объектов в файле
inline int TFileShare::Count() const
{
  return arr.Count();
}
//-----------------------------------------------------------------------------
// Кол-во объектов в файле
inline int TFileShare::CountBE()
{
  Begin( dbgName() );
  int cnt = arr.Count();
  End();
  return cnt;
}
//-----------------------------------------------------------------------------
// Удалить объект из файла 
inline void TFileShare::DeleteUID( guint32 uid, TImageFile* pImage )
{
  int index = FindIndexUID( uid );
  PUIDobject p = Item( index );
  if( p )
    Delete( p, index, pImage );
}
//-----------------------------------------------------------------------------
#endif
