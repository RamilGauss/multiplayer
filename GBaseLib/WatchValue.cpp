#include "WatchValue.h"
#include "BL_ConfigFile.h"

GBASELIB_EI TWatchValueManager WatchValueManager;

//-----------------------------------------------------------------------------
TWatchValue::TWatchValue( const char* name, bool active, EWatchValueType type, bool hex, int value )
{
  size_t len = strlen( name );
  mName = new char[len+1];
  memcpy( mName, name, len+1 );
  
  mActive = active;
  mType = type;
  mIsHex = hex;

  mInitValue = mValue = value;
}
//-----------------------------------------------------------------------------
TWatchValue::~TWatchValue(void)
{
  delete[] mName;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
TWatchValueManager::TWatchValueManager(void):mCS("WatchValueManager")
{
  mChangedList = false;
}
//-----------------------------------------------------------------------------
TWatchValueManager::~TWatchValueManager(void)
{
  Done();
}
//-----------------------------------------------------------------------------
void TWatchValueManager::Done()
{
  begin();
  mList.deleteAll();
  mChangedList = true;
  end();
}
//-----------------------------------------------------------------------------
TWatchValue* TWatchValueManager::Register( const char* name, EWatchValueType type, int value, bool hex )
{
  begin();
  // �������� �� ��������� �����������
  TWatchValue* p = mList.first();
  while( p )
  {
    if( strcmp( p->mName, name ) == 0 )
    {
      end();
      return p;
    }
    p = p->mNext;
  }
  // ������������� ����������� �������� ���������� 
  bool active = AppConfigFile->GetBool( "WatchValue", name, false );
  p = new TWatchValue( name, active, type, hex, value );

  // �������� ������� � ������ � ������������ � �������� ����������
  mList.insert( p );
  
  // ��������� ������ ��������
  mChangedList = true;
  end();

  return p;
}
//-----------------------------------------------------------------------------
// �������� ����� ����������� ����������
void TWatchValueManager::setActive( TWatchValue* item, bool flag )
{
  begin();
  item->mActive = flag;
  AppConfigFile->SetBool( "WatchValue", item->mName, flag );
  // ��������������� ������� �������� � ������ � ������������ � �������� ����������
  mList.checkSortPos( item );
  mChangedList = true;
  end();
}
//-----------------------------------------------------------------------------
