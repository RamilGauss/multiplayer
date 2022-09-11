/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
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

#include <winsock.h>

#include "NetControlUDP.h"
#include "BL_Debug.h"
#include "common_defs.h"
#include "INetMakerEvent.h"

#include "Logger.h"

#define PRINTF(X,F) GetLogger()->Get(STR_NAME_NET_TRANSPORT)->WriteF_time(X,F)
#define PRINTF_0(X) PRINTF(X,0)

#define GET_ERROR WSAGetLastError()

using namespace std;
using namespace nsNetTypeEvent;

TNetControlUDP::TNetControlUDP()
{
	mSocketLocal = -1;

	mReadSize = 0;
}
//------------------------------------------------------------------------------
TNetControlUDP::~TNetControlUDP()
{
  Done();
}
//------------------------------------------------------------------------------
void TNetControlUDP::Work(INetMakerEventThread* pThreadContext, int sock, list<eTypeEvent>& event)
{
  list<eTypeEvent>::iterator bit = event.begin();
  list<eTypeEvent>::iterator eit = event.end();
  while(bit!=eit)
  {
    switch(*bit)
    {
      case eRead:
				ReadEvent();
        break;
      case eWrite:
				WriteEvent();
        break;
      case eConnect:
				ConnectEvent();
        break;
      case eAccept:
				AcceptEvent();				
        break;
      case eClose:
				CloseEvent();
        break;
    }
    bit++;
  }
}
//------------------------------------------------------------------------------
bool TNetControlUDP::Open( unsigned short port, unsigned char numNetWork )
{
  mSocketLocal = mDevice.Open(false, port, numNetWork);
  if(mSocketLocal==INVALID_SOCKET)
    return false;

	bool res = mDevice.SetRecvBuffer(mSocketLocal, eSystemSizeForRecvBuffer_Socket);
       res = mDevice.SetSendBuffer(mSocketLocal, eSystemSizeForSendBuffer_Socket);

  list<eTypeEvent> lEvent;
  lEvent.push_back(eRead);
  lEvent.push_back(eWrite);
  GetMakerEvent()->AddWithoutDelay(mSocketLocal, this, lEvent);

  return (mSocketLocal!=INVALID_SOCKET);
}
//------------------------------------------------------------------------------
bool TNetControlUDP::Connect(unsigned int ip, unsigned short port)
{
  PRINTF_0("Try connect on UDP.\n");
  return false;// нельзя, только TCP
}
//------------------------------------------------------------------------------
void TNetControlUDP::Send(unsigned int ip, unsigned short port, TBreakPacket bp)
{
	// формируем заголовок
	TInfoConnect infoConnect;
	GetInfoConnect(TIP_Port(ip,port),infoConnect);
	unsigned short count_out = infoConnect.cnt_out++;
	bp.PushFront( (char*)&count_out, sizeof(count_out));
	bp.Collect();
	mDevice.Send(mSocketLocal, (char*)bp.GetCollectPtr(), bp.GetSize(), ip, port);
}
//------------------------------------------------------------------------------
//void TNetControlUDP::Close(unsigned int ip, unsigned short port)
//{
//	mDevice.Close(mSocketLocal);
//	mSocketLocal = -1;
//}
//------------------------------------------------------------------------------
bool TNetControlUDP::IsStreamFresh( TIP_Port& ip_port)
{
	TInfoConnect infoConnect;
	GetInfoConnect(ip_port, infoConnect);

	unsigned short cnt_in = ((unsigned short*)mBuffer)[0];
	if(A_more_B( cnt_in, infoConnect.cnt_in))
	{
		SetCntInByIP_Port(ip_port, cnt_in);
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------------
bool TNetControlUDP::A_more_B(unsigned short A, unsigned short B)
{
	return ::A_more_B_cycle<unsigned short>(A,B);
}
//----------------------------------------------------------------------------------
void TNetControlUDP::ReadEvent()
{
	unsigned int   ip;
	unsigned short port;
	mReadSize = mDevice.Read(mSocketLocal, mBuffer, sizeof(mBuffer), ip, port);
	TIP_Port ip_port(ip, port);
	if(IsStreamFresh(ip_port))
	{
		INetTransport::TDescRecv descRecv;
		descRecv.ip_port      = ip_port;
		descRecv.type					= INetTransport::eStream;
		descRecv.data         = mBuffer   + sizeof(unsigned short);
		descRecv.sizeData     = mReadSize - sizeof(unsigned short);
		NotifyRecv((char*)&descRecv, sizeof(descRecv));
	}
	else
  {
    PRINTF_0("ReadEvent UDP recv not fresh packet.\n");
  }
}
//----------------------------------------------------------------------------------
void TNetControlUDP::WriteEvent()
{
	// пусто
}
//----------------------------------------------------------------------------------
void TNetControlUDP::ConnectEvent()
{
	// пусто
}
//----------------------------------------------------------------------------------
void TNetControlUDP::AcceptEvent()		
{
	// пусто
}
//----------------------------------------------------------------------------------
void TNetControlUDP::CloseEvent()
{
	// пусто
}
//----------------------------------------------------------------------------------
void TNetControlUDP::GetInfoConnect(TIP_Port& ip_port, TInfoConnect& info_out)
{
	lockSendRcv();

	TMapIP_ICIt fit = mMapInfoConnect.find(ip_port);
	if(fit==mMapInfoConnect.end())
	{
		mMapInfoConnect.insert(TMapIP_IC::value_type(ip_port,TInfoConnect()));
		fit = mMapInfoConnect.find(ip_port);
	}
	info_out = fit->second;

	unlockSendRcv();
}
//----------------------------------------------------------------------------------
void TNetControlUDP::SetCntInByIP_Port(TIP_Port& ip_port, unsigned short cnt_in)
{
	lockSendRcv();

	TMapIP_ICIt fit = mMapInfoConnect.find(ip_port);
	if(fit==mMapInfoConnect.end())
	{
		PRINTF_0("SetCntInByIP_Port not found info connect.\n");
		unlockSendRcv();
		return;
	}
	fit->second.cnt_in = cnt_in;

	unlockSendRcv();
}
//----------------------------------------------------------------------------------
void TNetControlUDP::Close(int sock)
{
  // ignore sock, mSocketLocal is true socket
  if(mSocketLocal!=INVALID_SOCKET)
    mDevice.Close(mSocketLocal);
	mSocketLocal = INVALID_SOCKET;
}
//----------------------------------------------------------------------------------
void TNetControlUDP::Done()
{

}
//----------------------------------------------------------------------------------
void TNetControlUDP::Close(unsigned int ip, unsigned short port)
{

}
//----------------------------------------------------------------------------------
