/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ScRecommutationClient_MasterImpl.h"
#include "ContextScRecommutationClient.h"

using namespace nsMMOEngine;

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

}
//--------------------------------------------------------------------------------------
void TScRecommutationClient_MasterImpl::RecvFromSlaveDonor(TDescRecvSession* pDesc)
{

}
//--------------------------------------------------------------------------------------
void TScRecommutationClient_MasterImpl::RecvFromSlaveRecipient(TDescRecvSession* pDesc)
{

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
	//NeedSlaveSessionDonor(Context()->GetClientKey());
	
}
//--------------------------------------------------------------
