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
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef WatchValueH
#define WatchValueH

#include "GCS.h"
#include "List_DualLink.h"
#include <string.h>

//-----------------------------------------------------------------------------
// ���� ������ ������� ����� �������������� � ������� WatchValue
typedef enum
{
  wvt_char    = 0,   // ������ � ��������� Latin1
  wvt_uint8   = 1,  // 
  wvt_short   = 2,  
  wvt_ushort  = 3, 
  wvt_int     = 4,   
  wvt_uint    = 5, 
  
  wvt_count = 6
} EWatchValueType;
//-----------------------------------------------------------------------------
// �������������� (������������) ��������
class TWatchValue : public TDLL_ItemT<TWatchValue>
{
  friend class TWatchValueManager;
public:
  char* mName;        // �������� 
  volatile int mValue;// ��������
  int   mInitValue;   // ��������� �������� (��� ������)

  bool mActive;       // ����������� ��������
  EWatchValueType mType;// ��� ������
  bool mIsHex;        // ������ �����������: 0 - ����������, 1 - �����������������

  // ������� ���������� ������� � ������
  bool operator < ( const TWatchValue& val )
  {
    if( mActive && !val.mActive ) return true;
    if( !mActive && val.mActive ) return false;
    return strcmp( mName, val.mName ) < 0;
  }
public:
  TWatchValue( const char* name, bool visible, EWatchValueType type, bool hex, int value );
  ~TWatchValue(void);
  
  // �������� ���������� � ��������� ��������
  void reset()          { mValue = mInitValue; }

  // ���������/���������� ��������
  void set( int value ) { mValue = value; }
  int  get() const      { return mValue; }

  // ���������������� ��������
  void inc( int delta = 1 ) { mValue += delta; }

  // ���������� ���������
  void setMax( int value ) { if( mValue < value ) mValue = value; }
};
//-----------------------------------------------------------------------------
typedef TDLL_ListT<TWatchValue> TListWatchValue;

class GBASELIB_EI TWatchValueManager 
{
  // ������ ������������������ �������� ��� �����������
  #ifdef _MSC_VER
    # pragma warning(push)
    # pragma warning( disable : 4251 )
      TListWatchValue mList;
    # pragma warning(pop)
  #else
    TListWatchValue mList;
  #endif
  // ���� ��������� ��������� ������ ��������
  // (������������ ��� ������� ������� � ������ ������� push() )
  volatile bool mChangedList;   
  
  GCS mCS;
  void begin() { mCS.lock(); }
  void end()   { mCS.unlock(); }
public:
  TWatchValueManager(void);
  ~TWatchValueManager(void);

  void Done();
public:

  TWatchValue* Register( const char* name, EWatchValueType type = wvt_int, int value = 0, bool hex = false );

  // �������� ����� ����������� ����������
  // item - ���������� 
  // flag - ���� ������������� ����������� 
  void setActive( TWatchValue* item, bool flag );
  
  // ��������� ������ �������� ��� ������������ �������������
  // onlyChanged - �������� ������������� ������ �������� ������ � ������ ��������� ��� ���������
  // list [out] - ��������� �� ����������� ������
  // ���������: ������������ � ������������� ������ ��� ���. 
  // ����.: ���� ������ ������������, �� ����� ���������� ��� ������������� ���������� �������
  //        ������ ���������� �� ������ ������� ������ unlock()
  bool lock( bool onlyChanged, TListWatchValue* &list )
  {
    if( onlyChanged && !mChangedList ) return false;
    begin();
    list = &mList;
    return true;
  }
  
  // ����� ���������� �� ������
  void unlock() 
  { 
    // ������� ���������� ���� ��������� ��������� ������, ������������ ���
    // ���������� ������� ���������� ������ ���������
    mChangedList = false; 
    end(); 
  }
};
//-----------------------------------------------------------------------------
extern GBASELIB_EI TWatchValueManager WatchValueManager;

#endif // WatchValueH
