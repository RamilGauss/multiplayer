/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
√удаков –амиль —ергеевич 
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
// “ипы данных которые можно контролировать в системе WatchValue
typedef enum
{
  wvt_char    = 0,   // символ в кодировке Latin1
  wvt_uint8   = 1,  // 
  wvt_short   = 2,  
  wvt_ushort  = 3, 
  wvt_int     = 4,   
  wvt_uint    = 5, 
  
  wvt_count = 6
} EWatchValueType;
//-----------------------------------------------------------------------------
//  онтролируемый (отображаемый) параметр
class TWatchValue : public TDLL_ItemT<TWatchValue>
{
  friend class TWatchValueManager;
public:
  char* mName;        // название 
  volatile int mValue;// значение
  int   mInitValue;   // начальное значение (дл€ сброса)

  bool mActive;       // отображение значени€
  EWatchValueType mType;// “ип данных
  bool mIsHex;        // формат отображени€: 0 - дес€тичный, 1 - шестнадцатеричный

  // ѕор€док сортировки записей в списке
  bool operator < ( const TWatchValue& val )
  {
    if( mActive && !val.mActive ) return true;
    if( !mActive && val.mActive ) return false;
    return strcmp( mName, val.mName ) < 0;
  }
public:
  TWatchValue( const char* name, bool visible, EWatchValueType type, bool hex, int value );
  ~TWatchValue(void);
  
  // сбросить переменную в начальное значение
  void reset()          { mValue = mInitValue; }

  // ѕрочитать/установить значение
  void set( int value ) { mValue = value; }
  int  get() const      { return mValue; }

  // »нкрементировать значение
  void inc( int delta = 1 ) { mValue += delta; }

  // Ќакопление максимума
  void setMax( int value ) { if( mValue < value ) mValue = value; }
};
//-----------------------------------------------------------------------------
typedef TDLL_ListT<TWatchValue> TListWatchValue;

class GBASELIB_EI TWatchValueManager 
{
  // —писок зарегистрированных значений дл€ отображени€
  #ifdef _MSC_VER
    # pragma warning(push)
    # pragma warning( disable : 4251 )
      TListWatchValue mList;
    # pragma warning(pop)
  #else
    TListWatchValue mList;
  #endif
  // ‘лаг изменени€ структуры списка значений
  // (—брасываетс€ при запросе доступа к списку методом push() )
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

  // »зменить режим отображени€ переменной
  // item - переменна€ 
  // flag - флаг необходимости отображени€ 
  void setActive( TWatchValue* item, bool flag );
  
  // «апросить список значений дл€ монопольного использовани€
  // onlyChanged - указание предоставл€ть список значений только в случае изменени€ его структуры
  // list [out] - указатель на запрошенный список
  // –езультат: предоставлен в использование список или нет. 
  // ѕрим.: ≈сли список предоставлен, то после завершени€ его использовани€ необходимо указать
  //        сн€тие блокировки со списка вызовом метода unlock()
  bool lock( bool onlyChanged, TListWatchValue* &list )
  {
    if( onlyChanged && !mChangedList ) return false;
    begin();
    list = &mList;
    return true;
  }
  
  // —н€ть блокировку со списка
  void unlock() 
  { 
    // ѕопутно сбрасываем флаг изменени€ структуры списка, подразумева€ что
    // вызывающа€ сторона обработала данные изменени€
    mChangedList = false; 
    end(); 
  }
};
//-----------------------------------------------------------------------------
extern GBASELIB_EI TWatchValueManager WatchValueManager;

#endif // WatchValueH
