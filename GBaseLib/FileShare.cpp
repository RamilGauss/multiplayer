/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012, 2013
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


#include <string.h>
#include <stdlib.h>
#include "FileShare.h"
#include "ImageFile.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
TFileShare::TFileShare( const char* _dbgname ) : GCS( _dbgname )
{
  arrLock[0].pFile = this;
  arrLock[0].exclusive = true;
  cntLock = 1;
  cntChildFiles = 0;
  mParentFile = NULL;
}
//-----------------------------------------------------------------------------
void TFileShare::OnAfterOpen()
{
  // Рассылаем уведомление всем зарегистрированным образам
  arrImage.ForAny( (TForFunc)TImageFile::fileOpen, NULL );
}
//-----------------------------------------------------------------------------
void TFileShare::Close()
{
  Begin( dbgName() );

  // Если существуют "дочерние" файлы
  if( cntChildFiles )
    // Исключить без удаления дочерние элементы всех типов
    arr.UnlinkChild( -1 );
  // очищаем данные
  arr.Clear();
  // Рассылаем уведомление всем зарегистрированным образам
  arrImage.ForAny( (TForFunc)TImageFile::fileClose, NULL );
  End();
}
//-----------------------------------------------------------------------------
void TFileShare::Done()
{
  lock();

  // Все образы должны быть отключены
  BL_ASSERT( arrImage.Count() == 0 );
  // Но все равно отключим
  arrImage.Unlink();
  arrImage.Done();

  // Все данные должны быть выгружены
  BL_ASSERT( arr.Count() == 0 );
  // Но все равно убиваем данные
  arr.Clear();
  arr.Done();

  unlock();
}
//-----------------------------------------------------------------------------
// Вставить вспомогательный файл в начало списка
// Предполагается, что буфер под список, предоставленный предварительным
// вызовом SetLock(), достаточен для размещения всех "субфайлов"
void TFileShare::PushFirstLock( PFileShare pFile, bool exclusive )
{
  lock();
  if( cntLock < l_MaxCount )
  {
    memmove( arrLock + 1, arrLock, sizeof(TRecLock)*cntLock );

    arrLock[0].pFile = pFile;
    arrLock[0].exclusive = exclusive;
    cntLock++;
  }
  else BL_FIX_BUG();
  unlock();
}
//-----------------------------------------------------------------------------
// Удалить из списка вспомогательный файл
void TFileShare::DelLock( PFileShare pFile )
{
  lock();
  for( int i = 0; i < cntLock; i++ )
    if( arrLock[i].pFile == pFile )
    {
      memmove( arrLock + i, arrLock + i + 1, sizeof(TRecLock)*(cntLock-i-1) );
      cntLock--;
      break;
    }
  unlock();
}
//-----------------------------------------------------------------------------
// Очистить список вспомогательный файлов
void TFileShare::ClearLock()
{
  lock();
  cntLock = 0;
  unlock();
}
//-----------------------------------------------------------------------------
// Захват файла - блокировка доступа из потоков других
// При захвате данного файла захватываются также и все его "субфайлы"
void TFileShare::Begin( const char* _ownerLock )
{
  if( cntLock )
  {
    int cnt = cntLock;
    TRecLock* p = arrLock;
    do
    {
      if( p->exclusive )  p->pFile->lock();
      else                p->pFile->Begin( _ownerLock );
      p++;
    }
    while( --cnt );
  }
  ownerLock = _ownerLock;
}
//-----------------------------------------------------------------------------
// Освобождение файла и всех его "субфайлов"
void TFileShare::End()
{
  if( ownerLock ) ownerLock++;
  if( cntLock )
  {
    int cnt = cntLock;
    TRecLock* p = arrLock + cntLock - 1;
    do
    {
      if( p->exclusive )  p->pFile->unlock();
      else                p->pFile->End();
      p--;
    }
    while( --cnt );
  }
}
//-----------------------------------------------------------------------------
// Зарегистрировать новый образ данных в файле
bool TFileShare::RegisterImage( TImageFile* pImage )
{
  // Исли образ уже зарегистрирован - выход
  int index = arrImage.FindValue( pImage );
  if( index != -1 ) return true;

  // Добавляем образ в список и инициализируем его
  if( arrImage.Append(pImage) != -1 )
    return pImage->Init( &arr, this );
  BL_FIX_BUG();
  return false;
}
//-----------------------------------------------------------------------------
// Отменить регистрирацию образ данных в файле
void TFileShare::UnregisterImage( TImageFile* pImage )
{
  //
  pImage->Done();
  // Выкидываем его из списка
  int index = arrImage.FindValue( pImage );
  if( index != -1 )
    arrImage.Unlink( index );
}
//-----------------------------------------------------------------------------
// Вставить объект в файл
bool TFileShare::Insert( int index, PUIDobject p, PImageFile pImage )
{
  TImageFile::TFileInsert def;

  // Сохраняем объект в БД
  // При сохранении должно настроиться поле <uid> !!!
  if( SaveObject( p ) )
  {
    BL_ASSERT( p->uid != UID_NULL );
    // Пометить запись как синхронизированную с БД
    p->MarkAsSyncro();
    def.indexImage = index;

    // Если образ, вставляющий запись, работает на едином с файлом массиве
    // и задана позиция вставки
    if( pImage && (pImage->depend == TImageFile::depend_full) && (index != -1) )
      // вставляем объект в массив файла в указанную позицию
      def.indexBase = arr.Insert( index, p ) ? index : -1;
    else
      // добавляем объект к массиву файла с поиском позиции вставки
      def.indexBase = arr.Add( p );

    // Рассылаем уведомление всем зарегистрированным образам
    if( def.indexBase != -1 )
    {
      def.p = p;
      def.pImage = pImage;
      arrImage.ForAny( (TForFunc)TImageFile::fileInsert, &def );
      return true;
    }
    else BL_FIX_BUG();
  }
  return false;
}
//-----------------------------------------------------------------------------
// Удалить объект из файла
// index  - позиция удаления в массиве образа, инициатора удаления
// p      - удаляемый объект
// pImage - образ, инициатор удаления
void TFileShare::Delete( PUIDobject p, int index, PImageFile pImage )
{
  TImageFile::TFileDel def;
  def.indexImage = index;
  // Если образ, вставляющий запись, работает на едином с файлом массиве
  // то -     индекс в массиве файла соответствует заданному индексу
  // иначе -  ищем расположение объекта в массиве файла
  def.indexBase = ( (index != -1) && (!pImage || (pImage->depend == TImageFile::depend_full)) )
                  ? index
                  : arr.FindValue( p );
  // удаляем объект из БД
  if( (def.indexBase != -1) )
  {
    if( DeleteObject( p ) )
    {
      // удаляем всех дочек из их файла без проведения этой операции в БД
      // (в БД отработает триггер по их удалению)
      for( int ic = 0; ic < cntChildFiles; ic++ )
      {
        int cnt = p->CountChild( ic );
        for( int iu = 0; iu < cnt; iu++ )
        {
          guint32 uid = p->GetUIDChild( iu, ic );
          if( uid != UID_NULL )
            arrChildFiles[ic]->WithoutFile_DeleteUID( uid, false );
        }
        p->UnlinkChild( ic );
      }

      // Рассылаем уведомление всем зарегистрированным образам
      def.pImage = pImage;
      def.p = p;
      arrImage.ForAny( (TForFunc)TImageFile::fileDelete, &def );

      // удаляем объект из массива файла и уничтожаем объект
      arr.Delete( def.indexBase );
    }
  }
  else BL_FIX_BUG();
}
//-----------------------------------------------------------------------------
// Удалить набор объектов по их идентификаторам
// cntDel - кол-во объектов
// arrDel - массив идентификаторов
// pImage - образ, инициатор удаления
void TFileShare::DeleteArray( int cntDel, guint32* arrDel, PImageFile pImage )
{
  TImageFile::TFileDelArray def( cntDel );
  def.pImage = pImage;

  // ищем индексы объектов по их идентифираторам
  for( int i = 0; i < cntDel; i++ )
  {
    int index = arr.FindIndexUID( arrDel[i] );
    if( index != -1 )
    {
      TRecDelArray* pRec = def.arrRec.Append();
      // удаляемый объект
      pRec->p = arr[index];
      // индекс, из которого было произведено удаление объекта в базовом наборе
      pRec->indexBase = index;
    }
  }
  if( !def.arrRec.Count() ) return;

  // удаляем всех дочек из их файла без проведения этой операции в БД
  // (в БД отработает триггер по их удалению)
  if( cntChildFiles )
    for( int i = 0; i < def.arrRec.Count(); i++ )
    {
      PUIDobject pUID = def.arrRec[i]->p;
      for( int ic = 0; ic < cntChildFiles; ic++ )
      {
        int cnt = pUID->CountChild( ic );
        for( int iu = 0; iu < cnt; iu++ )
        {
          guint32 uid = pUID->GetUIDChild( iu, ic );
          if( uid != UID_NULL )
            arrChildFiles[ic]->WithoutFile_DeleteUID( uid, false );
        }
        pUID->UnlinkChild( ic );
      }
    }

  // Функция сортировки по возрастанию значения поля p->uid
  def.arrRec.Sort( TRecDelArray::SortPUID );

  // Рассылаем уведомление всем зарегистрированным образам
  if( arrImage.Count() )
    arrImage.ForAny( (TForFunc)TImageFile::fileDeleteArray, &def );

  // Функция сортировки по возрастанию значения поля indexBase
  def.arrRec.Sort( TRecDelArray::SortIndexBaseUp );

  // удаляем объекты из БД
  DeleteObjects( def.arrRec );

  // Индексы отсортированы по возрастанию. При удалении объектов с конца
  // массива актуальность индексов не нарушиться
  cntDel = def.arrRec.Count();
  TRecDelArray* pRec = def.arrRec[cntDel-1];
  for( int i = 0; i < cntDel; i++, pRec-- )
    arr.Delete( pRec->indexBase );
}
//-----------------------------------------------------------------------------
// Уведомление от образа данных, что его владелец изменил поля объекта
// p - измененный объект
// index - индекс объекта в образе
// pImage - исходный образ
bool TFileShare::CheckChange( PUIDobject p, int index, PImageFile pImage )
{
  TImageFile::TFileChange def;
  def.indexImage = index;
  // Если образ, вставляющий запись, работает на едином с файлом массиве
  // то -     индекс в массиве файла соответствует заданному индексу
  // иначе -  ищем расположение объекта в массиве файла
  def.indexBaseOld = ( (index != -1) && (!pImage || (pImage->depend == TImageFile::depend_full)) )
                    ? index
                    : arr.FindValue( p );
  // Сохраняем объект в БД
  if( (def.indexBaseOld != -1) )
  {
    if( SaveObject( p ) )
    {
      // Пометить запись как синхронизированную с БД
      p->MarkAsSyncro();
      // новый индекс объекта в базовом наборе
      def.indexBaseNew = arr.CheckChange( def.indexBaseOld );
      def.pImage = pImage;
      def.p = p;
      // Рассылаем уведомление всем зарегистрированным образам
      if( arrImage.Count() )
        arrImage.ForAny( (TForFunc)TImageFile::fileChange, &def );
      if( mParentFile )
        mParentFile->ChangeChild( p->getUIDParent(), p, numToParent );
      return true;
    }
  }
  else BL_FIX_BUG();
  return false;
}
//-----------------------------------------------------------------------------
bool TFileShare::AppendFromLoad( PUIDobject p )
{
  if( arr.Append( p ) != -1 )
  {
    // Если задан родитель - включаемся в него
    if( mParentFile )
      mParentFile->AddChildFromLoad( p->getUIDParent(), p, numToParent );
    return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Вставить объект в массив без сохранения в файл
// p      - добавляемый объект
bool TFileShare::WithoutFile_Add( PUIDobject p )
{
  if( mParentFile )
    mParentFile->AddChild( p->getUIDParent(), p, numToParent );

  // Набор данных для обработчика события изменения объекта
  TImageFile::TFileInsert def;

  // индекс, в который произведиться вставка объекта в базовом наборе
  def.indexBase = arr.Add( p );
  // Рассылаем уведомление всем зарегистрированным образам
  if( def.indexBase != -1 )
  {
    if( arrImage.Count() )
    {
      def.p = p;          // добавляемый объект
      def.pImage = NULL;  // образ - инициатор операции
      //def.indexImage;   // индекс, в который произведиться вставка объекта в образе - инициаторе
      arrImage.ForAny( (TForFunc)TImageFile::fileInsert, &def );
    }
    return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
// Уведомление от изменении объекта без сохранения в файл
// p - измененный объект
// index - индекс объекта в образе
void TFileShare::WithoutFile_Change( PUIDobject p, int index, TImageFile* pImage, guint32 id_parent )
{
  // Набор данных для обработчика события изменения объекта
  TImageFile::TFileChange def;

  def.p = p;          // указатель на измененный объект
  def.pImage = pImage; // образ - инициатор операции
  def.indexBaseOld = index; // старый индекс объекта в базовом наборе
  // новый индекс объекта в базовом наборе
  def.indexBaseNew = arr.CheckChange( def.indexBaseOld );

  // Рассылаем уведомление всем зарегистрированным образам
  if( arrImage.Count() )
    arrImage.ForAny( (TForFunc)TImageFile::fileChange, &def );

  // Уведомим родителя об изменении дочки
  if( mParentFile && (id_parent != UID_NULL) )                     // ???
    mParentFile->ChangeChild( id_parent, p, numToParent );
}
//-----------------------------------------------------------------------------
// Уведомление от изменении объекта без сохранения в файл
// uid - идентификатор объекта
void TFileShare::WithoutFile_Change( guint32 uid, PImageFile pImage )
{
  int index = arr.FindIndexUID( uid );
  PUIDobject p = (PUIDobject)arr[index];
  if( p )
    WithoutFile_Change( p, index, pImage );
}
//-----------------------------------------------------------------------------
// Удалить объект из массива без удаления из файла
// uid - идентификатор удаляемого объекта
void TFileShare::WithoutFile_DeleteUID( guint32 uid, bool notifyParent )
{
  int index = arr.FindIndexUID( uid );
  WithoutFile_Delete( index, notifyParent );
}
//-----------------------------------------------------------------------------
// Удалить объект из массива без удаления из файла
// index - индекс удаляемого объекта
void TFileShare::WithoutFile_Delete( int index, bool notifyParent )
{
  PUIDobject pUID = arr[index];
  if( !pUID ) return;

  // удаляем всех дочек из их файла без проведения этой операции в БД
  // ( в БД отработает триггер по их удалению)
  for( int ic = 0; ic < cntChildFiles; ic++ )
  {
    int cnt = pUID->CountChild( ic );
    for( int iu = 0; iu < cnt; iu++ )
    {
      guint32 uid = pUID->GetUIDChild( iu, ic );
      if( uid != UID_NULL )
        arrChildFiles[ic]->WithoutFile_DeleteUID( uid, false );
    }
    pUID->UnlinkChild( ic );
  }
  if( notifyParent && mParentFile )
    mParentFile->DelChild( pUID->getUIDParent(), pUID->uid, numToParent );

  TImageFile::TFileDel def;

  // индекс, из которого было произведено удаление объекта в базовом наборе
  def.indexBase = index;
  if( (def.indexBase != -1) )
  {
    def.p = (PUIDobject)arr[def.indexBase]; // удаляемый объект
    def.pImage = NULL;          // образ - инициатор операции
    //def.indexImage;   // индекс, из которого произведиться удаление объекта в образе - инициаторе

    // Рассылаем уведомление всем зарегистрированным образам
    arrImage.ForAny( (TForFunc)TImageFile::fileDelete, &def );

    // удаляем объект из массива файла и уничтожаем объект
    arr.Delete( def.indexBase );
  }
}
//-----------------------------------------------------------------------------
// Указание списка "дочерних" файлов
void TFileShare::SetChilds( PFileShare* childFiles, int cntChilds )
{
  arrChildFiles = childFiles;
  cntChildFiles = cntChilds;
  for( int i = 0; i < cntChildFiles; i++ )
    arrChildFiles[i]->SetParent( this, i );
}
//-----------------------------------------------------------------------------
// Добавление дочернего объекта к элементу массива
// uid - идентификатор элемента массива (родителя)
// pChild - дочерний элемент
// type - тип дочернего элемента
// Результат: родитель найден и дочка добавлена
bool TFileShare::AddChild( guint32 uid, PUIDobject pChild, int type )
{
  int index = arr.FindIndexUID( uid );
  PUIDobject p = arr[index];
  if( p )
  {
    p->AddChild( pChild, type );
    WithoutFile_Change( p, index, NULL );
    return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
bool TFileShare::AddChildFromLoad( guint32 uid, PUIDobject pChild, int type )
{
#if 1
  static guint32 ACFL_lastUID = UID_NULL;
  static int ACFL_lastIndex = -1;
  if( uid != ACFL_lastUID )
  {
    ACFL_lastUID = uid;
    ACFL_lastIndex = arr.FindIndexUID( uid );
  }
  PUIDobject p = arr[ACFL_lastIndex];
  if( p )
  {
    p->AddChild( pChild, type );
    return true;
  }
  return false;
#else
  int index = arr.FindIndexUID( uid );
  PUIDobject p = arr[index];
  if( p )
  {
    p->AddChild( pChild, type );
    return true;
  }
  return false;
#endif
}
//-----------------------------------------------------------------------------
// Изменение дочернего объекта элемента массива
// uid - идентификатор элемента массива (родителя)
// pChild - дочерний элемент
// type - тип дочернего элемента
void TFileShare::ChangeChild( guint32 uid, PUIDobject , int  )// pChild, type )
{
  int index = arr.FindIndexUID( uid );
  PUIDobject p = (PUIDobject)arr[index];
  if( p )
    WithoutFile_Change( p, index, NULL );
  else BL_FIX_BUG();
}
//-----------------------------------------------------------------------------
// Удаление дочернего объекта из элемента массива
// uid - идентификатор элемента массива (родителя)
// uidChild - дочерний элемент
// type - тип дочернего элемента
void TFileShare::DelChild( guint32 uid, guint32 uidChild, int type )
{
  int index = arr.FindIndexUID( uid );
  PUIDobject p = (PUIDobject)arr[index];
  if( p )
  {
    p->DelChild( uidChild, type );
    WithoutFile_Change( p, index, NULL );
  }
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Удалить набор объектов из файла
// Для файлов тех типов что не поддерживают групповое удаление
// приводим ситуацию к одиночному удалению
void TFileShare::DeleteObjects( TArrayRDA& arrDel )
{
  int cntDel = arrDel.Count();
  TRecDelArray* pRec = arrDel[cntDel-1];
  for( int i = 0; i < cntDel; i++, pRec-- )
    DeleteObject( pRec->p );
}
//-----------------------------------------------------------------------------

