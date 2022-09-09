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


#ifndef UIDobjectH
#define UIDobjectH

#include "TObject.h"

//---------------------------------------------------------------------------
class GBASELIB_EI TStateRecord
{
  // Биты признаков состояния записи
  enum {
    inactive = (1<<7),  // неактивна (удалена из набора)
    database = (1<<6),  // запись существует в БД
    changed  = (1<<5),  // изменена
    value    = 0x1F     // маска значения задаваемого извне
  };
  guint8 state;

public:
  TStateRecord()
  {
    state = 0; // активная, в БД не существует, не изменена
  }

  enum {
    tsNONE,   // ничего не надо
    tsINSERT, // INSERT
    tsUPDATE, // UPDATE
    tsDELETE  // DELETE
  };
  // Тип необходимой операции для синхронизацией с БД
  // Результат: см. tsXXX
  int TypeSynchro();

  // Пометить как загруженную из БД
  void SetLoaded()  { state = database; }

  // Пометить запись как синхронизированную с БД
  void MarkAsSyncro()
  {
    // если неактивна - значит не существует в БД
    if( state & inactive ) state &= ~database;
    // иначе сохранена в базе
    else state = (guint8)( (state | database) & ~changed );
  }

  // Проверка - синхронизированна ли запись с БД
  bool IsSyncro();

  // Пометить запись как сохраненную в БД
  void MarkSaved()    { state |= database; }

  //
  // Функции управления состоянием объекта
  //
  void MarkDeleted()  { state |= inactive; }
  void UnmarkDeleted(){ state &= ~inactive; }
  bool IsDeleted()    { return (state & inactive) != 0; }

  void MarkChanged()  { state |= changed; }
  bool IsChanged()    { return (state & changed) != 0; }

  void SetAsNew()     { state = 0; }
  bool InDatabase()   { return (state & database) != 0; }

  void SetAsSynchro( guint8 val = 0 ) { state = (guint8)( database | val ); }
};

//---------------------------------------------------------------------------

DECLARE_CLASS( UIDobject )
#define UID_NULL    ((guint32)-1)  // недействительное значение
#define UID_ANY     ((guint32)-2)  // любое значения

class GBASELIB_EI TUIDobject : public TObject
{
  typedef TObject inherited;
protected:
  TStateRecord  state;
public:
  guint32 uid;        // идентификатор объекта, уникален в файле во время всего существования файла

  TUIDobject()  { uid = UID_NULL; }

  //
  // Функции управления состоянием объекта
  //
  void MarkDeleted()  { state.MarkDeleted(); }
  bool IsDeleted()    { return state.IsDeleted(); }

  void MarkChanged()  { state.MarkChanged(); }
  bool IsChanged()    { return state.IsChanged(); }

  void SetAsNew()     { state.SetAsNew(); }
  bool InDatabase()   { return state.InDatabase(); }

  // Тип необходимой операции для синхронизацией с БД
  // Результат: см. TStateRecord::tsXXX
  int TypeSynchro()   { return state.TypeSynchro(); }

  // Пометить запись как синхронизированную с БД
  virtual void MarkAsSyncro()    { state.MarkAsSyncro(); }
  void SetAsSynchro() { state.SetAsSynchro(); }
  void MarkSaved()    { state.MarkSaved(); }
  // Проверка - синхронизированна ли запись с БД
  bool IsSyncro() { return state.IsSyncro(); }

  //.........................................................
  //   Операции с объектов дочерних наборов данных
  //.........................................................
  // Получить идентификатор родителя
  virtual guint32 getUIDParent() const  { return UID_NULL; }
  virtual void setUIDParent( guint32 )  { }

  // Добавление дочерного элемента
  // PUIDobject - указатель на дочерний объект
  // int - номер типа дочки
  virtual void AddChild( PUIDobject, int ) {}

  // Удаление дочерного элемента
  // guint32 - идентификатор дочернего объекта
  // int - номер типа дочки
  virtual void DelChild( guint32, int ) {}

  // Кол-во дочерних элементов
  // int - номер типа дочки
  virtual int CountChild( int ) { return 0; }

  // Получить идентификатор дочернего элемента
  // index - порядковый номер в списке
  // int - номер типа дочки
  virtual guint32 GetUIDChild( int /*index*/, int ) { return UID_NULL; }

  // Исключить все дочерние элементы без удаления
  // int - номер типа дочки (-1 - дочерние элементы всех типов)
  virtual void UnlinkChild( int ) {  }
  //.........................................................
  //.........................................................

  //
  // Обязательное наследство от Object
  //
  virtual bool     save( TIOStream& od ) const;
  virtual bool     load( TIOStream& od );
  virtual int      length() const;
  virtual bool     Assign( const TObject& obj )
  {
    PUIDobject p = (PUIDobject)&obj;
    uid = p->uid;
    state = p->state;
    return true;
  }
  //
  // Поддержка uid
  //
  virtual void  ResetUID()    = 0;
  virtual void  CorrectUID()  = 0;
  virtual void  GenerateUID() = 0;

  static int SortUID( const void* po1, const void* po2 );
  static int FindUID( const void* po, const void* uid );

  static int  cntCallSort;
};

#define DECLARE_UID()           \
  static guint32 maxUID;        \
  virtual void  ResetUID();     \
  virtual void  CorrectUID();   \
  virtual void  GenerateUID();

#define DEFINE_UID( name )                                        \
  guint32 name::maxUID;                                           \
  void  name::ResetUID()      { maxUID = 0; }                     \
  void  name::CorrectUID()    { if( maxUID < uid ) maxUID = uid; }\
  void  name::GenerateUID()   { uid = ++maxUID; }

#define DEFINE_UID_ZERO( name )                                   \
  guint32 name::maxUID;                                           \
  void  name::ResetUID()      { maxUID = 0; }                     \
  void  name::CorrectUID()    { if( maxUID < uid ) maxUID = uid; }\
  void  name::GenerateUID()   { }

//---------------------------------------------------------------------------

#endif                                                        
