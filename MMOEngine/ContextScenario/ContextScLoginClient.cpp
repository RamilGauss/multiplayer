/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ContextScLoginClient.h"
#include "BL_Debug.h"
#include "Base.h"

using namespace nsMMOEngine;
using namespace std;

namespace nsContextScLoginClient
{
  string strDataCommunicationClientMaster = "ClientMaster";
  string strDataCommunicationMasterSlave  = "MasterSlave";
  string strDataCommunicationClientSlave  = "ClientSlave";
  string strDataCommunicationMasterSS     = "MasterSS";
};

using namespace nsContextScLoginClient;

TContextScLoginClient::TContextScLoginClient()
{
  mNumInQueue             = 0;
  mTimeWaitAnswer         = 0;
  mTimeLastNeedNumInQueue = 0;
  mState                  = eUndef;

  mIDClient = (unsigned int)-1;

  flgNeedLeaveQueue = true;
}
//------------------------------------------------------------------
TContextScLoginClient::~TContextScLoginClient()
{

}
//------------------------------------------------------------------
void TContextScLoginClient::SetTimeWait(unsigned int v)
{
  mTimeWaitAnswer = v;
}
//------------------------------------------------------------------
unsigned int TContextScLoginClient::GetTimeWait()
{
  return mTimeWaitAnswer;
}
//------------------------------------------------------------------
void TContextScLoginClient::SetTimeLastNeedNumInQueue(unsigned int v)
{
  mTimeLastNeedNumInQueue = v;
}
//------------------------------------------------------------------
unsigned int TContextScLoginClient::GetTimeLastNeedNumInQueue()
{
  return mTimeLastNeedNumInQueue;
}
//------------------------------------------------------------------
void TContextScLoginClient::Accept()
{
  mState = eAccept;
}
//------------------------------------------------------------------
void TContextScLoginClient::Reject()
{
  mState = eReject;
}
//------------------------------------------------------------------
void TContextScLoginClient::SetID_Session(std::string& name, unsigned int id)
{
	TMapStrUintIt fit = mMapStrID_Session.find(name);
	if(fit==mMapStrID_Session.end())
		mMapStrID_Session.insert(TMapStrUint::value_type(name,id));
	else
		fit->second = id;
}
//--------------------------------------------------------------
unsigned int TContextScLoginClient::GetID_Session(std::string& name)
{
  TMapStrUintIt fit = mMapStrID_Session.find(name);
  if(fit==mMapStrID_Session.end())
    return INVALID_HANDLE_SESSION;
  return fit->second;
}
//--------------------------------------------------------------
unsigned int TContextScLoginClient::GetID_SessionClientMaster()
{
  return GetID_Session(strDataCommunicationClientMaster);
}
//--------------------------------------------------------------
void TContextScLoginClient::SetID_SessionClientMaster(unsigned int id)
{
  SetID_Session(strDataCommunicationClientMaster, id);
}
//--------------------------------------------------------------
unsigned int TContextScLoginClient::GetID_SessionClientSlave()
{
  return GetID_Session(strDataCommunicationClientSlave);
}
//--------------------------------------------------------------
void TContextScLoginClient::SetID_SessionClientSlave(unsigned int id)
{
  SetID_Session(strDataCommunicationClientSlave, id);
}
//--------------------------------------------------------------
unsigned int TContextScLoginClient::GetID_SessionMasterSlave()
{
  return GetID_Session(strDataCommunicationMasterSlave);
}
//--------------------------------------------------------------
void TContextScLoginClient::SetID_SessionMasterSlave(unsigned int id)
{
  SetID_Session(strDataCommunicationMasterSlave, id);
}
//--------------------------------------------------------------
unsigned int TContextScLoginClient::GetID_SessionMasterSS()
{
  return GetID_Session(strDataCommunicationMasterSS);
}
//--------------------------------------------------------------
void TContextScLoginClient::SetID_SessionMasterSS(unsigned int id)
{
  SetID_Session(strDataCommunicationMasterSS, id);
}
//--------------------------------------------------------------
bool TContextScLoginClient::IsReject()
{
  return mState==eReject;
}
//--------------------------------------------------------------
bool TContextScLoginClient::IsAccept()
{
  return mState==eAccept;
}
//--------------------------------------------------------------
void TContextScLoginClient::SetNumInQueue(int val)
{
  mNumInQueue = val;
}
//--------------------------------------------------------------
int TContextScLoginClient::GetNumInQueue()
{
  return mNumInQueue;
}
//--------------------------------------------------------------
void TContextScLoginClient::SaveAcceptData(void* resForClient, int sizeResClient)
{
  mAcceptData.SetData((char*)resForClient, sizeResClient);
}
//--------------------------------------------------------------
void* TContextScLoginClient::GetSaveAcceptDataPtr()
{
  return mAcceptData.GetPtr();
}
//--------------------------------------------------------------
int TContextScLoginClient::GetSaveAcceptDataSize()
{
  return mAcceptData.GetSize();
}
//--------------------------------------------------------------
void TContextScLoginClient::SaveQueueData(void* resForClient, int sizeResClient)
{
	mQueueData.SetData((char*)resForClient, sizeResClient);
}
//--------------------------------------------------------------
void* TContextScLoginClient::GetSaveQueueDataPtr()
{
	return mQueueData.GetPtr();
}
//--------------------------------------------------------------
int TContextScLoginClient::GetSaveQueueDataSize()
{
	return mQueueData.GetSize();
}
//--------------------------------------------------------------
unsigned int TContextScLoginClient::GetIDClient()
{
  return mIDClient;
}
//--------------------------------------------------------------
void TContextScLoginClient::SetIDClient(unsigned int id)
{
  mIDClient = id;
}
//--------------------------------------------------------------
void TContextScLoginClient::SetNeedLeaveQueue(bool val)
{
  flgNeedLeaveQueue = val;
}
//--------------------------------------------------------------
bool TContextScLoginClient::NeedLeaveQueue()
{
  return flgNeedLeaveQueue;
}
//--------------------------------------------------------------
TIP_Port TContextScLoginClient::GetSlaveIP_Port()
{
  return mIP_Port_slave;
}
//--------------------------------------------------------------
void TContextScLoginClient::SetSlaveIP_Port(TIP_Port& ip_port)
{
  mIP_Port_slave = ip_port;
}
//--------------------------------------------------------------
bool TContextScLoginClient::GetFakeClient()
{
  return flgFakeClient;
}
//--------------------------------------------------------------
void TContextScLoginClient::SetFakeClient(bool val)
{
  flgFakeClient = val;
}
//--------------------------------------------------------------


