/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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
  // ��������� ����������� ���� ������������������ �������
  arrImage.ForAny( (TForFunc)TImageFile::fileOpen, NULL );
}
//-----------------------------------------------------------------------------
void TFileShare::Close()
{
  Begin( dbgName() );

  // ���� ���������� "��������" �����
  if( cntChildFiles )
    // ��������� ��� �������� �������� �������� ���� �����
    arr.UnlinkChild( -1 );
  // ������� ������
  arr.Clear();
  // ��������� ����������� ���� ������������������ �������
  arrImage.ForAny( (TForFunc)TImageFile::fileClose, NULL );
  End();
}
//-----------------------------------------------------------------------------
void TFileShare::Done()
{
  lock();

  // ��� ������ ������ ���� ���������
  BL_ASSERT( arrImage.Count() == 0 );
  // �� ��� ����� ��������
  arrImage.Unlink();
  arrImage.Done();

  // ��� ������ ������ ���� ���������
  BL_ASSERT( arr.Count() == 0 );
  // �� ��� ����� ������� ������
  arr.Clear();
  arr.Done();

  unlock();
}
//-----------------------------------------------------------------------------
// �������� ��������������� ���� � ������ ������
// ��������������, ��� ����� ��� ������, ��������������� ���������������
// ������� SetLock(), ���������� ��� ���������� ���� "���������"
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
// ������� �� ������ ��������������� ����
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
// �������� ������ ��������������� ������
void TFileShare::ClearLock()
{
  lock();
  cntLock = 0;
  unlock();
}
//-----------------------------------------------------------------------------
// ������ ����� - ���������� ������� �� ������� ������
// ��� ������� ������� ����� ������������� ����� � ��� ��� "��������"
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
// ������������ ����� � ���� ��� "���������"
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
// ���������������� ����� ����� ������ � �����
bool TFileShare::RegisterImage( TImageFile* pImage )
{
  // ���� ����� ��� ��������������� - �����
  int index = arrImage.FindValue( pImage );
  if( index != -1 ) return true;

  // ��������� ����� � ������ � �������������� ���
  if( arrImage.Append(pImage) != -1 )
    return pImage->Init( &arr, this );
  BL_FIX_BUG();
  return false;
}
//-----------------------------------------------------------------------------
// �������� ������������� ����� ������ � �����
void TFileShare::UnregisterImage( TImageFile* pImage )
{
  //
  pImage->Done();
  // ���������� ��� �� ������
  int index = arrImage.FindValue( pImage );
  if( index != -1 )
    arrImage.Unlink( index );
}
//-----------------------------------------------------------------------------
// �������� ������ � ����
bool TFileShare::Insert( int index, PUIDobject p, PImageFile pImage )
{
  TImageFile::TFileInsert def;

  // ��������� ������ � ��
  // ��� ���������� ������ ����������� ���� <uid> !!!
  if( SaveObject( p ) )
  {
    BL_ASSERT( p->uid != UID_NULL );
    // �������� ������ ��� ������������������ � ��
    p->MarkAsSyncro();
    def.indexImage = index;

    // ���� �����, ����������� ������, �������� �� ������ � ������ �������
    // � ������ ������� �������
    if( pImage && (pImage->depend == TImageFile::depend_full) && (index != -1) )
      // ��������� ������ � ������ ����� � ��������� �������
      def.indexBase = arr.Insert( index, p ) ? index : -1;
    else
      // ��������� ������ � ������� ����� � ������� ������� �������
      def.indexBase = arr.Add( p );

    // ��������� ����������� ���� ������������������ �������
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
// ������� ������ �� �����
// index  - ������� �������� � ������� ������, ���������� ��������
// p      - ��������� ������
// pImage - �����, ��������� ��������
void TFileShare::Delete( PUIDobject p, int index, PImageFile pImage )
{
  TImageFile::TFileDel def;
  def.indexImage = index;
  // ���� �����, ����������� ������, �������� �� ������ � ������ �������
  // �� -     ������ � ������� ����� ������������� ��������� �������
  // ����� -  ���� ������������ ������� � ������� �����
  def.indexBase = ( (index != -1) && (!pImage || (pImage->depend == TImageFile::depend_full)) )
                  ? index
                  : arr.FindValue( p );
  // ������� ������ �� ��
  if( (def.indexBase != -1) )
  {
    if( DeleteObject( p ) )
    {
      // ������� ���� ����� �� �� ����� ��� ���������� ���� �������� � ��
      // (� �� ���������� ������� �� �� ��������)
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

      // ��������� ����������� ���� ������������������ �������
      def.pImage = pImage;
      def.p = p;
      arrImage.ForAny( (TForFunc)TImageFile::fileDelete, &def );

      // ������� ������ �� ������� ����� � ���������� ������
      arr.Delete( def.indexBase );
    }
  }
  else BL_FIX_BUG();
}
//-----------------------------------------------------------------------------
// ������� ����� �������� �� �� ���������������
// cntDel - ���-�� ��������
// arrDel - ������ ���������������
// pImage - �����, ��������� ��������
void TFileShare::DeleteArray( int cntDel, guint32* arrDel, PImageFile pImage )
{
  TImageFile::TFileDelArray def( cntDel );
  def.pImage = pImage;

  // ���� ������� �������� �� �� ���������������
  for( int i = 0; i < cntDel; i++ )
  {
    int index = arr.FindIndexUID( arrDel[i] );
    if( index != -1 )
    {
      TRecDelArray* pRec = def.arrRec.Append();
      // ��������� ������
      pRec->p = arr[index];
      // ������, �� �������� ���� ����������� �������� ������� � ������� ������
      pRec->indexBase = index;
    }
  }
  if( !def.arrRec.Count() ) return;

  // ������� ���� ����� �� �� ����� ��� ���������� ���� �������� � ��
  // (� �� ���������� ������� �� �� ��������)
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

  // ������� ���������� �� ����������� �������� ���� p->uid
  def.arrRec.Sort( TRecDelArray::SortPUID );

  // ��������� ����������� ���� ������������������ �������
  if( arrImage.Count() )
    arrImage.ForAny( (TForFunc)TImageFile::fileDeleteArray, &def );

  // ������� ���������� �� ����������� �������� ���� indexBase
  def.arrRec.Sort( TRecDelArray::SortIndexBaseUp );

  // ������� ������� �� ��
  DeleteObjects( def.arrRec );

  // ������� ������������� �� �����������. ��� �������� �������� � �����
  // ������� ������������ �������� �� ����������
  cntDel = def.arrRec.Count();
  TRecDelArray* pRec = def.arrRec[cntDel-1];
  for( int i = 0; i < cntDel; i++, pRec-- )
    arr.Delete( pRec->indexBase );
}
//-----------------------------------------------------------------------------
// ����������� �� ������ ������, ��� ��� �������� ������� ���� �������
// p - ���������� ������
// index - ������ ������� � ������
// pImage - �������� �����
bool TFileShare::CheckChange( PUIDobject p, int index, PImageFile pImage )
{
  TImageFile::TFileChange def;
  def.indexImage = index;
  // ���� �����, ����������� ������, �������� �� ������ � ������ �������
  // �� -     ������ � ������� ����� ������������� ��������� �������
  // ����� -  ���� ������������ ������� � ������� �����
  def.indexBaseOld = ( (index != -1) && (!pImage || (pImage->depend == TImageFile::depend_full)) )
                    ? index
                    : arr.FindValue( p );
  // ��������� ������ � ��
  if( (def.indexBaseOld != -1) )
  {
    if( SaveObject( p ) )
    {
      // �������� ������ ��� ������������������ � ��
      p->MarkAsSyncro();
      // ����� ������ ������� � ������� ������
      def.indexBaseNew = arr.CheckChange( def.indexBaseOld );
      def.pImage = pImage;
      def.p = p;
      // ��������� ����������� ���� ������������������ �������
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
    // ���� ����� �������� - ���������� � ����
    if( mParentFile )
      mParentFile->AddChildFromLoad( p->getUIDParent(), p, numToParent );
    return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// �������� ������ � ������ ��� ���������� � ����
// p      - ����������� ������
bool TFileShare::WithoutFile_Add( PUIDobject p )
{
  if( mParentFile )
    mParentFile->AddChild( p->getUIDParent(), p, numToParent );

  // ����� ������ ��� ����������� ������� ��������� �������
  TImageFile::TFileInsert def;

  // ������, � ������� ������������� ������� ������� � ������� ������
  def.indexBase = arr.Add( p );
  // ��������� ����������� ���� ������������������ �������
  if( def.indexBase != -1 )
  {
    if( arrImage.Count() )
    {
      def.p = p;          // ����������� ������
      def.pImage = NULL;  // ����� - ��������� ��������
      //def.indexImage;   // ������, � ������� ������������� ������� ������� � ������ - ����������
      arrImage.ForAny( (TForFunc)TImageFile::fileInsert, &def );
    }
    return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
// ����������� �� ��������� ������� ��� ���������� � ����
// p - ���������� ������
// index - ������ ������� � ������
void TFileShare::WithoutFile_Change( PUIDobject p, int index, TImageFile* pImage, guint32 id_parent )
{
  // ����� ������ ��� ����������� ������� ��������� �������
  TImageFile::TFileChange def;

  def.p = p;          // ��������� �� ���������� ������
  def.pImage = pImage; // ����� - ��������� ��������
  def.indexBaseOld = index; // ������ ������ ������� � ������� ������
  // ����� ������ ������� � ������� ������
  def.indexBaseNew = arr.CheckChange( def.indexBaseOld );

  // ��������� ����������� ���� ������������������ �������
  if( arrImage.Count() )
    arrImage.ForAny( (TForFunc)TImageFile::fileChange, &def );

  // �������� �������� �� ��������� �����
  if( mParentFile && (id_parent != UID_NULL) )                     // ???
    mParentFile->ChangeChild( id_parent, p, numToParent );
}
//-----------------------------------------------------------------------------
// ����������� �� ��������� ������� ��� ���������� � ����
// uid - ������������� �������
void TFileShare::WithoutFile_Change( guint32 uid, PImageFile pImage )
{
  int index = arr.FindIndexUID( uid );
  PUIDobject p = (PUIDobject)arr[index];
  if( p )
    WithoutFile_Change( p, index, pImage );
}
//-----------------------------------------------------------------------------
// ������� ������ �� ������� ��� �������� �� �����
// uid - ������������� ���������� �������
void TFileShare::WithoutFile_DeleteUID( guint32 uid, bool notifyParent )
{
  int index = arr.FindIndexUID( uid );
  WithoutFile_Delete( index, notifyParent );
}
//-----------------------------------------------------------------------------
// ������� ������ �� ������� ��� �������� �� �����
// index - ������ ���������� �������
void TFileShare::WithoutFile_Delete( int index, bool notifyParent )
{
  PUIDobject pUID = arr[index];
  if( !pUID ) return;

  // ������� ���� ����� �� �� ����� ��� ���������� ���� �������� � ��
  // ( � �� ���������� ������� �� �� ��������)
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

  // ������, �� �������� ���� ����������� �������� ������� � ������� ������
  def.indexBase = index;
  if( (def.indexBase != -1) )
  {
    def.p = (PUIDobject)arr[def.indexBase]; // ��������� ������
    def.pImage = NULL;          // ����� - ��������� ��������
    //def.indexImage;   // ������, �� �������� ������������� �������� ������� � ������ - ����������

    // ��������� ����������� ���� ������������������ �������
    arrImage.ForAny( (TForFunc)TImageFile::fileDelete, &def );

    // ������� ������ �� ������� ����� � ���������� ������
    arr.Delete( def.indexBase );
  }
}
//-----------------------------------------------------------------------------
// �������� ������ "��������" ������
void TFileShare::SetChilds( PFileShare* childFiles, int cntChilds )
{
  arrChildFiles = childFiles;
  cntChildFiles = cntChilds;
  for( int i = 0; i < cntChildFiles; i++ )
    arrChildFiles[i]->SetParent( this, i );
}
//-----------------------------------------------------------------------------
// ���������� ��������� ������� � �������� �������
// uid - ������������� �������� ������� (��������)
// pChild - �������� �������
// type - ��� ��������� ��������
// ���������: �������� ������ � ����� ���������
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
// ��������� ��������� ������� �������� �������
// uid - ������������� �������� ������� (��������)
// pChild - �������� �������
// type - ��� ��������� ��������
void TFileShare::ChangeChild( guint32 uid, PUIDobject , int  )// pChild, type )
{
  int index = arr.FindIndexUID( uid );
  PUIDobject p = (PUIDobject)arr[index];
  if( p )
    WithoutFile_Change( p, index, NULL );
  else BL_FIX_BUG();
}
//-----------------------------------------------------------------------------
// �������� ��������� ������� �� �������� �������
// uid - ������������� �������� ������� (��������)
// uidChild - �������� �������
// type - ��� ��������� ��������
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
// ������� ����� �������� �� �����
// ��� ������ ��� ����� ��� �� ������������ ��������� ��������
// �������� �������� � ���������� ��������
void TFileShare::DeleteObjects( TArrayRDA& arrDel )
{
  int cntDel = arrDel.Count();
  TRecDelArray* pRec = arrDel[cntDel-1];
  for( int i = 0; i < cntDel; i++, pRec-- )
    DeleteObject( pRec->p );
}
//-----------------------------------------------------------------------------

