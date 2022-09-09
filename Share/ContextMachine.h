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


#ifndef ContextMachineH
#define ContextMachineH

#include <map>

template <class Key, class Value>
class TContextMachine
{
public:
  typedef std::map< Key, Value > TMapKeyValue;

protected:
  typedef std::multimap< Key, TMapKeyValue > TMultiMapKeyMap;
  TMultiMapKeyMap mMultiMapKeyMap;

public:

  TContextMachine();
  virtual ~TContextMachine();

  // �������� ������� - �������� ������� (������� ����� � �� ����)
  bool Action(Key k, Value& v);

  // ��� �������������
  void AddState(Key k, TMapKeyValue* mKV);
  int GetStateCount() const;

protected:
  void Done();

  int GetCountByKey(Key& k);
  void UpdateMapKeyCount(Key& k, int count);

  typedef std::map<Key, int> TMapKeyInt;

  // ��� �����������
  TMapKeyInt mMapKeyCount; 
  int mCurCount;
  bool flgNeedInitSame;
  typename TMultiMapKeyMap::iterator mBeginSameIt;
  typename TMultiMapKeyMap::iterator mCurSameIt;

  TMapKeyValue* mCurrentMap;
  int mCurSameState;
  Key mSameState;
};
//---------------------------------------------------------------------
//---------------------------------------------------------------------
template <class Key, class Value>
TContextMachine<Key,Value>::TContextMachine()
{
  mCurrentMap = NULL;
  mCurSameState = 0;
  flgNeedInitSame = true;
}
//---------------------------------------------------------------------
template <class Key, class Value>
TContextMachine<Key,Value>::~TContextMachine()
{
  Done();
}
//---------------------------------------------------------------------
template <class Key, class Value>
bool TContextMachine<Key,Value>::Action(Key k, Value& v)
{
  // ���������� ���������
  if(mSameState==k)// �� �� �������� ���������
  {
    if(flgNeedInitSame)
    {
      TMultiMapKeyMap::iterator fit = mMultiMapKeyMap.lower_bound(k);
      if(fit!=mMultiMapKeyMap.end())
      {
        mBeginSameIt = fit;
        mCurCount    = GetCountByKey(k);
        
        mCurSameIt = mBeginSameIt;
      }
      flgNeedInitSame = false;
    }
    //------------------------------------
    mCurSameState++;
    if(mCurSameState==mCurCount)
    {
      mCurSameState = 0;
      mCurSameIt = mBeginSameIt;
    }
    else
      mCurSameIt++;
    
    mCurrentMap = &(mCurSameIt->second);
  }
  else
  {
    TMultiMapKeyMap::iterator fit = mMultiMapKeyMap.lower_bound(k);
    if(fit!=mMultiMapKeyMap.end())
    {
      mSameState      = k;
      mCurSameState   = 0;
      mCurCount       = GetCountByKey(k);
      mBeginSameIt    = fit;
      flgNeedInitSame = false;

      mCurSameIt = mBeginSameIt;
      
      mCurrentMap = &(mCurSameIt->second);
    }
  }
  //------------------------------------------------------------------
  // ����� ��� ���������
  TMapKeyValue::iterator ffit = mCurrentMap->find(k);
  if(ffit!=mCurrentMap->end())
  {
    v = ffit->second;
    return true;
  }
  return false;
}
//---------------------------------------------------------------------
template <class Key, class Value>
void TContextMachine<Key,Value>::AddState(Key k,TMapKeyValue* pMKV)
{
  mMultiMapKeyMap.insert(TMultiMapKeyMap::value_type(k,*pMKV));
  if(mCurrentMap==NULL)
  {
    mCurrentMap = &(mMultiMapKeyMap.begin()->second);
    mSameState  = k;
  }
  UpdateMapKeyCount(k,mMultiMapKeyMap.count(k));
}
//---------------------------------------------------------------------
template <class Key, class Value>
int TContextMachine<Key,Value>::GetStateCount() const
{
  return mMultiMapKeyMap.size();
}
//---------------------------------------------------------------------
template <class Key, class Value>
void TContextMachine<Key,Value>::Done()
{
  mMultiMapKeyMap.clear();
  mMapKeyCount.clear();
  mCurrentMap = NULL;
  mCurSameState = 0;
}
//---------------------------------------------------------------------
template <class Key, class Value>
int TContextMachine<Key,Value>::GetCountByKey(Key& k)
{
  TMapKeyInt::iterator fit = mMapKeyCount.find(k);
  if(fit!=mMapKeyCount.end())
    return fit->second;

  return 0;
}
//---------------------------------------------------------------------
template <class Key, class Value>
void TContextMachine<Key,Value>::UpdateMapKeyCount(Key& k, int count)
{
  TMapKeyInt::iterator fit = mMapKeyCount.find(k);
  if(fit!=mMapKeyCount.end())
    fit->second = count;
  else
    mMapKeyCount.insert(TMapKeyInt::value_type(k,count));
}
//---------------------------------------------------------------------



#endif