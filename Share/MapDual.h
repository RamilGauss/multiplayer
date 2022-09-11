/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
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

#ifndef MapDualH
#define MapDualH

#include <map>

// в Boost есть аналог этого класса bimap

template <class Key, class Value>
class TMapDual
{
  typedef std::map<Key,  Value> TMapKeyValue;
  typedef std::map<Value,Key>   TMapValueKey;
  TMapKeyValue mMapKeyValue;
  TMapValueKey mMapValueKey;

public:

  TMapDual(){}
  virtual ~TMapDual(){Clear();}

  // добавление
  void Add(Key& k, Value& v);

  // поиск
  bool GetValueByKey(Key& k, Value& v);
  bool GetKeyByValue(Key& k, Value& v);

  // изменение
  bool SetValueByKey(Key& k, Value& v);
  bool SetKeyByValue(Key& k, Value& v);

  // удаление
  bool RemoveValueByKey(Key& k);
  bool RemoveValue(Value& v);

  // чистка
  void Clear();
protected:
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
template <class Key, class Value>
void TMapDual<Key,Value>::Add(Key& k, Value& v)
{
  mMapKeyValue.insert(TMapKeyValue::value_type(k, v));
  mMapValueKey.insert(TMapValueKey::value_type(v,k));
}
//-------------------------------------------------------------------
template <class Key, class Value>
bool TMapDual<Key,Value>::GetValueByKey(Key& k, Value& v)
{
  TMapKeyValue::iterator fitKey = mMapKeyValue.find(k);
  if(fitKey!=mMapKeyValue.end())
  {
    v = fitKey->second;
    return true;
  }
  return false;
}
//-------------------------------------------------------------------
template <class Key, class Value>
bool TMapDual<Key,Value>::GetKeyByValue(Key& k, Value& v)
{
  TMapValueKey::iterator fitValue = mMapValueKey.find(v);
  if(fitValue!=mMapValueKey.end())
  {
    k = fitValue->second;
    return true;
  }
  return false;
}
//-------------------------------------------------------------------
template <class Key, class Value>
bool TMapDual<Key,Value>::RemoveValueByKey(Key& k)
{
  bool res = false;
  TMapKeyValue::iterator fitKey = mMapKeyValue.find(k);
  if(fitKey!=mMapKeyValue.end())
  {
    TMapValueKey::iterator fitValue = mMapValueKey.find(fitKey->second);
    if(fitValue!=mMapValueKey.end())
    {
      mMapKeyValue.erase(fitKey);
      mMapValueKey.erase(fitValue);
      res = true;
    }
  }
  return res;
}
//-------------------------------------------------------------------
template <class Key, class Value>
bool TMapDual<Key,Value>::RemoveValue(Value& v)
{
  bool res = false;
  TMapValueKey::iterator fitValue = mMapValueKey.find(v);
  if(fitValue!=mMapValueKey.end())
  {
    TMapKeyValue::iterator fitKey = mMapKeyValue.find(fitValue->second);
    if(fitKey!=mMapKeyValue.end())
    {
      mMapKeyValue.erase(fitKey);
      mMapValueKey.erase(fitValue);
      res = true;
    }
  }
  return res;
}
//-------------------------------------------------------------------
template <class Key, class Value>
void TMapDual<Key,Value>::Clear()
{
  mMapKeyValue.clear();
  mMapValueKey.clear();
}
//-------------------------------------------------------------------
template <class Key, class Value>
bool TMapDual<Key,Value>::SetValueByKey(Key& k, Value& v)
{
  TMapKeyValue::iterator fitKey = mMapKeyValue.find(k);
  if(fitKey!=mMapKeyValue.end())
  {
    fitKey->second = v;
    return true;
  }
  return false;
}
//-------------------------------------------------------------------
template <class Key, class Value>
bool TMapDual<Key,Value>::SetKeyByValue(Key& k, Value& v)
{
  TMapValueKey::iterator fitValue = mMapValueKey.find(v);
  if(fitValue!=mMapValueKey.end())
  {
    fitValue->second = k;
    return true;
  }
  return false;
}
//-------------------------------------------------------------------


#endif
