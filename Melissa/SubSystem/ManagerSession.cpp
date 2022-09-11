/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
√удаков –амиль —ергеевич 
2011, 2012, 2013
===========================================================================
                        Common Information
"TornadoEngine" GPL Source Code

This file is part of the "TornadoEngine" GPL Source Code.

"TornadoEngine" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"TornadoEngine" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "TornadoEngine" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "TornadoEngine" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "TornadoEngine" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 

#include "ManagerSession.h"
#include "Logger.h"
#include "Base.h"
#include "DescRecvSession.h"

using namespace std;

namespace nsMelissa
{

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

  mMakerTransport->Delete(mTransport);
  mTransport = NULL;
  mMakerTransport = NULL;
  // удалить сессии
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
    GetLogger()->Get(STR_NAME_MELISSA)->WriteF_time("TManagerSession::Start() mTransport==NULL.\n");
    BL_FIX_BUG();
    return false;
  }
  if(mTransport->IsActive())
  {
    GetLogger()->Get(STR_NAME_MELISSA)->WriteF_time("TManagerSession::Start() ѕовторный вызов.\n");
    BL_FIX_BUG();
    return false;
  }
  //-------------------------------------------------------------
  bool resOpen = mTransport->Open(port,subNet);
  if(resOpen==false) 
  {
    GetLogger()->Get(STR_NAME_MELISSA)->WriteF_time("TManagerSession::Start() Ќе удалось открыть порт %u.\n", port);
    BL_FIX_BUG();
    return false;
  }
  // старт потока чтени€
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
  set<TSession*>::iterator bit = mNavigateSession.Begin();
  set<TSession*>::iterator eit = mNavigateSession.End();
  while(bit!=eit)
  {
    (*bit)->Work();
    bit++;
  }
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
  // соединитьс€ с сервером
  if(mTransport->Connect(ip, port)==false) 
    return INVALID_HANDLE_SESSION;// нет такого прослушивающего порта
  TIP_Port ip_port(ip,port);

  lockAccessSession();
  
  TSession* pSession = mNavigateSession.FindSessionByIP(ip_port);
  if(pSession==NULL)
    pSession = NewSession(ip_port);
  else
  {
    GetLogger()->Get(STR_NAME_MELISSA)->
      WriteF_time("TManagerSession::Send(0x%X,%u) отправка при наличии —ессии.\n", ip, port);
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
  lockAccessSession();
  TSession* pSession = mNavigateSession.FindSessionByID(ID_Session);
  mNavigateSession.Delete(pSession);
  unlockAccessSession();
}
//--------------------------------------------------------------------------------------------
void TManagerSession::Recv( INetTransport::TDescRecv* pDescRecv )
{
  lockAccessSession();
  // определить нова€ сесси€ или нет
  TSession* pSession = mNavigateSession.FindSessionByIP(pDescRecv->ip_port);
  if(pSession==NULL)
    pSession = NewSession(pDescRecv->ip_port);
  else
    pSession->Recv();// уведомить сессию о приеме
  unsigned int id = pSession->GetID();
  unlockAccessSession();
  //-----------------------------------------------
  TDescRecvSession descRecvSession;
  *((INetTransport::TDescRecv*)&descRecvSession) = *pDescRecv;
  descRecvSession.id = id;
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
  TSession* pSession = mNavigateSession.FindSessionByIP(*ip_port);
  unsigned int id = pSession->GetID();
  mCallBackDiconnect.Notify(id);
  mNavigateSession.Delete(pSession);
  unlockAccessSession();
}
//--------------------------------------------------------------------------------------------
TSession* TManagerSession::NewSession(TIP_Port& ip_port)
{
  mLastID_Session++;// нет проверки на совпадение, unsigned int 4 млрд - слишком много
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
}