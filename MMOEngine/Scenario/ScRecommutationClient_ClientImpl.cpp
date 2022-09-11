/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ScRecommutationClient_ClientImpl.h"

using namespace nsMMOEngine;
using namespace nsRecommutationClientStruct;

TScRecommutationClient_ClientImpl::TScRecommutationClient_ClientImpl(IScenario* pSc):
TBaseScRecommutationClient(pSc)
{

}
//--------------------------------------------------------------------------------------
void TScRecommutationClient_ClientImpl::Work(unsigned int time_ms)
{

}
//--------------------------------------------------------------------------------------
void TScRecommutationClient_ClientImpl::RecvInherit(TDescRecvSession* pDesc)
{
  THeader* pHeader = (THeader*)pDesc->data;
  switch(pHeader->from)
  {
    case eSlaveDonor:
      RecvFromSlaveDonor(pDesc);
      break;
    case eSlaveRecipient:
      RecvFromSlaveRecipient(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//--------------------------------------------------------------------------------------
void TScRecommutationClient_ClientImpl::RecvFromSlaveDonor(TDescRecvSession* pDesc)
{
  THeader* pHeader = (THeader*)pDesc->data;
  switch(pHeader->subType)
  {
    case eBeginClient:
      BeginClient(pDesc);
      break;
    case eInfoRecipientToClient:
      InfoRecipientToClient(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//--------------------------------------------------------------------------------------
void TScRecommutationClient_ClientImpl::RecvFromSlaveRecipient(TDescRecvSession* pDesc)
{
  THeader* pHeader = (THeader*)pDesc->data;
  switch(pHeader->subType)
  {
    case eRequestConnect:
      RequestConnect(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//--------------------------------------------------------------------------------------
void TScRecommutationClient_ClientImpl::BeginClient(TDescRecvSession* pDesc)
{

}
//--------------------------------------------------------------
void TScRecommutationClient_ClientImpl::InfoRecipientToClient(TDescRecvSession* pDesc)
{

}
//--------------------------------------------------------------
void TScRecommutationClient_ClientImpl::RequestConnect(TDescRecvSession* pDesc)
{

}
//--------------------------------------------------------------
