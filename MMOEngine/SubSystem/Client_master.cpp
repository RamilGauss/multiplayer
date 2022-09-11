/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "Client_master.h"

#include <stddef.h>

#include "Slave_master.h"

using namespace nsMMOEngine;

TClient_master::TClient_master()
{
  mSlave   = NULL;
  mGroupID = -1;
  mKey     = -1;
}
//----------------------------------------------------------------
void TClient_master::SetSlave(TSlave_master* pSlave)
{
  mSlave = pSlave;
}
//----------------------------------------------------------------
TSlave_master* TClient_master::GetSlave()
{
  return mSlave;
}
//----------------------------------------------------------------
void TClient_master::SetGroupID(unsigned int id)
{
  mGroupID = id;
}
//----------------------------------------------------------------
unsigned int TClient_master::GetGroupID()
{
  return mGroupID;
}
//----------------------------------------------------------------
void TClient_master::SetKey(unsigned int k)
{
  mKey = k;
}
//----------------------------------------------------------------
unsigned int TClient_master::GetKey()
{
  return mKey;
}
//----------------------------------------------------------------
void TClient_master::SetState(TClient_master::tState state)
{
  mState = state;
}
//----------------------------------------------------------------
TClient_master::tState TClient_master::GetState()
{
  return mState;
}
//----------------------------------------------------------------
TContainerContextSc* TClient_master::GetC()
{
  return &mC;
}
//----------------------------------------------------------------