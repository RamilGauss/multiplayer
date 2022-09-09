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
  TArrayObject  arrImage;   // ������ ������������������ ������� �����
  TArrUIDobject arr;        // ������ ����������� ������

  //.......................................................
  //       ������� ������������� ����� � ��������
  //.......................................................
protected:
  // �������� �����, ��� �������� �������� ������������� �� ��������� � ��������� ������� �����
  // ������ "��������" � ������ ������ ������������� ��������� � ���� � ����� ����
  // ���� ������ �� "��������" ��������.
  TFileShare*     mParentFile;
  // ���� ����� � ������ ������������� �����
  int             numToParent;

  // �������� ������ "��������" ������
  PFileShare*     arrChildFiles;
  int             cntChildFiles;

  // �������� ������������� ����� � ���� ��� � ���
  void SetParent( PFileShare _mParentFile, int num )
    {
      mParentFile = _mParentFile;
      numToParent = num;
    }
public:
  // �������� ������ "��������" ������
  void SetChilds( PFileShare* childFiles, int cntChilds );
  //.......................................................
protected:
  //............................................................................
  // �������� ������ �� ������������������ �������.
  // ��� ������� ������� ����� ������������� ����� � ��� ��������������� ����� ("��������").
  // ����� ������� ������� "���������" - ������ ������������������ ��� ����� ����������.
  //............................................................................
  struct TRecLock
  {
    PFileShare  pFile;
    bool        exclusive;// ����������� ������������� ����, ��� ��� ������������
  };
  enum { l_MaxCount = 10 };
  TRecLock  arrLock[l_MaxCount];  // ������ "���������", �� ������� ���� ������
  int cntLock;                    // � �� ���-��

  const char* ownerLock;          // �������� ����������
public:

  // �������� ��������������� ���� � ������ ������
  void PushFirstLock( PFileShare pFile, bool exclusive = false );
  // ������� �� ������ ��������������� ����
  void DelLock( PFileShare pFile );
  // �������� ������ ��������������� ������
  void ClearLock();

  void Begin( const char* _ownerLock ); // ������ �����
  void End();   // ������������ �����
  //............................................................................

public:
  TFileShare( const char* _name );

  virtual void OnAfterOpen();
  virtual void Close();
  virtual void Done();
  virtual ~TFileShare() { Done(); }

  // ��������� ������� ���������� ������� ����������� ������
  void  Sort( TCmpFunc Cmp, bool doSort = true )  { arr.Sort( Cmp, doSort ); }

  // ���������������� ������ ������ ������ ������������ ������ ������
  bool  RegisterImage( TImageFile* pView );
  // ����� �����������
  void  UnregisterImage( TImageFile* pView );

  // ������������� ��������� ��� ������������������ ������
  // ������������� ��� �������� �������������� ���������� ������.
  // ������� �������������� ������ ������������������ ������� �� �������������!
  void  UnlinkAllImages()    { arrImage.Unlink(); }
  //.................................................................
  //         �������� ��� ������� ������ � ����������� � ����
  //.................................................................
  // �������� ������ � ����
  // index  - ������� ������� � ������ ������, ���������� ���������� ���
  //          -1, ���� ������� ������� ��� ���� ����������
  // p      - ����������� ������
  // pImage - �����, ��������� ���������� (��� NULL)
  bool Insert( int index, PUIDobject p, TImageFile* pImage );

  // ������� ������ �� �����
  // index  - ������� �������� � ������� ������, ���������� �������� (��� -1)
  // p      - ��������� ������
  // pImage - �����, ��������� �������� (��� NULL)
  void Delete( PUIDobject p, int index, TImageFile* pImage );

  // ������� ������ �� ����� 
  // uid    - uid ���������� �������
  // pImage - �����, ��������� �������� (��� NULL)
  void  DeleteUID( guint32 uid, TImageFile* pImage );

  // ������� ����� �������� �� �� ���������������
  // cntDel - ���-�� ��������
  // arrDel - ������ ���������������
  // pImage - �����, ��������� �������� (��� NULL)
  virtual void DeleteArray( int cntDel, guint32* arrDel, TImageFile* pImage );

  // ����������� �� ������ ������, ��� ��� �������� ������� ���� �������
  // p - ���������� ������
  // index - ������ ������� � ������
  // pImage - �����, ��������� ��������� (��� NULL)
  bool CheckChange( PUIDobject p, int index, TImageFile* pImage );

protected:
  bool AppendFromLoad( PUIDobject p );
  //.................................................................
  //        �������� �� ������� ������ ��� ���������� � ����
  //.................................................................
  bool WithoutFile_Add( PUIDobject p );
  void WithoutFile_DeleteUID( guint32 uid, bool notifyParent );
  void WithoutFile_Delete( int index, bool notifyParent );
  void WithoutFile_Change( PUIDobject p, int index, TImageFile* pImage, guint32 id_parent = UID_NULL );
  void WithoutFile_Change( guint32 uid, TImageFile* pImage = NULL );
  //.................................................................

  //.................................................................
  //     �������� � ��������� ���������� �������� ������ ������
  //.................................................................
  // ���������� ��������� ������� � �������� �������
  // uid - ������������� �������� ������� (��������)
  // pChild - �������� �������
  // type - ��� ��������� ��������
  // ���������: �������� ������ � ����� ���������
  bool AddChild        ( guint32 uid, PUIDobject pChild, int type );
  bool AddChildFromLoad( guint32 uid, PUIDobject pChild, int type );

  // ��������� ��������� ������� �������� �������
  // uid - ������������� �������� ������� (��������)
  // pChild - �������� �������
  // type - ��� ��������� ��������
  virtual void ChangeChild( guint32 uid, PUIDobject pChild, int type );

  // �������� ��������� ������� �� �������� �������
  // uid - ������������� �������� ������� (��������)
  // uidChild - �������� �������
  // type - ��� ��������� ��������
  void DelChild( guint32 uid, guint32 uidChild, int type );
  //.................................................................

  // ��������� ������ � ����� � ��������� ��� uid
  virtual bool SaveObject( void* p ) = 0;
  // ������� ������ �� �����
  virtual bool DeleteObject( void* p ) = 0;

public:
  // ����� ������ ��� ����������� ������� �������� ������� �������� �� �����
  struct TRecDelArray {
    PUIDobject p;       // ��������� ������
    int indexBase;      // ������, �� �������� ���� ����������� �������� ������� � ������� ������
    // ������� ���������� �� ����������� �������� ���� indexBase
    static int SortIndexBaseUp( const void* p1, const void* p2 );
    // ������� ���������� �� ����������� �������� ���� p->uid
    static int SortPUID( const void* p1, const void* p2 );
    // ������� ������ �������� ���� p->uid
    static int FindPUID( const void* p1, const void* p2 );
  };
  typedef TArrayVAR<TRecDelArray> TArrayRDA;

  // ������� ����� �������� �� �����
  // arrDel ������������ �� ����������� �������� ���� indexBase
  virtual void DeleteObjects( TArrayRDA& arrDel );

  // ��������� � �� ���������, ������������� �� ���� ������
  //virtual bool Apply( PArrUIDobject pArr, PImageFile pImage ) = 0;

  //.................................................................
  //....           ��� ���������� �������������    ..................
  //....        �������� ������ ����������� ������   ................
  //.................................................................
public:
  // ���-�� �������� � �����
  // ���������� ������ ��������� begin/end:
  int Count() const;  // ����������
  int CountBE();      // �� ���������

  // ������ ����������� ������.
  const TArrUIDobject& getArray() const { return arr; }

  // ����� � ������� ������� � �������� ����������
  int FindIndexUID( guint32 uid ) const;
  PUIDobject FindUID( guint32 uid ) const;  // ����� ��������

  // ������� ����� � ������������� �������
  int FastSearch( const void* p, int * ins, TCmpFunc Cmp ) const;
  // ���������������� ����� � �������
  int StepSearch( const void* p, TCmpFunc Cmp ) const;

  // �������� ������� �������
  PUIDobject Item( int index ) { return arr[index]; }
};
//=============================================================================
//=============================================================================
//=============================================================================
// ����� � ������� ������� � �������� ���������� ���������������
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
// ������� ����� � ������������� �������
inline int TFileShare::FastSearch( const void* p, int* ins, TCmpFunc Cmp ) const
{
  return arr.FastSearch( p, ins, Cmp );
}
//-----------------------------------------------------------------------------
// ���������������� ����� � �������
inline int TFileShare::StepSearch( const void* p, TCmpFunc Cmp ) const
{
  return arr.StepSearch( p, Cmp );
}
//-----------------------------------------------------------------------------
// ���-�� �������� � �����
inline int TFileShare::Count() const
{
  return arr.Count();
}
//-----------------------------------------------------------------------------
// ���-�� �������� � �����
inline int TFileShare::CountBE()
{
  Begin( dbgName() );
  int cnt = arr.Count();
  End();
  return cnt;
}
//-----------------------------------------------------------------------------
// ������� ������ �� ����� 
inline void TFileShare::DeleteUID( guint32 uid, TImageFile* pImage )
{
  int index = FindIndexUID( uid );
  PUIDobject p = Item( index );
  if( p )
    Delete( p, index, pImage );
}
//-----------------------------------------------------------------------------
#endif
