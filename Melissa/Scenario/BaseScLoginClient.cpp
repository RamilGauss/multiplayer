/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "BaseScLoginClient.h"
#include "ContextScLoginClient.h"

using namespace nsMelissa;
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
  switch(pPacket->subType)
  {
    case eTryLoginC2M:
  //  // ниже вообще пофиг куда, все равно верхнее соединение не меняет контекст в клиенте    
  //  case eQueueM2C:
  //  case eRejectM2C:
  //  case eInfoSlaveM2C:
  //  case eClientInClusterS2C:
  //  case eLeaveQueueC2M:
  //  case eCheckLeaveQueueM2C:
      mScenario->NeedContext(id_session);
      break;
    case eClientConnectS2M:
  //  case eAcceptM2S:
  //  case eCheckAddContentS2M:
  //  case eLoginContentC2S:
  //  case eClientLoginS2M:
  //  case eClientInClusterM2SS:
  //  case eCheckClientInClusterSS2M:
  //  case eCheckClientLoginM2S:
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
  //switch(pPacket->subType)
  //{
  //  case eTryLoginC2M:
  //    TryLoginC2M(pDesc);
  //    break;
  //  case eRejectM2C:
  //    RejectM2C(pDesc);
  //    break;
  //  case eAcceptM2S:
  //    AcceptM2S(pDesc);
  //    break;
  //  case eQueueM2C:
  //    QueueM2C(pDesc);
  //    break;
  //  case eLeaveQueueC2M:
  //    LeaveQueueC2M(pDesc);
  //    break;
  //  case eCheckLeaveQueueM2C:
  //    CheckLeaveQueueM2C(pDesc);
  //    break;
  //  //------------------------------------------
  //  case eCheckAddContentS2M:
  //    CheckAddContentS2M(pDesc);
  //    break;
  //  case eInfoSlaveM2C:
  //    InfoSlaveM2C(pDesc);
  //    break;
  //  case eLoginContentC2S:
  //    LoginContentC2S(pDesc);
  //    break;
  //  case eClientLoginS2M:
  //    ClientLoginS2M(pDesc);
  //    break;
  //  case eClientInClusterM2SS:
  //    ClientInClusterM2SS(pDesc);
  //    break;
  //  case eCheckClientInClusterSS2M:
  //    CheckClientInClusterSS2M(pDesc);
  //    break;
  //  case eCheckClientLoginM2S:
  //    CheckClientLoginM2S(pDesc);
  //    break;
  //  case eClientInClusterS2C:
  //    ClientInClusterS2C(pDesc);
  //    break;
  //  default:BL_FIX_BUG();
  //}
}
//---------------------------------------------------------------------
void TBaseScLoginClient::NeedContextByKeyClient(unsigned int key)
{
  mCallBackNeedContextForKeyClient.Notify(key);
}
//---------------------------------------------------------------------
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
