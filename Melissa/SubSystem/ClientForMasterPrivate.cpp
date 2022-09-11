/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ClientForMasterPrivate.h"

#include <stddef.h>

#include "SlavePrivate.h"

using namespace nsMelissa;

TClientForMasterPrivate::TClientForMasterPrivate()
{
  mSlave   = NULL;
  mGroupID = -1;
  mKey     = -1;
}
//----------------------------------------------------------------
void TClientForMasterPrivate::SetSlave(TSlavePrivate* pSlave)
{
  mSlave = pSlave;
}
//----------------------------------------------------------------
TSlavePrivate* TClientForMasterPrivate::GetSlave()
{
  return mSlave;
}
//----------------------------------------------------------------
void TClientForMasterPrivate::SetGroupID(unsigned int id)
{
  mGroupID = id;
}
//----------------------------------------------------------------
unsigned int TClientForMasterPrivate::GetGroupID()
{
  return mGroupID;
}
//----------------------------------------------------------------
void TClientForMasterPrivate::SetKey(unsigned int k)
{
  mKey = k;
}
//----------------------------------------------------------------
unsigned int TClientForMasterPrivate::GetKey()
{
  return mKey;
}
//----------------------------------------------------------------
void TClientForMasterPrivate::SetState(TClientForMasterPrivate::tState state)
{
  mState = state;
}
//----------------------------------------------------------------
TClientForMasterPrivate::tState TClientForMasterPrivate::GetState()
{
  return mState;
}
//----------------------------------------------------------------
TContainerContextSc* TClientForMasterPrivate::GetC()
{
  return &mC;
}
//----------------------------------------------------------------