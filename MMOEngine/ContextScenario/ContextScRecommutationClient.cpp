/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ContextScRecommutationClient.h"

using namespace nsMMOEngine;

TContextScRecommutationClient::TContextScRecommutationClient()
{
  mClient = NULL;
}
//------------------------------------------------------------------
TContextScRecommutationClient::~TContextScRecommutationClient()
{

}
//------------------------------------------------------------------
TClient_master* TContextScRecommutationClient::GetClient()
{
  return mClient;
}
//------------------------------------------------------------------
void TContextScRecommutationClient::SetClient(TClient_master* p)
{
  mClient = p;
}
//------------------------------------------------------------------
