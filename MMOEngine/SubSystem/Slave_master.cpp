/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "Slave_master.h"

#include "Base.h"
#include <boost/foreach.hpp>

using namespace std;
using namespace nsMMOEngine;

TSlave_master::TSlave_master()
{
  mID_SessionDown   = INVALID_HANDLE_SESSION;
  mState            = eConnect;
  mCountFreeClient  = 0;
  mCountGroupClient = 0;
}  
//----------------------------------------------------------------------
void TSlave_master::SetID_Session(unsigned int v)
{
  mID_SessionDown = v;
  mC.SetID_Session(mID_SessionDown);
}
//----------------------------------------------------------------------
unsigned int TSlave_master::GetID_Session()
{
  return mID_SessionDown;
}
//----------------------------------------------------------------------
void TSlave_master::SetLoadProcent(unsigned char v)
{
  mLoadProcent = v;
}
//----------------------------------------------------------------------
unsigned char TSlave_master::GetLoadProcent()
{
  return mLoadProcent;
}
//----------------------------------------------------------------------
TSlave_master::tState TSlave_master::GetState()
{
  return mState;
}
//----------------------------------------------------------------------
void TSlave_master::SetState(TSlave_master::tState state)
{
  mState = state;
}
//----------------------------------------------------------------------
TContainerContextSc* TSlave_master::GetC()
{
  return &mC;
}
//----------------------------------------------------------------------
int TSlave_master::GetLoadProcentFreeClient()
{
  return int(float(mLoadProcent*mCountFreeClient)/mSetClientKey.size());
}
//----------------------------------------------------------------------
int TSlave_master::GetLoadProcentGroupClient()
{
  return int(float(mLoadProcent*mCountGroupClient)/mSetClientKey.size());
}
//----------------------------------------------------------------------
int TSlave_master::CalcCountClientForEmpty()
{
  int allCount = mSetClientKey.size();
  if(allCount==0)
    return eDefCountClient;// нельзя определить ###
  
  BL_ASSERT(mCountGroupClient+mCountFreeClient==allCount);
  
  int maxCount = int(allCount/(mLoadProcent/100.0f));
  return maxCount - allCount;
}
//----------------------------------------------------------------------
void TSlave_master::AddClient(list<unsigned int>& l_id_client)
{
  BOOST_FOREACH(unsigned int id, l_id_client)
    mSetClientKey.insert(id);
  mCountFreeClient += l_id_client.size();
}
//----------------------------------------------------------------------
void TSlave_master::RemoveClient(list<unsigned int>& l_id_client)
{
  BOOST_FOREACH(unsigned int id, l_id_client)
    mSetClientKey.erase(id);
  
  mCountFreeClient -= l_id_client.size();
}
//----------------------------------------------------------------------
void TSlave_master::AddDeltaGroupClient(int count)
{
  mCountGroupClient += count;
  mCountFreeClient  -= count;
}
//----------------------------------------------------------------------
void TSlave_master::AddDeltaFreeClient(int count)
{
  mCountFreeClient  += count;
  mCountGroupClient -= count;
}
//----------------------------------------------------------------------
int TSlave_master::GetCountClient(list<unsigned int>& l_id_client)
{
  int cnt = 0;
  BOOST_FOREACH( unsigned int id, l_id_client)
    if(mSetClientKey.find(id)!=mSetClientKey.end())
      cnt++;
  return cnt;
}
//----------------------------------------------------------------------
void TSlave_master::GetVectorID_client(vector<unsigned int>& vID_client)
{

}
//----------------------------------------------------------------------