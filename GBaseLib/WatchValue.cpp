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
  // Проверка на повторную регистрацию
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
  // необходимость отображения значения переменной 
  bool active = AppConfigFile->GetBool( "WatchValue", name, false );
  p = new TWatchValue( name, active, type, hex, value );

  // Добавить элемент в списка в соответствии с порядком сортировки
  mList.insert( p );
  
  // структура списка изменена
  mChangedList = true;
  end();

  return p;
}
//-----------------------------------------------------------------------------
// Изменить режим обнаружения переменной
void TWatchValueManager::setActive( TWatchValue* item, bool flag )
{
  begin();
  item->mActive = flag;
  AppConfigFile->SetBool( "WatchValue", item->mName, flag );
  // Скорректировать позицию элемента в списке в соответствии с порядком сортировки
  mList.checkSortPos( item );
  mChangedList = true;
  end();
}
//-----------------------------------------------------------------------------
