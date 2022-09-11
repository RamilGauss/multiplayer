/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
√удаков –амиль —ергеевич 
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
  mNavigateSession = new TNavigateSession;
  mMngTransport = new TManagerTransport(this);

	flgStart = false;
  mTimeLiveSession  = eDefTimeLive;
  mLastID_Session   = 0;
}
//--------------------------------------------------------------------------------------------
TManagerSession::~TManagerSession()
{
  // сначала уничтожить сессии, важно вызвать до уничтожени€ ћенеджера транспорта
  lockAccessSession();
  delete mNavigateSession;
  mNavigateSession = NULL;// не готов к приему пакетов
  unlockAccessSession();

  delete mMngTransport;
}
//--------------------------------------------------------------------------------------------
void TManagerSession::SetMakerTransport(IMakerTransport* pMakerTransport)
{
	mMngTransport->SetTransport(pMakerTransport);
}
//--------------------------------------------------------------------------------------------
bool TManagerSession::Start(TDescOpen* pDesc, int count)
{
	if(flgStart)
	{
		GetLogger(STR_NAME_MMO_ENGINE)->WriteF_time("TManagerSession::Start() restart.\n");
		BL_FIX_BUG();
		return false;
	}
	flgStart = true;

	for(int i = 0 ; i < count ; i++ )
		if(StartTransport(pDesc[i].port, pDesc[i].subNet)==false)
			return false;
	return true;
}
//--------------------------------------------------------------------------------------------
bool TManagerSession::StartTransport(unsigned short port, unsigned char subNet)
{
	INetTransport* pTransport = mMngTransport->Add(subNet);
  bool resOpen = pTransport->Open(port,subNet);
  if(resOpen==false) 
  {
    GetLogger(STR_NAME_MMO_ENGINE)->WriteF_time("TManagerSession::Start() open port %u FAIL.\n", port);
    BL_FIX_BUG();
    return false;
  }
  // старт потока чтени€
  pTransport->Start();
  return resOpen;
}
//--------------------------------------------------------------------------------------------
void TManagerSession::Work()
{
  lockAccessSession();
  if(mNavigateSession==NULL)
  {
    unlockAccessSession();
    return;
  }
  //===================================================================
  mNavigateSession->Work();
  unlockAccessSession();
}
//--------------------------------------------------------------------------------------------
void TManagerSession::Send(unsigned int id_session, TBreakPacket bp, bool check)
{
  lockAccessSession();
  if(mNavigateSession==NULL)
  {
    unlockAccessSession();
    return;
  }
  //===================================================================
  TSession* pSession = mNavigateSession->FindSessionByID(id_session);
  if(pSession) 
    pSession->Send(bp,check);
  
  unlockAccessSession();
}
//--------------------------------------------------------------------------------------------
unsigned int TManagerSession::Send(unsigned int ip, unsigned short port, TBreakPacket bp, unsigned char subNet, bool check)
{
	INetTransport* pTransport = mMngTransport->FindBySubNet(subNet);
	if(pTransport==NULL)
		return INVALID_HANDLE_SESSION;

  lockAccessSession();
  if(mNavigateSession==NULL)
  {
    unlockAccessSession();
    return INVALID_HANDLE_SESSION;
  }
  //===================================================================
  // соединитьс€ с сервером
  if(pTransport->Connect(ip, port)==false) 
  {
    unlockAccessSession();
    return INVALID_HANDLE_SESSION;// нет такого прослушивающего порта
  }
  TIP_Port ip_port(ip,port);
 
  TSession* pSession = mNavigateSession->FindSessionByIP(ip_port);
  if(pSession==NULL)
    pSession = NewSession(ip_port, pTransport);
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
  if(mNavigateSession==NULL)
  {
    unlockAccessSession();
    return id;
  }
  //===================================================================
  TSession* pSession = mNavigateSession->FindSessionByIP(TIP_Port(ip,port));
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
  if(mNavigateSession==NULL)
  {
    unlockAccessSession();
    return;
  }
  //===================================================================
  TSession* pSession = mNavigateSession->FindSessionByID(ID_Session);
  if(pSession)
    mNavigateSession->Delete(pSession);
  unlockAccessSession();
}
//--------------------------------------------------------------------------------------------
void TManagerSession::Recv( INetTransport::TDescRecv* pDescRecv, INetTransport* pTransport)
{
  lockAccessSession();
  if(mNavigateSession==NULL)
  {
    unlockAccessSession();
    return;
  }
  //===================================================================
  // определить нова€ сесси€ или нет
  TSession* pSession = mNavigateSession->FindSessionByIP(pDescRecv->ip_port);
  if(pSession==NULL)
    pSession = NewSession(pDescRecv->ip_port, pTransport);
  else
    pSession->Recv();// уведомить сессию о приеме
  unsigned int id = pSession->GetID();
  unlockAccessSession();
  //-----------------------------------------------
  TDescRecvSession descRecvSession;
  *((INetTransport::TDescRecv*)&descRecvSession) = *pDescRecv;
  descRecvSession.id_session = id;
  // данные, пришедшие от сессии содержат заголовок, учесть при формировании
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
  if(mNavigateSession==NULL)
  {
    unlockAccessSession();
    return;
  }
  //===================================================================
  TSession* pSession = mNavigateSession->FindSessionByIP(*ip_port);
  if(pSession)
  {
    unsigned int id = pSession->GetID();
    mCallBackDiconnect.Notify(id);
    mNavigateSession->Delete(pSession);
  }
  unlockAccessSession();
}
//--------------------------------------------------------------------------------------------
TSession* TManagerSession::NewSession(TIP_Port& ip_port, INetTransport* pTransport)
{
  mLastID_Session++;// нет проверки на совпадение, unsigned int 4 млрд - слишком много
  TSession* pSession = new TSession(mTimeLiveSession);
  pSession->SetTransport(pTransport);
  pSession->SetInfo(ip_port);
  pSession->SetID(mLastID_Session);

  mNavigateSession->Add(pSession);
  return pSession;
}
//--------------------------------------------------------------------------------------------
bool TManagerSession::IsExist(unsigned int ID_Session)
{
  lockAccessSession();
  if(mNavigateSession==NULL)
  {
    unlockAccessSession();
    return false;
  }
  //===================================================================
  TSession* pSession = mNavigateSession->FindSessionByID(ID_Session);
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
  if(mNavigateSession==NULL)
  {
    unlockAccessSession();
    return false;
  }
  //===================================================================
  TSession* pSession = mNavigateSession->FindSessionByID(ID_Session);
  if(pSession)
  {
    res = true;
    pSession->GetInfo(ip_port_out);
  }
  unlockAccessSession();
  return res;
}
//-------------------------------------------------------------------------
