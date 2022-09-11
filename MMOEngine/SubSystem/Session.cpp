/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "Session.h"
#include "HiTimer.h"

using namespace nsMMOEngine;


TSession::TSession(unsigned int time_live_ms )
{
  mTimeLive = time_live_ms;

  mLastTimeActive = ht_GetMSCount();
  mTransport = NULL;
}
//---------------------------------------------------------------------
TSession::~TSession()
{
  Close();
}
//---------------------------------------------------------------------
void TSession::Work()
{
  unsigned int now_ms = ht_GetMSCount();
  if(mLastTimeActive + mTimeLive < now_ms)
  {
    SendEcho();
    RefreshLastTime();
  }
}
//---------------------------------------------------------------------
void TSession::Send(TBreakPacket bp, bool check)
{
  SendData(ePacket, bp, check);
  // гарантия того что пакет дойдет
  // иначе сессия на той стороне не освежит время
  if(check)
    RefreshLastTime();
}
//---------------------------------------------------------------------
void TSession::SetTransport(INetTransport* pTransport)
{
  mTransport = pTransport;
}
//---------------------------------------------------------------------
void TSession::GetInfo(TIP_Port& pDesc)
{
  pDesc = mIP_Port;
}
//---------------------------------------------------------------------
void TSession::SetInfo(TIP_Port& pDesc)
{
  mIP_Port = pDesc;
}
//---------------------------------------------------------------------
void TSession::Recv()
{
  RefreshLastTime();
}
//---------------------------------------------------------------------
void TSession::SendEcho()
{
  TBreakPacket bp;
  SendData(eEcho, bp);
}
//---------------------------------------------------------------------
void TSession::RefreshLastTime()
{
  mLastTimeActive = ht_GetMSCount();
}
//---------------------------------------------------------------------
void TSession::SendData(char type, TBreakPacket& bp, bool check)
{
  THeader h(type);
  bp.PushFront((char*)&h, sizeof(THeader));
  mTransport->Send(mIP_Port.ip, mIP_Port.port, bp, check);
}
//---------------------------------------------------------------------
void TSession::Close()
{
  mTransport->Close(mIP_Port.ip, mIP_Port.port);
}
//---------------------------------------------------------------------

