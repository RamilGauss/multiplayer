/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef StateMachineH
#define StateMachineH

#include <map>
#include <string>
#include <boost/foreach.hpp>

/*
  ������ ����������� ������� "���- ����� ����-��� ���������" � �������
  �� ���� �������� �������� � ����� ��������� ����� ������� � ������ �� �����,
  ���� � ������ ������ � ������ ���������.


  ���1 - ����� {����2-���2, ����3-���3}
  ���2 - ����� {����1-���1}
  ���3 - ����� {����1-���1, ����2-���2}

  �������� ������� ��������� ���1, � ����� � Action(����2), ������� ��������� �������� �� ���2 � 
  ������� ������ ��� ���2.
*/

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

  // ��� �������������
  // mKS - �������� ���������(�����), � ������� ����� ������� �� �����
  void AddState(std::string name, TMapKeyStr* mKS, Data* pData);
  // ��������� ��������� �� �����
  bool Init(const char* sNewStateName);

  // �������� ������� - �������� ������� (������� ����� � �� ����)
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
    if(Init(fit->second.data()))// ���� ����� ���������
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
  BOOST_FOREACH(TMapStrState::value_type& bit,mMapStrState)
    delete bit.second;
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

