/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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

// ����� ������ ��� ���������/��������������
class GBASELIB_EI TImageFile : public TObject
{
#ifdef _DEBUG
  int cntBlock;
#endif
  // ���������� ���� �� ������������� �����-�� ��������� (insert, delete, ... )
  bool selfNotifyChange;

  volatile bool mHasChange;        // ������� ��������� � ������ ������
public:

#ifdef _DEBUG
  bool IsBlock() { return cntBlock > 0; }
#endif
  enum TDependence
  {
    depend_full,  // ������ ����������� ������ �� ��������
    independ_arr, // ������������� ������ �� ��������, �� �� ������� ��������
    independ_subArr,// ������������� ������ �� ��������, ������������ ������� ��������
    //independ_obj, // ������������� � ������ � �������� ������ �� �������
    depend_count
  };
protected:
  TDependence   depend;     // ������� ����������� ������ ������ �� �������� ������
  IImageNotify  *mNotify;   // ������ �������� ����������� �� ��������� ���������

  PArrUIDobject pArray;     // ��������� �� ����� ������
  int          indexRes;    // ������ ��������� �����������/���������/���������� ������
  int          indexOldChange; // ������ ��������� ���������� ������ �� ��������������

  TCmpFunc      filterFunc; // ������� ���������� ��������� �������
  void*         filterParam;// �������� ����������

  TCmpFunc      CmpFunc;    // ������� ���������� ������� (�������� pArray->CmpFunc)
  char          name[255];  // �������� ��� (��� �������)

  // ����� ������ ��� ����������� ������� ����������/������� ������� � ����
  struct TFileInsert {
    PUIDobject p;       // ����������� ������
    PImageFile pImage;  // ����� - ��������� ��������
    int indexBase;      // ������, � ������� ������������� ������� ������� � ������� ������
    int indexImage;     // ������, � ������� ������������� ������� ������� � ������ - ����������
  };

  // ����� ������ ��� ����������� ������� �������� ������� �� �����
  struct TFileDel {
    PUIDobject p;       // ��������� ������
    PImageFile pImage;  // ����� - ��������� ��������
    int indexImage;     // ������, �� �������� ������������� �������� ������� � ������ - ����������
    int indexBase;      // ������, �� �������� ���� ����������� �������� ������� � ������� ������
  };
protected:
  struct TFileDelArray {
    TFileShare::TArrayRDA  arrRec;
    PImageFile pImage;  // ����� - ��������� ��������
    TFileDelArray( int size ): arrRec( NULL, size ) {pImage=NULL;}
  };

  // ����� ������ ��� ����������� ������� ��������� ������� �� �����
  struct TFileChange {
    PUIDobject p;       // ��������� �� ���������� ������
    PImageFile pImage;  // ����� - ��������� ��������
    int   indexImage;   // ������ ������� � ������ - ����������
    int   indexBaseOld; // ������ ������ ������� � ������� ������
    int   indexBaseNew; // ����� ������ ������� � ������� ������
  };

  // �������-���������� ������� �������� �����
  static void fileOpen( PImageFile* ppImage, PArrUIDobject parr );
  // �������-���������� ������� �������� �����
  static void fileClose( PImageFile* ppImage, void* );
  // �������-���������� ������� ����������/������� ������� � �����
  static void fileInsert( PImageFile* ppImage, TFileInsert* def );
  // �������-���������� ������� �������� ������� �� �����
  static void fileDelete( PImageFile* ppImage, TFileDel* def );
  // �������-���������� ������� �������� ������� �������� �� �����
  static void fileDeleteArray( PImageFile* ppImage, TFileDelArray* def );
  // �������-���������� ������� ��������� ������� �� �����
  static void fileChange( PImageFile* ppImage, TFileChange* def );

  // ���������� � ������� ���������� �������
  virtual void doNotifyInsert( TImageFile* imageSrc, int index, PUIDobject p );
  // ���������� � ������� �������� �������
  virtual void doNotifyDelete( TImageFile* imageSrc, int index, PUIDobject p );
  // ���������� � ������� �������� �������
  virtual void doNotifyDeleteArray( TImageFile* imageSrc );
  // ���������� � ������� ��������� �������
  virtual void doNotifyChange( TImageFile* imageSrc, int oldIndex, int index, PUIDobject p );

  PFileShare pFile;     // ��������� �� ������� ������ �����
  friend class TFileShare;

  bool Init( PArrUIDobject parr, PFileShare pfile );
  void Done();

public:

  ///////////////////////////////////////////////////////////////
  // �������, ����� ��� ���� ������� �����������
  //

  TImageFile();
  virtual ~TImageFile();

  // ����� ���������������
  bool IsRegistered();
  void Unregister();

  // ��������� ������ ������� � ����������
  void Config( const char* name, IImageNotify *notify, TDependence _depend, TCmpFunc cmpFunc = NULL );

  // ��������� ������ ����������
  // filterFunc - ������� ����������. ���������� 0 � ������ ���������� ������� �������
  // filterParam - ���������������� �������� �������
  void ConfigFilter( TCmpFunc filterFunc, void* filterParam );

  // ����� ��������� ����������
  void ChangeFilterParam( void* filterParam );

  // ��������� ���� ������� ��������� � ������ ������ (begin/end �������������)
  // reset - �������� ���� ����� �������� 
  bool IsChanged( bool reset = true );

  // �������� ���� ������� ��������� 
  void ResetChanged();

  // ���������� ���� �� ������������� �����-�� ��������� (insert, delete, ... )
  void SelfNotifyChange( bool flag );

  // ������� ����� � ������������� �������
  // �������� ��. � hArray.cpp
  int FastSearch( const void * p, int * ins = NULL, TCmpFunc Cmp = NULL );

  // ����� � ������� ������� � �������� ����������
  int FindIndexUID( guint32 uid );
  PUIDobject FindUID( guint32 uid );  // ����� ��������

  // ����� � ������� ��������� ������� ( ���������������� ��������� ! )
  int FindPtr( void* p );
  int FindPtr( void* p, int start, bool revers = false );

  void  Sort( TCmpFunc Cmp, bool doSort = true );
  
  // �������� ��������������� ������� �� ��������� �������
  bool  IsSortBy( TCmpFunc cmpFunc ) const { return CmpFunc == cmpFunc; }

  // �������� ����� �������
  // uid - ������������� �������
  // r   - �������� ������
  bool GetCopy( guint32 uid, TUIDobject& r );
  
  // �������� �������� �������
  // r - ����� ��������� ������� � ������� uid
  bool Replace( TUIDobject& r );
  
  // ���������� ���������
  // ���������� ������ ��������� begin/end:
  int Count();     // ����������
  int CountBE();   // �� ���������

  // ������ � ������� � ��������� �� ��������� � ��������
  PUIDobject Item( int index );
  PUIDobject operator[]( int index ) { return Item( index ); }

  // ��������� ��������� ��� ������� ������� � ��������� �������
  // �������, �� �� ���������� ������.
  // ��������� ������������ ����� �������� ����������/�������� ��������.
  PUIDobject* baseItem();

  // ������� ���-�� ����� ���������� (������������ ������� a[i-1] <= a[i] <= a[i+1]
  int CountErrSort( int *pError );

  ///////////////////////////////////////////////////////////////
  // -------------�������, ��� ������� <depend_full> � <independ_arr>
  //

  // �������� ������������� ������� � ������.
  // ����������� ��� ������ �� ����� ���� ������������ ���������
  void begin();
  void end();

  // ������� ��� ����������� ����������� ��������� ����������� ��������
  // <cntBlock> � ������� �������� ������� ������������ ������ ����� ������ ��������
  // ����������� �������� TFileShare::SetOwner( )
  #ifdef _DEBUG
  void FALSEbegin() { g_atomic_int_inc( &cntBlock ); }
  void FALSEend()   { BL_ASSERT( g_atomic_int_exchange_and_add( &cntBlock, -1) > 0 ); }
  #else
  void FALSEbegin() {}
  void FALSEend()   {}
  #endif

  // �������� ������
  // p - ����������� ������
  // ���������: ������� ������� �������� � �����
  int Add( PUIDobject p );

  // �������� ������
  // p - ����������� ������
  // index - ������� �������
  // ���������: �����
  bool  Insert( int index, PUIDobject p );

  // ������� ������
  // index - ������ ������� � ������
  // p - ��������� �� ������
  // uid - ������������� �������
  void  Delete( int index );   // ����� �������
  void  Delete( PUIDobject p );
  void  DeleteUID( guint32 uid );  // ����� ���������, ���� ����� �� ������������ �� uid

  // ������� ����� �������� �� �� ���������������
  void  Delete( int cntUID, guint32* arrUID );

  // ��������� ����� �� ��������� �������
  // ��������� ������� ���������� ����� ����������� ��������� �������
  // ���������: ����� ������� ������������������ ������� (����� �������� � �������).
  // index - ������ ������� � ������
  // p - ��������� �� ������
  int CheckChange( int index );    // ����� �������
  int CheckChange( PUIDobject p );

  // ��������� ����� �� ��������� �������, �� �������� ������
  void  WithoutFile_Change( PUIDobject p, int index );

  ///////////////////////////////////////////////////////////////

  #ifdef _DEBUG
  // �������� ������������ ������ ������� ������ ��������� ����������
  bool isCorrectLock()
    { return pArray && (cntBlock > 0); }
  #endif

  // �������� ������������(���������� ������) UID-����� �������� � �������
  // �������������� �������� ���������:
  // *pInd1, *pInd2 - ������� ����������-������
  // ���������: ������������ ��������� �������
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
// ������� ����� � ������������� �������
inline int TImageFile::FastSearch( const void * p, int * ins, TCmpFunc Cmp )
{
  BL_ASSERT( isCorrectLock() );
  return pArray ? pArray->FastSearch( p, ins, Cmp ) : -1;
}
//-----------------------------------------------------------------------------
// ����� � ������� ������� � �������� ���������� ���������������
inline int TImageFile::FindIndexUID( guint32 uid )
{
  return pArray ? pArray->FindIndexUID( uid ) : -1;
}
//-----------------------------------------------------------------------------
// ����� ��������
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
// ���������� ���������
// ���������� ������ ��������� begin/end �� ���������
inline int TImageFile::CountBE()
{
  begin();
  int cnt = pArray->Count();
  end();
  return cnt;
}
//-----------------------------------------------------------------------------
// ���������� ���������
// ���������� ������ ��������� begin/end ����������
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
// ��������� ���� ������� ��������� � ������ ������
inline bool TImageFile::IsChanged( bool reset )
{
  bool res = mHasChange;
  if( reset ) mHasChange = false;
  return res;
}
//-----------------------------------------------------------------------------
// �������� ���� ������� ���������
inline void TImageFile::ResetChanged()
{
  mHasChange = false;
}
//-----------------------------------------------------------------------------
// �������� ������������(���������� ������) UID-����� �������� � �������
inline bool TImageFile::CheckUniqueUID( int *pInd1, int *pInd2 )
{
  BL_ASSERT( isCorrectLock() );
  return pArray->CheckUniqueUID( pInd1, pInd2 );
}
//-----------------------------------------------------------------------------
#endif
