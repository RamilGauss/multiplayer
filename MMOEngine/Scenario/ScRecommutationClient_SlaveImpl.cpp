/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ScRecommutationClient_SlaveImpl.h"

using namespace nsMMOEngine;
using namespace nsRecommutationClientStruct;


TScRecommutationClient_SlaveImpl::TScRecommutationClient_SlaveImpl(IScenario* pSc):
TBaseScRecommutationClient(pSc)
{

}
//--------------------------------------------------------------------------------------
void TScRecommutationClient_SlaveImpl::Work(unsigned int time_ms)
{

}
//--------------------------------------------------------------------------------------
void TScRecommutationClient_SlaveImpl::RecvInherit(TDescRecvSession* pDesc)
{
  // защита от хака
  if(pDesc->sizeData < sizeof(THeader))
    return;
  //=======================================
  THeader* pHeader = (THeader*)pDesc->data;
  switch(pHeader->from)
  {
    case eClient:
      RecvFromClient(pDesc);
      break;
    case eMaster:
      RecvFromMaster(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//--------------------------------------------------------------------------------------
void TScRecommutationClient_SlaveImpl::RecvFromMaster(TDescRecvSession* pDesc)
{
  THeader* pHeader = (THeader*)pDesc->data;
  switch(pHeader->subType)
  {
    case eBeginDonor:
      BeginDonor(pDesc);
      break;
    case eInfoRecipientToDonor:
      InfoRecipientToDonor(pDesc);
      break;
    case eBeginRecipient:
      BeginRecipient(pDesc);
      break;
    case eDisconnectClient:
      DisconnectClientToSlave(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//--------------------------------------------------------------------------------------
void TScRecommutationClient_SlaveImpl::RecvFromClient(TDescRecvSession* pDesc)
{
  THeader* pHeader = (THeader*)pDesc->data;
  switch(pHeader->subType)
  {
    case eCheckBeginClient:
      CheckBeginClient(pDesc);
      break;
    case eCheckInfoRecipient:
      CheckInfoRecipient(pDesc);
      break;
    case eRequestConnect:
      RequestConnect(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//--------------------------------------------------------------------------------------
void TScRecommutationClient_SlaveImpl::BeginDonor(TDescRecvSession* pDesc)
{

}
//--------------------------------------------------------------
void TScRecommutationClient_SlaveImpl::InfoRecipientToDonor(TDescRecvSession* pDesc)
{

}
//--------------------------------------------------------------
void TScRecommutationClient_SlaveImpl::BeginRecipient(TDescRecvSession* pDesc)
{

}
//--------------------------------------------------------------
void TScRecommutationClient_SlaveImpl::DisconnectClientToSlave(TDescRecvSession* pDesc)
{

}
//--------------------------------------------------------------
void TScRecommutationClient_SlaveImpl::CheckBeginClient(TDescRecvSession* pDesc)
{

}
//--------------------------------------------------------------
void TScRecommutationClient_SlaveImpl::CheckInfoRecipient(TDescRecvSession* pDesc)
{

}
//--------------------------------------------------------------
void TScRecommutationClient_SlaveImpl::RequestConnect(TDescRecvSession* pDesc)
{

}
//--------------------------------------------------------------
void TScRecommutationClient_SlaveImpl::SaveContext(void* data, int size)
{

}
//--------------------------------------------------------------
