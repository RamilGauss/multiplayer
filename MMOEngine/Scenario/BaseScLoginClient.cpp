/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "BaseScLoginClient.h"
#include "ContextScLoginClient.h"

using namespace nsMMOEngine;
using namespace nsLoginClientStruct;


//--------------------------------------------------------------
TBaseScLoginClient::TBaseScLoginClient(IScenario* pSc)
{
  mScenario = pSc;
}
//--------------------------------------------------------------
TBaseScLoginClient::~TBaseScLoginClient()
{

}
//--------------------------------------------------------------
TContextScLoginClient* TBaseScLoginClient::Context()
{
  return (TContextScLoginClient*)mScenario->GetContext();
}
//--------------------------------------------------------------
unsigned int TBaseScLoginClient::GetID_SessionClientMaster()
{
  return Context()->GetID_SessionClientMaster();
}
//--------------------------------------------------------------
void TBaseScLoginClient::SetID_SessionClientMaster(unsigned int id)
{
  Context()->SetID_SessionClientMaster(id);
}
//--------------------------------------------------------------
unsigned int TBaseScLoginClient::GetID_SessionClientSlave()
{
  return Context()->GetID_SessionClientSlave();
}
//--------------------------------------------------------------
void TBaseScLoginClient::SetID_SessionClientSlave(unsigned int id)
{
  Context()->SetID_SessionClientSlave(id);
}
//--------------------------------------------------------------
unsigned int TBaseScLoginClient::GetID_SessionMasterSlave()
{
  return Context()->GetID_SessionMasterSlave();
}
//--------------------------------------------------------------
void TBaseScLoginClient::SetID_SessionMasterSlave(unsigned int id)
{
  Context()->SetID_SessionMasterSlave(id);
}
//--------------------------------------------------------------
unsigned int TBaseScLoginClient::GetID_SessionMasterSS()
{
  return Context()->GetID_SessionMasterSS();
}
//--------------------------------------------------------------
void TBaseScLoginClient::SetID_SessionMasterSS(unsigned int id)
{
  Context()->SetID_SessionMasterSS(id);
}
//--------------------------------------------------------------
void TBaseScLoginClient::SetupContext(THeader* pPacket, unsigned int id_session)
{
  // для этих типов не назначать контекст
  //eResultLoginM2C:
  //eCheckLeaveQueueM2C:
  //eInfoSlaveM2C:
  //eCheckConnectToSlaveS2C:
  //eConnectToSlaveC2S: контекст назначается в SetIsExistClientID

  switch(pPacket->subType)
  {
    case eRequestM2SS:
      NeedContextIDclientIDmaster(pPacket->id_client,id_session);
      break;
    case eTryLoginC2M:
      mScenario->NeedContext(id_session);
      break;
    case eClientConnectS2M:
    case eLeaveQueueC2M:
    case eCheckRequestSS2M:
    case eInfoClientM2S:
    case eCheckInfoClientS2M:
    case eCheckClientConnectM2S:
      NeedContextByKeyClient(pPacket->id_client);
      break;
    default:BL_FIX_BUG();
  }
}
//---------------------------------------------------------------------
void TBaseScLoginClient::Recv(TDescRecvSession* pDesc)
{
  THeader* pPacket = (THeader*)pDesc->data;
  SetupContext(pPacket, pDesc->id_session);
  RecvInherit(pDesc);
}
//---------------------------------------------------------------------
void TBaseScLoginClient::NeedContextByKeyClient(unsigned int id_client)
{
  mCallBackNeedContextForKeyClient.Notify(id_client);
}
//---------------------------------------------------------------------
void TBaseScLoginClient::NeedIsExistClientID( unsigned int id_client)
{
  mCallBackNeedIsExistClientID.Notify(id_client);
}
//--------------------------------------------------------------
bool TBaseScLoginClient::Begin()
{
  return mScenario->Begin();
}
//---------------------------------------------------------------------
void TBaseScLoginClient::End()
{
  return mScenario->End();
}
//---------------------------------------------------------------------
void TBaseScLoginClient::NeedContext(unsigned int id_session)
{
  return mScenario->NeedContext(id_session);
}
//---------------------------------------------------------------------
void TBaseScLoginClient::SetTimeWaitForNow()
{
  Context()->SetTimeWait(ht_GetMSCount());
}
//--------------------------------------------------------------
void TBaseScLoginClient::NeedContextIDclientIDmaster(unsigned int id_client,
                                                     unsigned int id_session)
{
  mCallBackNeedContextIDclientIDmaster.Notify(id_client,id_session);
}
//--------------------------------------------------------------
