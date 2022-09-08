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
