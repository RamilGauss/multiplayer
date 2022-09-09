/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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
  // ���� ��������� ��������� ������
  enum {
    inactive = (1<<7),  // ��������� (������� �� ������)
    database = (1<<6),  // ������ ���������� � ��
    changed  = (1<<5),  // ��������
    value    = 0x1F     // ����� �������� ����������� �����
  };
  guint8 state;

public:
  TStateRecord()
  {
    state = 0; // ��������, � �� �� ����������, �� ��������
  }

  enum {
    tsNONE,   // ������ �� ����
    tsINSERT, // INSERT
    tsUPDATE, // UPDATE
    tsDELETE  // DELETE
  };
  // ��� ����������� �������� ��� �������������� � ��
  // ���������: ��. tsXXX
  int TypeSynchro();

  // �������� ��� ����������� �� ��
  void SetLoaded()  { state = database; }

  // �������� ������ ��� ������������������ � ��
  void MarkAsSyncro()
  {
    // ���� ��������� - ������ �� ���������� � ��
    if( state & inactive ) state &= ~database;
    // ����� ��������� � ����
    else state = (guint8)( (state | database) & ~changed );
  }

  // �������� - ����������������� �� ������ � ��
  bool IsSyncro();

  // �������� ������ ��� ����������� � ��
  void MarkSaved()    { state |= database; }

  //
  // ������� ���������� ���������� �������
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
#define UID_NULL    ((guint32)-1)  // ���������������� ��������
#define UID_ANY     ((guint32)-2)  // ����� ��������

class GBASELIB_EI TUIDobject : public TObject
{
  typedef TObject inherited;
protected:
  TStateRecord  state;
public:
  guint32 uid;        // ������������� �������, �������� � ����� �� ����� ����� ������������� �����

  TUIDobject()  { uid = UID_NULL; }

  //
  // ������� ���������� ���������� �������
  //
  void MarkDeleted()  { state.MarkDeleted(); }
  bool IsDeleted()    { return state.IsDeleted(); }

  void MarkChanged()  { state.MarkChanged(); }
  bool IsChanged()    { return state.IsChanged(); }

  void SetAsNew()     { state.SetAsNew(); }
  bool InDatabase()   { return state.InDatabase(); }

  // ��� ����������� �������� ��� �������������� � ��
  // ���������: ��. TStateRecord::tsXXX
  int TypeSynchro()   { return state.TypeSynchro(); }

  // �������� ������ ��� ������������������ � ��
  virtual void MarkAsSyncro()    { state.MarkAsSyncro(); }
  void SetAsSynchro() { state.SetAsSynchro(); }
  void MarkSaved()    { state.MarkSaved(); }
  // �������� - ����������������� �� ������ � ��
  bool IsSyncro() { return state.IsSyncro(); }

  //.........................................................
  //   �������� � �������� �������� ������� ������
  //.........................................................
  // �������� ������������� ��������
  virtual guint32 getUIDParent() const  { return UID_NULL; }
  virtual void setUIDParent( guint32 )  { }

  // ���������� ��������� ��������
  // PUIDobject - ��������� �� �������� ������
  // int - ����� ���� �����
  virtual void AddChild( PUIDobject, int ) {}

  // �������� ��������� ��������
  // guint32 - ������������� ��������� �������
  // int - ����� ���� �����
  virtual void DelChild( guint32, int ) {}

  // ���-�� �������� ���������
  // int - ����� ���� �����
  virtual int CountChild( int ) { return 0; }

  // �������� ������������� ��������� ��������
  // index - ���������� ����� � ������
  // int - ����� ���� �����
  virtual guint32 GetUIDChild( int /*index*/, int ) { return UID_NULL; }

  // ��������� ��� �������� �������� ��� ��������
  // int - ����� ���� ����� (-1 - �������� �������� ���� �����)
  virtual void UnlinkChild( int ) {  }
  //.........................................................
  //.........................................................

  //
  // ������������ ���������� �� Object
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
  // ��������� uid
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
