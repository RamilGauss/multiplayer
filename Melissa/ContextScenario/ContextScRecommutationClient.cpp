/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ContextScRecommutationClient.h"

using namespace nsMelissa;

TContextScRecommutationClient::TContextScRecommutationClient()
{
  mClient = NULL;
}
//------------------------------------------------------------------
TContextScRecommutationClient::~TContextScRecommutationClient()
{

}
//------------------------------------------------------------------
TClientForMasterPrivate* TContextScRecommutationClient::GetClient()
{
  return mClient;
}
//------------------------------------------------------------------
void TContextScRecommutationClient::SetClient(TClientForMasterPrivate* p)
{
  mClient = p;
}
//------------------------------------------------------------------
