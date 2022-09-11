/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
√удаков –амиль —ергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ScLoginClient_MasterImpl.h"

using namespace nsMelissa;
using namespace nsLoginClientStruct;

TScLoginClient_MasterImpl::TScLoginClient_MasterImpl(IScenario* pSc):
TBaseScLoginClient(pSc)
{

}
//-----------------------------------------------------------------------------
void TScLoginClient_MasterImpl::RecvInherit(TDescRecvSession* pDesc)
{

}
//-----------------------------------------------------------------------------
void TScLoginClient_MasterImpl::Work(unsigned int time_ms)
{

}
//-----------------------------------------------------------------------------
void TScLoginClient_MasterImpl::Reject(void* resForClient, int sizeResClient)
{
  //Context()->Reject();

  //TBreakPacket bp;
  //bp.PushFront((char*)resForClient, sizeResClient);
  //THeaderRejectM2C h;
  //h.sizeResClient = sizeResClient;
  //bp.PushFront((char*)&h, sizeof(h));
  //Context()->GetMS()->Send(GetID_SessionClientMaster(), bp);
  //End();
}
//--------------------------------------------------------------
void TScLoginClient_MasterImpl::Accept(unsigned int key, void* resForClient, int sizeResClient, 
                                  unsigned int id_session_slave)
{
  //SetID_SessionMasterSlave(id_session_slave);
  //Context()->Accept();

  //TBreakPacket bp;
  //bp.PushFront((char*)resForClient, sizeResClient);
  //THeaderAcceptM2S h;
  //h.key_client = key;
  //h.sizeResClient = sizeResClient;
  //bp.PushFront((char*)&h, sizeof(h));
  //Context()->GetMS()->Send(GetID_SessionMasterSlave(), bp);
}
//--------------------------------------------------------------
void TScLoginClient_MasterImpl::Queue(int num)
{
  //Context()->SetNumInQueue(num);

  //TBreakPacket bp;
  //THeaderQueueM2C h;
  //h.numInQueue = num;
  //bp.PushFront((char*)&h, sizeof(h));
  //Context()->GetMS()->Send(GetID_SessionClientMaster(), bp);
}
//--------------------------------------------------------------
void TScLoginClient_MasterImpl::NeedNumInQueue()
{
  mCallBackNeedNumInQueue.Notify();
}
//---------------------------------------------------------------------
//void TScenarioLoginClient::TryLoginC2M(TDescRecvSession* pDesc)
//{
  //if(Begin()==false)
  //{
  //  // генераци€ ошибки
  //  GetLogger(STR_NAME_MELISSA)->
  //    WriteF_time("TScenarioLoginClient::TryLoginC2M() scenario is not active.\n");

  //  BL_FIX_BUG();
  //  return;
  //}
  //SetTimeWaitForNow();
  //// нова€ сесси€, сохранить
  //SetID_SessionClientMaster(pDesc->id_session);
  //// в буфере, который передали, содержитс€ заголовок и блок
  //// размер блока прописан в заголовке
  //THeaderTryLoginC2M* pPacket = (THeaderTryLoginC2M*)pDesc->data;
  //char* data = pDesc->data + sizeof(THeaderTryLoginC2M);
  //// генераци€ событи€ о попытке авторизации
  //TEventTryLogin event;
  //event.id_session = GetID_SessionClientMaster();
  //event.c.SetData(data,pPacket->sizeData);
  //Context()->GetSE()->AddEventCopy(&event,sizeof(event));
//}
//--------------------------------------------------------------
