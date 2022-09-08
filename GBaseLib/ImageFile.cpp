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


#include <string.h>
#include "ImageFile.h"

//=============================================================================
//=============================================================================
//=============================================================================
TImageFile::TImageFile()
{
  pArray = NULL;
  pFile = NULL;
  depend = depend_count;
#ifdef _DEBUG
  cntBlock = 0;
#endif
  selfNotifyChange = false;
  CmpFunc = NULL;
  filterFunc = NULL; // Функция фильтрации исходного массива
  name[0] = '\0';
  mNotify = NULL;
}
//-----------------------------------------------------------------------------
TImageFile::~TImageFile()
{
  Unregister();
}
//-----------------------------------------------------------------------------
void TImageFile::Config( const char* _name, IImageNotify  *notify, TDependence _depend, TCmpFunc cmpFunc )
{
  mNotify = notify;
  depend  = _depend;
  CmpFunc = cmpFunc;
  if( _name ) strcpy( name, _name );
  else BL_FIX_BUG();
  BL_ASSERT( (depend != independ_arr) || cmpFunc );
}
//-----------------------------------------------------------------------------
void TImageFile::ConfigFilter( TCmpFunc _filterFunc, void* _filterParam )
{
  BL_ASSERT( depend == independ_subArr );
  filterFunc  = _filterFunc;
  filterParam = _filterParam;
}
//-----------------------------------------------------------------------------
void TImageFile::ChangeFilterParam( void* _filterParam )
{
  // независимость набора от базового, подмножество базовых объектов
  BL_ASSERT( depend == independ_subArr );
  begin();
  filterParam = _filterParam;
  pArray->Unlink();
  pArray->AddFilteredValues( pFile->arr, filterFunc, filterParam );
  mHasChange = true;
  end();
}
//-----------------------------------------------------------------------------
bool TImageFile::Init( PArrUIDobject parr, PFileShare pfile )
{
  pFile = pfile;
  mHasChange = true;
  switch( depend )
  {
    case depend_full:   // полная зависимость набора от базового
    {
      pArray = parr;
      return true;
    }
    case independ_arr:  // независимость набора от базового, но не базовых объектах
    {
      if( !pArray ) pArray = new TArrUIDobject( *parr );
      bool res = pArray && pArray->AddValues( *parr );
      BL_ASSERT( res && parr->Count() == pArray->Count() );
      if( CmpFunc )
        pArray->Sort( CmpFunc, true );
      return res;
    }
    case independ_subArr:// независимость набора от базового, подмножество базовых объектов
    {
      if( !pArray ) pArray = new TArrUIDobject( *parr );
      BL_ASSERT( filterFunc );
      bool res = pArray && pArray->AddFilteredValues( *parr, filterFunc, filterParam );
      BL_ASSERT( res );
      if( CmpFunc )
        pArray->Sort( CmpFunc, true );
      return res;
    }
    default:
      BL_FIX_BUG();
      return false;
  }
}
//-----------------------------------------------------------------------------
void TImageFile::Done()
{
  pFile = NULL;
  if( pArray )
  {
    switch( depend )
    {
      case depend_full:   // полная зависимость набора от базового
      {
        break;
      }
      case independ_arr:      // независимость набора от базового
      case independ_subArr:   // независимость набора от базового
      {
        pArray->Unlink();
        delete pArray;
        break;
      }
      default:;
    }
    pArray = NULL;
  }
}
//-----------------------------------------------------------------------------
void TImageFile::Unregister()
{
  if( pFile )
  {
    pFile->UnregisterImage( this );
    pFile = NULL;
  }
}
//-----------------------------------------------------------------------------
// уведомлять себя об произведенных собою-же операциях (insert, delete, ... )
void TImageFile::SelfNotifyChange( bool flag )
{
  begin();
  selfNotifyChange = flag;
  end();
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Функция-обработчик события открытия файла
/*static*/void TImageFile::fileOpen( PImageFile* ppImage, PArrUIDobject parr )
{
  PImageFile pImage = *ppImage;
  BL_ASSERT( pImage->pArray && pImage->pFile );
  pImage->mHasChange = true;

  switch( pImage->depend )
  {
    case independ_arr:  // независимость набора от базового, но не базовых объектах
    {
      pImage->pArray->AddValues( *parr );
      if( pImage->CmpFunc )
        pImage->pArray->Sort( pImage->CmpFunc, true );
      break;
    }
    case independ_subArr:
    {
      bool res = pImage->pArray && pImage->pArray->AddFilteredValues( *parr, pImage->filterFunc, pImage->filterParam );
      if( res )
      {
        if( pImage->CmpFunc )
          pImage->pArray->Sort( pImage->CmpFunc, true );
      }
      else BL_FIX_BUG();
      break;
    }
    default:;
  }
  if( pImage->mNotify )
    pImage->mNotify->OnOpen( pImage );
}
//-----------------------------------------------------------------------------
// Функция-обработчик события закрытия файла
/*static*/void TImageFile::fileClose( PImageFile* ppImage, void* )
{
  PImageFile pImage = *ppImage;
  switch( pImage->depend )
  {
    case depend_full:   // полная зависимость набора от базового
    {
      break;
    }
    case independ_arr:  // независимость набора от базового
    case independ_subArr:
    {
      pImage->pArray->Unlink(); // Отключаемся от данных файла
      break;
    }
    default:;
  }
  if( pImage->mNotify )
    pImage->mNotify->OnClose( pImage );
}
//-----------------------------------------------------------------------------
// Функция-обработчик события вставки объекта в файла
/*static*/void TImageFile::fileInsert( PImageFile* ppImage, TFileInsert* def )
{
  PImageFile pImage = *ppImage;
  switch( pImage->depend )
  {
    case independ_subArr: // независимость набора от базового, подмножество базовых объектов
    {
      // вставлен объект не попадающий в подмножество - игнорируем
      if( pImage->filterFunc( def->p, pImage->filterParam ) != 0 )
        return;
      // иначе обрабатываем как и для <independ_arr>
    }
    case independ_arr:
    {
      // образ - инициатор операции и задан индекс позиции вставка
      if( (pImage == def->pImage) && (def->indexImage != -1) )
      {
        pImage->pArray->Insert( def->indexImage, def->p );

        pImage->indexRes = def->indexImage;
      }
      else
      {
        pImage->indexRes = pImage->pArray->Add( def->p );
      }
      break;
    }
    case depend_full:
      pImage->indexRes = def->indexBase;
      break;
    default: return;
  }
  // Фиксируем произведение изменения
  pImage->mHasChange = true;

  pImage->doNotifyInsert( def->pImage, pImage->indexRes, def->p );
}
//-----------------------------------------------------------------------------
// Оповещение о событии добавления объекта
void TImageFile::doNotifyInsert( TImageFile* imageSrc, int index, PUIDobject p )
{
  // Если задано уведомлять себя об произведенных собою-же операциях или
  // операция была произведена кем-то другим
  if( selfNotifyChange || (imageSrc != this) )
    // уведомляем
    if( mNotify )
      mNotify->OnInsert( this, index, p );
}
//-----------------------------------------------------------------------------
// Функция-обработчик события удаления объекта из файла
/*static*/void TImageFile::fileDelete( PImageFile* ppImage, TFileDel* def )
{
  PImageFile pImage = *ppImage;
  switch( pImage->depend )
  {
    case independ_subArr: // независимость набора от базового, подмножество базовых объектов
      pImage->indexRes = ( (pImage == def->pImage) && (def->indexImage != -1) )
                          ? def->indexImage
                          : pImage->pArray->FindValue( def->p );
      if( pImage->indexRes == -1 )
        return;
      else
        BL_ASSERT( pImage->pArray->Unlink( pImage->indexRes ) );
      break;
    case independ_arr:
      pImage->indexRes = ( (pImage == def->pImage) && (def->indexImage != -1) )
                          ? def->indexImage
                          : pImage->pArray->FindValue( def->p );
      BL_ASSERT( pImage->indexRes != -1 );
      BL_ASSERT( pImage->pArray->Unlink( pImage->indexRes ) );
      break;
    case depend_full:
      pImage->indexRes = def->indexBase;
      break;
    default: return;
  }
  // Фиксируем произведение изменения
  pImage->mHasChange = true;

  pImage->doNotifyDelete( def->pImage, pImage->indexRes, def->p );
}
//-----------------------------------------------------------------------------
// Оповещение о событии удаления объекта
void TImageFile::doNotifyDelete( TImageFile* imageSrc, int index, PUIDobject p )
{
  // Если задано уведомлять себя об произведенных собою-же операциях или
  // операция была произведена кем-то другим
  if( selfNotifyChange || (imageSrc != this) )
    // уведомляем
    if( mNotify )
      mNotify->OnDelete( this, index, p );
}
//-----------------------------------------------------------------------------
    // Функция сортировки по возрастанию значения поля indexBase
int TFileShare::TRecDelArray::SortIndexBaseUp( const void* p1, const void* p2 )
{
  int v1 = ((TFileShare::TRecDelArray*)p1)->indexBase;
  int v2 = ((TFileShare::TRecDelArray*)p2)->indexBase;
  return ( v1 < v2 ) ? -1 :
         ( v1 > v2 ) ? 1 : 0;
}
//-----------------------------------------------------------------------------
// Функция сортировки по возрастанию значения поля p->uid
int TFileShare::TRecDelArray::SortPUID( const void* p1, const void* p2 )
{
  guint32 v1 = ((TFileShare::TRecDelArray*)p1)->p->uid;
  guint32 v2 = ((TFileShare::TRecDelArray*)p2)->p->uid;
  return ( v1 < v2 ) ? -1 :
         ( v1 > v2 ) ? 1 : 0;
}
//-----------------------------------------------------------------------------
// Функция поиска значения поля p->uid
int TFileShare::TRecDelArray::FindPUID( const void* p1, const void* p2 )
{
  guint32 v1 = ((TFileShare::TRecDelArray*)p1)->p->uid;
  return ( v1 < (guint32)(size_t)p2 ) ? -1 :
         ( v1 > (guint32)(size_t)p2 ) ? 1 : 0;
}
//-----------------------------------------------------------------------------
// Функция-обработчик события удаления массива объектов из файла
/*static*/void TImageFile::fileDeleteArray( PImageFile* ppImage, TFileDelArray *def )
{
  PImageFile pImage = *ppImage;

  switch( pImage->depend )
  {
    case independ_subArr: // независимость набора от базового, подмножество базовых объектов
    case independ_arr:
    {
      for( int i = pImage->pArray->Count(); i > 0; i-- )
      {
        PUIDobject p = (PUIDobject)pImage->pArray->Item( i-1 );
        if( -1 != def->arrRec.FastSearch( (void*)(size_t)p->uid, NULL, TFileShare::TRecDelArray::FindPUID ) )
          pImage->pArray->Unlink( i-1 );
      }
/*      DWORD cntDel = def->arrRec.Count();
      TImageFile::TRecDelArray* pRec = def->arrRec.baseItem();
      for( DWORD i = 0; i < cntDel; i++, pRec++ )
      {
        DWORD indexRes = pImage->pArray->FindValue( pRec->p );
        if( indexRes != NO_DWORD )
          pImage->pArray->Unlink( indexRes );
      }*/
      break;
    }
    case depend_full:
      break;
    default: return;
  }
  pImage->indexRes = -1;
  // Фиксируем произведение изменения
  pImage->mHasChange = true;

  pImage->doNotifyDeleteArray( def->pImage );
}
//-----------------------------------------------------------------------------
// Оповещение о событии удаления объекта
void TImageFile::doNotifyDeleteArray( TImageFile* imageSrc )
{
  // Если задано уведомлять себя об произведенных собою-же операциях или
  // операция была произведена кем-то другим
  if( selfNotifyChange || (imageSrc != this) )
    // уведомляем
    if( mNotify )
      mNotify->OnDeleteArray( this );
}
//-----------------------------------------------------------------------------
// Функция-обработчик события коррекции объекта в файле
/*static*/void TImageFile::fileChange( PImageFile* ppImage, TFileChange* def )
{
  PImageFile pImage = *ppImage;
  switch( pImage->depend )
  {
    case independ_subArr:
    {
      pImage->indexOldChange = ( pImage == def->pImage )
                              ? def->indexImage
                              : pImage->pArray->FindValue( def->p );
      bool nowIncluded = pImage->filterFunc( def->p, pImage->filterParam ) == 0;
      if( pImage->indexOldChange != -1 )
      {
        if( nowIncluded ) // ...был в подмножестве и остался в нем...
        {
          pImage->indexRes = pImage->pArray->CheckChange( pImage->indexOldChange );
        }
        else              // ...был в подмножестве, но вышел из него...
        {
          // выкидываем из своего набора
          BL_ASSERT( pImage->pArray->Unlink( pImage->indexOldChange ) );
          pImage->indexRes = -1;

          // Если задано уведомлять себя об произведенных собою-же операциях или
          // операция была произведена кем-то другим - уведомляем
          if( pImage->selfNotifyChange || (def->pImage != pImage) )
            if( pImage->mNotify )
              pImage->mNotify->OnDelete( pImage, pImage->indexOldChange, def->p );
          return;
        }
      }
      else
      {
        if( nowIncluded ) // ...не был в подмножестве, но вошел в него...
        {
          pImage->indexRes = pImage->pArray->Add( def->p );

          // Если задано уведомлять себя об произведенных собою-же операциях или
          // операция была произведена кем-то другим - уведомляем
          if( pImage->selfNotifyChange || (def->pImage != pImage) )
            if( pImage->mNotify )
              pImage->mNotify->OnInsert( pImage, pImage->indexRes, def->p );
          return;
        }
        else             // ...не был в подмножестве, и не вошел в него...
          return; // игнорируем событие
      }
      break;
    }
    case independ_arr:
    {
      pImage->indexOldChange = ( pImage == def->pImage )
                              ? def->indexImage
                              : pImage->pArray->FindValue( def->p );
      BL_ASSERT( pImage->indexOldChange != -1 );

      pImage->indexRes = pImage->pArray->CheckChange( pImage->indexOldChange );
      break;
    }
    case depend_full:
      pImage->indexOldChange = def->indexBaseOld;
      pImage->indexRes = def->indexBaseNew;
      break;
    default: return;                                
  }
  // Фиксируем произведение изменения
  pImage->mHasChange = true;
  pImage->doNotifyChange( def->pImage, pImage->indexOldChange, pImage->indexRes, def->p );
}
//-----------------------------------------------------------------------------
// Оповещение о событии изменения объекта
void TImageFile::doNotifyChange( TImageFile* imageSrc, int oldIndex, int index, PUIDobject p )
{
  // Если задано уведомлять себя об произведенных собою-же операциях или
  // операция была произведена кем-то другим
  if( selfNotifyChange || (imageSrc != this) )
    // уведомляем
    if( mNotify )
      mNotify->OnChange( this, oldIndex, index, p );
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Отсортировать массив образа данных
// Использовать только для образов данных зарегистрированных как <independ_arr> и <independ_obj>
// Входные параметры:
// cmpFunc - функция сравнения
// doSort - команда "произвести сортировку"
// (иначе просто назначается функция сравнения для дальнейшего использования)
void TImageFile::Sort( TCmpFunc cmpFunc, bool doSort )
{
  BL_ASSERT( depend > depend_full );
  BL_ASSERT( pArray );

  if( (depend == independ_arr) || (depend == independ_subArr) ) begin();
  CmpFunc = cmpFunc;
  pArray->Sort( cmpFunc, doSort);
  if( (depend == independ_arr) || (depend == independ_subArr) ) end();
}
//-----------------------------------------------------------------------------
// Добавить объект
// p - добавляемый объект
// Результат: позиция вставки элемента в образ
int TImageFile::Add( PUIDobject p )
{
  BL_ASSERT( isCorrectLock() );
  return pFile->Insert( -1, p, this ) ? indexRes : -1;
}
//-----------------------------------------------------------------------------
bool TImageFile::Insert( int index, PUIDobject p )
{
  BL_ASSERT( isCorrectLock() );
  BL_ASSERT( p->uid == UID_NULL );

  return pFile->Insert( index, p, this );
}
//-----------------------------------------------------------------------------
// Получить копию объекта
bool TImageFile::GetCopy( guint32 uid, TUIDobject& r )
{
  r.uid = UID_NULL;
  begin();
  PUIDobject p = FindUID( uid );
  if( p ) r.Assign( *p );
  end();
  return r.uid != UID_NULL;
}
//-----------------------------------------------------------------------------
// Заменить значение объекта
bool TImageFile::Replace( TUIDobject& r )
{
  bool res = false;
  begin();
  int index = FindIndexUID( r.uid );
  PUIDobject p = Item( index );
  if( p ) 
  {
    p->Assign( r );
    res = true;
  }
  CheckChange( index );
  end();
  return res;
}
//=============================================================================
//=============================================================================
//=============================================================================
// Удалить объект
void TImageFile::Delete( PUIDobject p )
{
  BL_ASSERT( isCorrectLock() );

  int index = pArray->FindValue( p );
  if( index != -1 ) pFile->Delete( p, index, this );
}
//-----------------------------------------------------------------------------
// index - индекс объекта в массиве
void TImageFile::Delete( int index )
{
  BL_ASSERT( isCorrectLock() );

  PUIDobject p = (PUIDobject)(*pArray)[index];
  if( p ) pFile->Delete( p, index, this );
}
//-----------------------------------------------------------------------------
// uid - уникальный идентификатор объекта
void TImageFile::DeleteUID( guint32 uid )
{
  BL_ASSERT( isCorrectLock() );

  int index = FindIndexUID( uid );
  PUIDobject p = (PUIDobject)(*pArray)[index];
  if( p ) pFile->Delete( p, index, this );
}
//-----------------------------------------------------------------------------
// Удалить набор объектов по их идентификаторам
// cntDel - кол-во объектов
// arrDel - массив идентификаторов
void TImageFile::Delete( int cntUID, guint32* arrUID )
{
  BL_ASSERT( isCorrectLock() );
  pFile->DeleteArray( cntUID, arrUID, this );
}
//=============================================================================
//=============================================================================
//=============================================================================
// Уведомить образ об изменении объекта
// Проверить порядок сортировки после проведенной коррекции объекта
// Результат: новая позиция скорректированного объекта (может остаться и прежней).
int TImageFile::CheckChange( int index )
{
  BL_ASSERT( isCorrectLock() );
  BL_ASSERT( index != -1 );

  PUIDobject p = (PUIDobject)(*pArray)[index];

  return ( p && pFile->CheckChange( p, index, this ) ) ? indexRes : index;
}
//-----------------------------------------------------------------------------
// Проверить порядок сортировки после проведенной коррекции объекта
// Результат: новая позиция скорректированного объекта
int TImageFile::CheckChange( PUIDobject p )
{
  BL_ASSERT( isCorrectLock() );
  int index = pArray->FindValue( p );
  return ( (index != -1) && pFile->CheckChange( p, index, this ) ) ? indexRes : index;
}
//-----------------------------------------------------------------------------
void TImageFile::WithoutFile_Change( PUIDobject p, int index )
{
  pFile->WithoutFile_Change( p, index, this );
}
//-----------------------------------------------------------------------------
// Подсчет кол-ва сбоев сортировки (невыполнения условия a[i-1] <= a[i]
// pError - [out] индекс объекта нарушающего порядок сортировки (может быть NULL)
// Результат: кол-во сбоев
int TImageFile::CountErrSort( int *pError )
{
  begin();
  int res = pArray->CountErrSort( pError );
  end();
  return res;
}
//-----------------------------------------------------------------------------
