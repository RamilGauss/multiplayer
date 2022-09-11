/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "SlavePrivate.h"
#include "Base.h"
#include <boost/foreach.hpp>

using namespace std;
using namespace nsMelissa;

TSlavePrivate::TSlavePrivate()
{
  mID_SessionDown   = INVALID_HANDLE_SESSION;
  mState            = eConnect;
  mCountFreeClient  = 0;
  mCountGroupClient = 0;
}  
//----------------------------------------------------------------------
void TSlavePrivate::SetID_Session(unsigned int v)
{
  mID_SessionDown = v;
  mC.SetID_Session(mID_SessionDown);
}
//----------------------------------------------------------------------
unsigned int TSlavePrivate::GetID_Session()
{
  return mID_SessionDown;
}
//----------------------------------------------------------------------
void TSlavePrivate::SetLoadProcent(unsigned char v)
{
  mLoadProcent = v;
}
//----------------------------------------------------------------------
unsigned char TSlavePrivate::GetLoadProcent()
{
  return mLoadProcent;
}
//----------------------------------------------------------------------
void TSlavePrivate::SendToClient(unsigned int id_client, void* data, int size, bool check )
{

}
//----------------------------------------------------------------------
TSlavePrivate::tState TSlavePrivate::GetState()
{
  return mState;
}
//----------------------------------------------------------------------
void TSlavePrivate::SetState(TSlavePrivate::tState state)
{
  mState = state;
}
//----------------------------------------------------------------------
TContainerContextSc* TSlavePrivate::GetC()
{
  return &mC;
}
//----------------------------------------------------------------------
int TSlavePrivate::GetLoadProcentFreeClient()
{
  return int(float(mLoadProcent*mCountFreeClient)/mSetClientKey.size());
}
//----------------------------------------------------------------------
int TSlavePrivate::GetLoadProcentGroupClient()
{
  return int(float(mLoadProcent*mCountGroupClient)/mSetClientKey.size());
}
//----------------------------------------------------------------------
int TSlavePrivate::CalcCountClientForEmpty()
{
  int allCount = mSetClientKey.size();
  if(allCount==0)
    return eDefCountClient;// нельзя определить ###
  
  BL_ASSERT(mCountGroupClient+mCountFreeClient==allCount);
  
  int maxCount = int(allCount/(mLoadProcent/100.0f));
  return maxCount - allCount;
}
//----------------------------------------------------------------------
void TSlavePrivate::AddClient(list<unsigned int>& l_id_client)
{
  BOOST_FOREACH(unsigned int id, l_id_client)
    mSetClientKey.insert(id);
  mCountFreeClient += l_id_client.size();
}
//----------------------------------------------------------------------
void TSlavePrivate::RemoveClient(list<unsigned int>& l_id_client)
{
  BOOST_FOREACH(unsigned int id, l_id_client)
    mSetClientKey.erase(id);
  
  mCountFreeClient -= l_id_client.size();
}
//----------------------------------------------------------------------
void TSlavePrivate::AddGroupClient(int count)
{
  mCountGroupClient += count;
  mCountFreeClient  -= count;
}
//----------------------------------------------------------------------
void TSlavePrivate::AddFreeClient(int count)
{
  mCountFreeClient  += count;
  mCountGroupClient -= count;
}
//----------------------------------------------------------------------
int TSlavePrivate::GetCountClient(std::list<unsigned int>& l_id_client)
{
  int cnt = 0;
  BOOST_FOREACH( unsigned int id, l_id_client)
    if(mSetClientKey.find(id)!=mSetClientKey.end())
      cnt++;
  return cnt;
}
//----------------------------------------------------------------------
