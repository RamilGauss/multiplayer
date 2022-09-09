/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
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


#ifndef StateMachineH
#define StateMachineH

#include <map>
#include <string>

template <class Key, class Data>
class TStateMachine
{
public:
  typedef std::map<Key, std::string> TMapKeyStr;
protected:
  struct TState
  {
    Data* pData;
    TMapKeyStr m;
    TState(Data* _pData,TMapKeyStr* _m)
    {
      pData =  _pData;
      if(_m!=NULL)
        m = *_m;
      else
        m.clear();
    }
    ~TState()
    {
      delete pData;
    }
  };

  typedef std::map<std::string, TState*> TMapStrState;

  TMapStrState mMapStrState;

public:

  TStateMachine();
  virtual ~TStateMachine();

  // для инициализации
  void AddState(std::string name, TMapKeyStr* mKS, Data* pData);

  bool Init(const char* name);

  // добавить событие - получить реакцию (реакции может и не быть)
  Data* Action(Key k);

  std::string GetNameCurrent();

  Data* GetDataCurrent();

  int GetStateCount() const;

protected:
  void Done();

  typename TMapStrState::iterator mCurStateIt;
};
//---------------------------------------------------------------------
//---------------------------------------------------------------------
template <class Key, class Data>
TStateMachine<Key,Data>::TStateMachine()
{

}
//---------------------------------------------------------------------
template <class Key, class Data>
TStateMachine<Key,Data>::~TStateMachine()
{
  Done();
}
//---------------------------------------------------------------------
template <class Key, class Data>
bool TStateMachine<Key,Data>::Init(const char* sNewStateName)
{
  TMapStrState::iterator fit = mMapStrState.find(sNewStateName);
  if(fit!=mMapStrState.end())
  {
    mCurStateIt = fit;
    return true;
  }
  return false;
}
//---------------------------------------------------------------------
template <class Key, class Data>
Data* TStateMachine<Key,Data>::Action(Key k)
{
  TMapKeyStr::iterator fit = mCurStateIt->second->m.find(k);
  if(fit!=mCurStateIt->second->m.end())
  {
    if(Init(fit->second.data()))// ищем новое состояние
      return mCurStateIt->second->pData;
  }
  return NULL;
}
//---------------------------------------------------------------------
template <class Key, class Data>
void TStateMachine<Key,Data>::AddState(std::string name, TMapKeyStr* mKS, Data* pData)
{
  TState* pState = new TState(pData,mKS);
  mMapStrState.insert(TMapStrState::value_type(name,pState));
}
//---------------------------------------------------------------------
template <class Key, class Data>
int TStateMachine<Key,Data>::GetStateCount() const
{
  return mMapStrState.size();
}
//---------------------------------------------------------------------
template <class Key, class Data>
void TStateMachine<Key,Data>::Done()
{
  TMapStrState::iterator bit = mMapStrState.begin();
  TMapStrState::iterator eit = mMapStrState.end();
  while(bit!=eit)
  {
    delete bit->second;
    bit++;
  }
  mMapStrState.clear();
}
//---------------------------------------------------------------------
template <class Key, class Data>
std::string TStateMachine<Key,Data>::GetNameCurrent()
{
  return mCurStateIt->first;
}
//---------------------------------------------------------------------
template <class Key, class Data>
Data* TStateMachine<Key,Data>::GetDataCurrent()
{
  return mCurStateIt->second->pData;
}
//---------------------------------------------------------------------

#endif

