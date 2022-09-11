/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "GroupClient_master.h"

using namespace std;
using namespace nsMMOEngine;

TGroupClient_master::TGroupClient_master()
{
  mState = eConnect;
}
//------------------------------------------------------------------------------------------
void TGroupClient_master::SetID(unsigned int id)
{
  mID = id;
}
//------------------------------------------------------------------------------------------
unsigned int TGroupClient_master::GetID()
{
  return mID;
}
//------------------------------------------------------------------------------------------
void TGroupClient_master::SetClient(set<TClient_master*>& setClient)
{
  mSetClient = setClient;
}
//------------------------------------------------------------------------------------------
set<TClient_master*>& TGroupClient_master::GetClient()
{
  return mSetClient;
}
//------------------------------------------------------------------------------------------
void TGroupClient_master::SetState(TGroupClient_master::tState state)
{
  mState = state;
}
//------------------------------------------------------------------------------------------
TGroupClient_master::tState TGroupClient_master::GetState()
{
  return mState;
}
//------------------------------------------------------------------------------------------
