/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ScRecommutationClient_MasterImpl.h"
#include "ContextScRecommutationClient.h"
#include "ManagerSession.h"

using namespace nsMMOEngine;
using namespace nsRecommutationClientStruct;

TScRecommutationClient_MasterImpl::TScRecommutationClient_MasterImpl(IScenario* pSc):
TBaseScRecommutationClient(pSc)
{

}
//--------------------------------------------------------------------------------------
void TScRecommutationClient_MasterImpl::Work(unsigned int time_ms)
{

}
//--------------------------------------------------------------------------------------
void TScRecommutationClient_MasterImpl::RecvInherit(TDescRecvSession* pDesc)
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
void TScRecommutationClient_MasterImpl::RecvFromSlaveDonor(TDescRecvSession* pDesc)
{
  THeader* pHeader = (THeader*)pDesc->data;
  switch(pHeader->subType)
  {
    case eCheckBeginDonor:
      CheckBeginDonor(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//--------------------------------------------------------------------------------------
void TScRecommutationClient_MasterImpl::RecvFromSlaveRecipient(TDescRecvSession* pDesc)
{
  THeader* pHeader = (THeader*)pDesc->data;
  switch(pHeader->subType)
  {
    case eCheckBeginRecipient:
      CheckBeginRecipient(pDesc);
      break;
    case eClientConnect:
      ClientConnect(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//--------------------------------------------------------------------------------------
void TScRecommutationClient_MasterImpl::Start(unsigned int id_session_recipient,
                                              unsigned int id_client)
{
  Context()->SetClientKey(id_client);
	Context()->SetSessionRecipient(id_session_recipient);
	if(Begin()==false)
	{
		return;
	}
	SendFirstPacket();
}
//--------------------------------------------------------------
void TScRecommutationClient_MasterImpl::DelayBegin()
{
	SendFirstPacket();
}
//--------------------------------------------------------------
void TScRecommutationClient_MasterImpl::SendFirstPacket()
{
	// откуда уходим?
	NeedSessionDonorByClientKey(Context()->GetClientKey());
  // проверка на совпадение	Донора с Реципиентом
  if(Context()->GetSessionDonor()==Context()->GetSessionRecipient())
  {
    End();
    return;
  }
  
  TBreakPacket bp;
  THeaderBeginDonor h;
  h.id_client = Context()->GetClientKey();
  bp.PushFront((char*)&h, sizeof(h));
  Context()->GetMS()->Send(Context()->GetSessionDonor(), bp);
}
//--------------------------------------------------------------
void TScRecommutationClient_MasterImpl::CheckBeginDonor(TDescRecvSession* pDesc)
{

}
//--------------------------------------------------------------
void TScRecommutationClient_MasterImpl::CheckBeginRecipient(TDescRecvSession* pDesc)
{

}
//--------------------------------------------------------------
void TScRecommutationClient_MasterImpl::ClientConnect(TDescRecvSession* pDesc)
{

}
//--------------------------------------------------------------
void TScRecommutationClient_MasterImpl::DisconnectClient()
{

}
//--------------------------------------------------------------

