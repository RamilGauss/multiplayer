/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "Base.h"
#include <boost/foreach.hpp>

#include "Logger.h"
#include "ManagerSession.h"
#include "DescRecvSession.h"
#include "ManagerContextSc.h"
#include "ControlScenario.h"
#include "ContainerContextSc.h"

#include "ScenarioDisconnectClient.h"
#include "ScenarioDisconnectSlave.h"
#include "ScenarioFlow.h"
#include "ScenarioLoginClient.h"
#include "ScenarioLoginSlave.h"
#include "ScenarioLoginMaster.h"
#include "ScenarioRecommutationClient.h"
#include "ScenarioSendToClient.h"
#include "ScenarioSynchroSlave.h"

using namespace std;
using namespace nsMelissa;

//-------------------------------------------------------------------------
TBase::TBase():
mManagerSession(new TManagerSession),
mControlSc(new TControlScenario),
mContainerUp(new TContainerContextSc)
{
  mLoadProcent     = 0;

  flgConnectUp  = false;
  mID_SessionUp = INVALID_HANDLE_SESSION;

  SetupScForContext(mContainerUp.get());
  // задать контекст по-умолчанию
  mControlSc->mDisClient   ->SetContext(&mContainerUp->mDisClient);
  mControlSc->mDisSlave    ->SetContext(&mContainerUp->mDisSlave);
  mControlSc->mFlow        ->SetContext(&mContainerUp->mFlow);        
  mControlSc->mLoginClient ->SetContext(&mContainerUp->mLoginClient); // client
  mControlSc->mLoginSlave  ->SetContext(&mContainerUp->mLoginSlave);  // slave
  mControlSc->mLoginMaster ->SetContext(&mContainerUp->mLoginMaster); // master
  mControlSc->mRcm         ->SetContext(&mContainerUp->mRcm);         // client
  mControlSc->mSynchroSlave->SetContext(&mContainerUp->mSynchroSlave);// slave

  mControlSc->mDisClient   ->RegisterOnNeedContext(&TBase::NeedContextDisconnectClient, this);
  mControlSc->mDisSlave    ->RegisterOnNeedContext(&TBase::NeedContextDisconnectSlave, this);
  mControlSc->mLoginClient ->RegisterOnNeedContext(&TBase::NeedContextLoginClient, this);
  mControlSc->mLoginSlave  ->RegisterOnNeedContext(&TBase::NeedContextLoginSlave,  this);
  mControlSc->mLoginMaster ->RegisterOnNeedContext(&TBase::NeedContextLoginMaster, this);
  mControlSc->mRcm         ->RegisterOnNeedContext(&TBase::NeedContextRcm,         this);
  mControlSc->mSendToClient->RegisterOnNeedContext(&TBase::NeedContextSendToClient,this);
  mControlSc->mSynchroSlave->RegisterOnNeedContext(&TBase::NeedContextSynchroSlave,this);

  mControlSc->mDisClient   ->RegisterOnEnd(&TBase::EndDisconnectClient,this);
  mControlSc->mDisSlave    ->RegisterOnEnd(&TBase::EndDisconnectSlave, this);
  mControlSc->mLoginClient ->RegisterOnEnd(&TBase::EndLoginClient, this);
  mControlSc->mLoginSlave  ->RegisterOnEnd(&TBase::EndLoginSlave,  this);
  mControlSc->mLoginMaster ->RegisterOnEnd(&TBase::EndLoginMaster, this);
  mControlSc->mRcm         ->RegisterOnEnd(&TBase::EndRcm,         this);
  mControlSc->mSynchroSlave->RegisterOnEnd(&TBase::EndSynchroSlave,this);
}
//-------------------------------------------------------------------------
TBase::~TBase()
{
  mManagerSession->GetCallbackRecv()->Unregister(this);
  mManagerSession->GetCallbackDisconnect()->Unregister(this);

  BOOST_FOREACH( TManagerContextSc* pMSc, mSetManagerContextSc )
    delete pMSc;
  mSetManagerContextSc.clear();
}
//-------------------------------------------------------------------------
void TBase::Init(IMakerTransport* pMakerTransport)
{
  if(pMakerTransport==NULL)
  {
    GetLogger()->Get(STR_NAME_MELISSA)->WriteF_time("TBase::Init() pMakerTransport==NULL.\n");
    BL_FIX_BUG();
    return;
  }
  mManagerSession->SetMakerTransport(pMakerTransport);
  mManagerSession->GetCallbackRecv()->Register(&TBase::Recv, this);
  mManagerSession->GetCallbackDisconnect()->Register(&TBase::Disconnect, this);
}
//-------------------------------------------------------------------------
bool TBase::Open(unsigned short port, unsigned char subNet)
{
  return mManagerSession->Start(port, subNet);
}
//-------------------------------------------------------------------------
void TBase::SetLoad(int procent)
{
  mLoadProcent = procent;
}
//-------------------------------------------------------------------------
void TBase::DisconnectUp()
{

}
//-------------------------------------------------------------------------
void TBase::SendUp(TBreakPacket bp, bool check)
{
  // устанавливать для сценария контекст не требуется
  mControlSc->mFlow->SendUp(bp, check);
}
//-------------------------------------------------------------------------
bool TBase::IsConnectUp()
{
	return mManagerSession->IsExist(mID_SessionUp);
}
//-------------------------------------------------------------------------
bool TBase::IsConnect(unsigned int id)
{
  return mManagerSession->IsExist(id);
}
//-------------------------------------------------------------------------
void TBase::Recv( TDescRecvSession* pDesc )
{
  TDescRecvSession* pNewDesc = new TDescRecvSession;
  pNewDesc->Assign(pDesc);
  mRecvPacket.Add(pNewDesc);
}
//-------------------------------------------------------------------------
void TBase::Disconnect(unsigned int id)
{
  unsigned int* pID = new unsigned int(id);
  mIDSessionDisconnect.Add(pID);
}
//-------------------------------------------------------------------------
void TBase::Work()
{
  //пробежка по ожидающим разъединения и удаление сессий
  mManagerSession->Work();
  // отреагировать на событие дисконнект сессий
  HandleListDisconnect();
  // обработать полученные данные
  HandleListRecv();
  // дать отработать всем сценариям по своим задачам 
  // порядок вызовов здесь не случаен, сначала должен быть вызов HandleListRecv
  BOOST_FOREACH( TManagerContextSc* pMSc, mSetManagerContextSc )
    pMSc->Work();
	// например, Slave должен отсылать отчет по своей нагрузке CPU на Master
	WorkInherit();
}
//-------------------------------------------------------------------------
void TBase::SetTimeLiveSession(unsigned int time_ms)
{
  mManagerSession->SetTimeLiveSession(time_ms);
}
//-------------------------------------------------------------------------
void TBase::HandleListDisconnect()
{
  unsigned int** ppFirst = mIDSessionDisconnect.GetFirst();
  while(ppFirst)
  {
    unsigned int ID = *(*ppFirst);
    // сообщить о разрыве связи для различных реализаций
    DisconnectInherit(ID);
    // закрыть сессию
    mManagerSession->CloseSession(ID);
    // следующий ID
    mIDSessionDisconnect.Remove(ppFirst);
    ppFirst = mIDSessionDisconnect.GetFirst();
  }
}
//-------------------------------------------------------------------------
void TBase::HandleListRecv()
{
  TDescRecvSession** ppFirst = mRecvPacket.GetFirst();
  while(ppFirst)
  {
    TDescRecvSession* pDesc = *ppFirst;
    // обработать через сценарий
    mControlSc->Work(pDesc);
    mRecvPacket.Remove(ppFirst);
    ppFirst = mRecvPacket.GetFirst();
  }
}
//-------------------------------------------------------------------------
TManagerContextSc* TBase::AddManagerContextSc()
{
  TManagerContextSc* pMСSc = new TManagerContextSc();
  mSetManagerContextSc.insert(pMСSc);
  return pMСSc;
}
//-------------------------------------------------------------------------
void TBase::RemoveManagerContextSc(TManagerContextSc* pMСSc)
{
  mSetManagerContextSc.erase(pMСSc);
  delete pMСSc;
}
//-------------------------------------------------------------------------
void TBase::SetupScForContext(TContainerContextSc* pCCSc)
{
  pCCSc->SetMCSc(AddManagerContextSc());
  pCCSc->SetMS(mManagerSession.get());
  pCCSc->SetSE(this);

  pCCSc->mDisClient.   SetSc(mControlSc->mDisClient);
  pCCSc->mDisSlave.    SetSc(mControlSc->mDisSlave);
  pCCSc->mFlow.        SetSc(mControlSc->mFlow);
  pCCSc->mLoginClient. SetSc(mControlSc->mLoginClient);
  pCCSc->mLoginSlave.  SetSc(mControlSc->mLoginSlave);
  pCCSc->mLoginMaster. SetSc(mControlSc->mLoginMaster);
  pCCSc->mRcm.         SetSc(mControlSc->mRcm);
  pCCSc->mSendToClient.SetSc(mControlSc->mSendToClient);
  pCCSc->mSynchroSlave.SetSc(mControlSc->mSynchroSlave);
}
//-------------------------------------------------------------------------
