/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "GroupClientPrivate.h"


using namespace std;
using namespace nsMelissa;

TGroupClientPrivate::TGroupClientPrivate()
{
  mState = eConnect;
}
//------------------------------------------------------------------------------------------
void TGroupClientPrivate::SetID(unsigned int id)
{
  mID = id;
}
//------------------------------------------------------------------------------------------
unsigned int TGroupClientPrivate::GetID()
{
  return mID;
}
//------------------------------------------------------------------------------------------
void TGroupClientPrivate::SetClient(set<TClientForMasterPrivate*>& setClient)
{
  mSetClient = setClient;
}
//------------------------------------------------------------------------------------------
set<TClientForMasterPrivate*>& TGroupClientPrivate::GetClient()
{
  return mSetClient;
}
//------------------------------------------------------------------------------------------
void TGroupClientPrivate::SetState(TGroupClientPrivate::tState state)
{
  mState = state;
}
//------------------------------------------------------------------------------------------
TGroupClientPrivate::tState TGroupClientPrivate::GetState()
{
  return mState;
}
//------------------------------------------------------------------------------------------
