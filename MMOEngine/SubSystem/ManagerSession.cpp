/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ManagerSession.h"
#include "Logger.h"
#include "Base.h"
#include <boost/foreach.hpp>

using namespace std;
using namespace nsMMOEngine;

TManagerSession::TManagerSession()
{
  mTimeLiveSession  = eDefTimeLive;
  mLastID_Session   = 0;
  mMakerTransport   = NULL;
  mTransport        = NULL;
}
//--------------------------------------------------------------------------------------------
TManagerSession::~TManagerSession()
{
  Stop();
  
  // ������� ������
  mNavigateSession.Clear();

  mMakerTransport->Delete(mTransport);
  mTransport = NULL;
  mMakerTransport = NULL;
}
//--------------------------------------------------------------------------------------------
void TManagerSession::SetMakerTransport(IMakerTransport* pMakerTransport)
{
  mMakerTransport = pMakerTransport;
  mTransport      = mMakerTransport->New();
  
  mTransport->GetCallbackRecv()->Register(&TManagerSession::Recv,this);
  mTransport->GetCallbackDisconnect()->Register(&TManagerSession::Disconnect,this);
}
//--------------------------------------------------------------------------------------------
bool TManagerSession::Start(unsigned short port, unsigned char subNet)
{
  if(mTransport==NULL)
  {
    GetLogger(STR_NAME_MMO_ENGINE)->WriteF_time("TManagerSession::Start() mTransport==NULL.\n");
    BL_FIX_BUG();
    return false;
  }
  if(mTransport->IsActive())
  {
    GetLogger(STR_NAME_MMO_ENGINE)->WriteF_time("TManagerSession::Start() recall.\n");
    BL_FIX_BUG();
    return false;
  }
  //-------------------------------------------------------------
  bool resOpen = mTransport->Open(port,subNet);
  if(resOpen==false) 
  {
    GetLogger(STR_NAME_MMO_ENGINE)->WriteF_time("TManagerSession::Start() open port %u FAIL.\n", port);
    BL_FIX_BUG();
    return false;
  }
  // ����� ������ ������
  mTransport->Start();
  return resOpen;
}
//--------------------------------------------------------------------------------------------
void TManagerSession::Stop()
{
  if(mTransport==NULL) return;

  mTransport->Stop();
  mTransport->GetCallbackRecv()->Unregister(this);
  mTransport->GetCallbackDisconnect()->Unregister(this);
}
//--------------------------------------------------------------------------------------------
void TManagerSession::Work()
{
  lockAccessSession();
  mNavigateSession.Work();
  unlockAccessSession();
}
//--------------------------------------------------------------------------------------------
void TManagerSession::Send(unsigned int ID_Session, TBreakPacket bp, bool check)
{
  lockAccessSession();

  TSession* pSession = mNavigateSession.FindSessionByID(ID_Session);
  if(pSession) 
    pSession->Send(bp,check);
  
  unlockAccessSession();
}
//--------------------------------------------------------------------------------------------
unsigned int TManagerSession::Send(unsigned int ip, unsigned short port, TBreakPacket bp, bool check)
{
  lockAccessSession();
  // ����������� � ��������
  if(mTransport->Connect(ip, port)==false) 
  {
    unlockAccessSession();
    return INVALID_HANDLE_SESSION;// ��� ������ ��������������� �����
  }
  TIP_Port ip_port(ip,port);
 
  TSession* pSession = mNavigateSession.FindSessionByIP(ip_port);
  if(pSession==NULL)
    pSession = NewSession(ip_port);
  else
  {
    GetLogger(STR_NAME_MMO_ENGINE)->
      WriteF_time("TManagerSession::Send(0x%X,%u) sending to IP with exist session.\n", ip, port);
    BL_FIX_BUG();
  }
  unsigned int id_session = pSession->GetID();
  pSession->Send(bp,check);

  unlockAccessSession();
  return id_session;
}
//--------------------------------------------------------------------------------------------
unsigned int TManagerSession::GetSessionID(unsigned int ip, unsigned short port)
{
  unsigned int id = INVALID_HANDLE_SESSION;
  lockAccessSession();
  TSession* pSession = mNavigateSession.FindSessionByIP(TIP_Port(ip,port));
  if(pSession)
    id = pSession->GetID();
  unlockAccessSession();
  return id;
}
//--------------------------------------------------------------------------------------------
void TManagerSession::CloseSession(unsigned int ID_Session)
{
  if(ID_Session==INVALID_HANDLE_SESSION) 
    return;

  lockAccessSession();
  TSession* pSession = mNavigateSession.FindSessionByID(ID_Session);
  if(pSession)
    mNavigateSession.Delete(pSession);
  unlockAccessSession();
}
//--------------------------------------------------------------------------------------------
void TManagerSession::Recv( INetTransport::TDescRecv* pDescRecv )
{
  lockAccessSession();
  // ���������� ����� ������ ��� ���
  TSession* pSession = mNavigateSession.FindSessionByIP(pDescRecv->ip_port);
  if(pSession==NULL)
    pSession = NewSession(pDescRecv->ip_port);
  else
    pSession->Recv();// ��������� ������ � ������
  unsigned int id = pSession->GetID();
  unlockAccessSession();
  //-----------------------------------------------
  TDescRecvSession descRecvSession;
  *((INetTransport::TDescRecv*)&descRecvSession) = *pDescRecv;
  descRecvSession.id_session = id;
  // ������, ��������� �� ������ �������� ���������, ������ ��� ������������
  TSession::THeader* pHeader = (TSession::THeader*)descRecvSession.data;
  switch(pHeader->type)
  {
    case TSession::eEcho:
      break;
    case TSession::ePacket:
      descRecvSession.data     += sizeof(TSession::THeader);
      descRecvSession.sizeData -= sizeof(TSession::THeader);
      mCallBackRecv.Notify(&descRecvSession);
      break;
  }
}
//--------------------------------------------------------------------------------------------
void TManagerSession::Disconnect(TIP_Port* ip_port)
{
  lockAccessSession();
  TSession* pSession = mNavigateSession.FindSessionByIP(*ip_port);
  if(pSession)
  {
    unsigned int id = pSession->GetID();
    mCallBackDiconnect.Notify(id);
    mNavigateSession.Delete(pSession);
  }
  unlockAccessSession();
}
//--------------------------------------------------------------------------------------------
TSession* TManagerSession::NewSession(TIP_Port& ip_port)
{
  mLastID_Session++;// ��� �������� �� ����������, unsigned int 4 ���� - ������� �����
  TSession* pSession = new TSession(mTimeLiveSession);
  pSession->SetTransport(mTransport);
  pSession->SetInfo(ip_port);
  pSession->SetID(mLastID_Session);

  mNavigateSession.Add(pSession);
  return pSession;
}
//--------------------------------------------------------------------------------------------
bool TManagerSession::IsExist(unsigned int ID_Session)
{
  lockAccessSession();
  TSession* pSession = mNavigateSession.FindSessionByID(ID_Session);
  unlockAccessSession();
  return (pSession!=NULL);
}
//--------------------------------------------------------------------------------------------
void TManagerSession::SetTimeLiveSession(unsigned int time_ms)
{
  mTimeLiveSession = time_ms;
}
//-------------------------------------------------------------------------
bool TManagerSession::GetInfo(unsigned int ID_Session, TIP_Port& ip_port_out)
{
  bool res = false;
  lockAccessSession();
  TSession* pSession = mNavigateSession.FindSessionByID(ID_Session);
  if(pSession)
  {
    res = true;
    pSession->GetInfo(ip_port_out);
  }
  unlockAccessSession();
  return res;
}
//-------------------------------------------------------------------------
