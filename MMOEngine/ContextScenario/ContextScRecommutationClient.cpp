/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ContextScRecommutationClient.h"
#include "Base.h"

using namespace nsMMOEngine;

TContextScRecommutationClient::TContextScRecommutationClient()
{
  mClientKey        = -1;
  mSessionDonor     = INVALID_HANDLE_SESSION;
  mSessionRecipient = INVALID_HANDLE_SESSION;

  mID_SessionMasterSlave = INVALID_HANDLE_SESSION;

	mTypeSlave = eUndef;
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
void TContextScRecommutationClient::SetClientKey(unsigned int v)
{
  mClientKey = v;
}
//------------------------------------------------------------------
unsigned int TContextScRecommutationClient::GetClientKey()
{
  return mClientKey;
}
//------------------------------------------------------------------
void TContextScRecommutationClient::SetSessionDonor(unsigned int v)
{
  mSessionDonor = v;
}
//------------------------------------------------------------------
unsigned int TContextScRecommutationClient::GetSessionDonor()
{
  return mSessionDonor;
}
//------------------------------------------------------------------
void TContextScRecommutationClient::SetSessionRecipient(unsigned int v)
{
  mSessionRecipient = v;
}
//------------------------------------------------------------------
unsigned int TContextScRecommutationClient::GetSessionRecipient()
{
  return mSessionRecipient;
}
//------------------------------------------------------------------
void TContextScRecommutationClient::SaveContextData(char* p, int size)
{
  mContextData.SetData(p,size);
}
//------------------------------------------------------------------
char* TContextScRecommutationClient::GetContextDataPtr()
{
  return (char*)mContextData.GetPtr();
}
//------------------------------------------------------------------
int TContextScRecommutationClient::GetContextDataSize()
{
  return mContextData.GetSize();
}
//------------------------------------------------------------------
void TContextScRecommutationClient::CleanContextData()
{
  mContextData.Done();
}
//------------------------------------------------------------------
void TContextScRecommutationClient::SetDonor()
{
	mTypeSlave = eDonor;
}
//------------------------------------------------------------------
bool TContextScRecommutationClient::IsDonor()
{
	return (mTypeSlave==eDonor);
}
//------------------------------------------------------------------
void TContextScRecommutationClient::SetRecipient()
{
	mTypeSlave = eRecipient;
}
//------------------------------------------------------------------
bool TContextScRecommutationClient::IsRecipient()
{
	return (mTypeSlave==eRecipient);
}
//------------------------------------------------------------------
