/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ContextScRecommutationClient.h"
#include "Base.h"

using namespace nsMMOEngine;

TContextScRecommutationClient::TContextScRecommutationClient()
{
  mID_SessionMasterSlave = INVALID_HANDLE_SESSION;
}
//------------------------------------------------------------------
TContextScRecommutationClient::~TContextScRecommutationClient()
{

}
//------------------------------------------------------------------
unsigned int TContextScRecommutationClient::GetID_SessionClientSlave()
{
  return GetID_Session();
}
//------------------------------------------------------------------
void TContextScRecommutationClient::SetID_SessionClientSlave(unsigned int id_session)
{
  SetID_Session(id_session);
}
//------------------------------------------------------------------
unsigned int TContextScRecommutationClient::GetID_SessionMasterSlave()
{
  return mID_SessionMasterSlave;
}
//------------------------------------------------------------------
void TContextScRecommutationClient::SetID_SessionMasterSlave(unsigned int id_session)
{
  mID_SessionMasterSlave = id_session;
}
//------------------------------------------------------------------
