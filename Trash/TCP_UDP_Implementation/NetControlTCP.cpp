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

#include "NetControlTCP.h"
#include "INetMakerEvent.h"
#include "BL_Debug.h"

#include "Logger.h"
#include "HistoryPacketTCP.h"
#include "HeaderTCP.h"

#define PRINTF(X,F) GetLogger()->Get(STR_NAME_NET_TRANSPORT)->WriteF_time(X,F)
#define PRINTF_0(X) PRINTF(X,0)

#define GET_ERROR WSAGetLastError()

using namespace std;
using namespace nsNetTypeEvent;

TNetControlTCP::TNetControlTCP()
{
  mSocketDown = INVALID_SOCKET;
  mSocketUp   = INVALID_SOCKET;
}
//------------------------------------------------------------------------------
TNetControlTCP::~TNetControlTCP()
{
  Done();
}
//------------------------------------------------------------------------------
void TNetControlTCP::Work(INetMakerEventThread* pThreadContext, int sock, list<eTypeEvent>& event)
{
	list<eTypeEvent>::iterator bit = event.begin();
	list<eTypeEvent>::iterator eit = event.end();
	while(bit!=eit)
	{
		switch(*bit)
		{
			case eRead:
				ReadEvent(pThreadContext, sock);
				//PRINTF_0("C_TCP read\n");
				break;
			case eWrite:
				WriteEvent(pThreadContext, sock);
				//PRINTF_0("C_TCP write\n");
				break;
			case eConnect:
				ConnectEvent(pThreadContext, sock);
				PRINTF_0("C_TCP connect\n");
				break;
			case eAccept:
				AcceptEvent(pThreadContext, sock);
				PRINTF_0("C_TCP accept\n");
				break;
			case eClose:
				CloseEvent(pThreadContext, sock);
				PRINTF_0("C_TCP close\n");
				break;
		}
		bit++;
	}
}
//------------------------------------------------------------------------------
bool TNetControlTCP::Open( unsigned short port, unsigned char numNetWork )
{
  mLocalPort  = port; 
  mNumNetWork = numNetWork;

  mSocketDown = mDevice.Open(true,  mLocalPort, mNumNetWork);// как сервер, готовы к подсоединению

	AddToMakerEvent(mSocketDown, true);
  return (mSocketDown!=INVALID_SOCKET);
}
//------------------------------------------------------------------------------
bool TNetControlTCP::Connect(unsigned int ip, unsigned short port)
{
  // если был дисконнект, то попытаться заново открыть порт
  if(mSocketUp==INVALID_SOCKET)
    mSocketUp = mDevice.Open(false, mLocalPort, mNumNetWork);
  if(mSocketUp==INVALID_SOCKET)
  {
    PRINTF("TNetControlTCP::Connect Open Up FAIL %u.\n", GET_ERROR);
    return false;
  }
  //--------------------------------------------------------------------
  bool res = mDevice.Connect(mSocketUp, ip, port);
  if(res)
  {
    AddToMakerEvent(mSocketUp, true);
    GetMakerEvent()->AddIPSock(TIP_Port(ip, port), mSocketUp);
  }
	else
    PRINTF("TNetControlTCP::Connect FAIL %u.\n", GET_ERROR);
  return res;
}
//------------------------------------------------------------------------------
void TNetControlTCP::Send(unsigned int ip, unsigned short port, TBreakPacket bp)
{
  // найти сокет, ассоциированный с ip:port
  TIP_Port ip_port(ip, port);
  int sock = GetMakerEvent()->GetSocketByIpPort(ip_port);
  if(sock==INVALID_SOCKET)
  {
    GetLogger()->Get(STR_NAME_NET_TRANSPORT)->
      WriteF_time("Send not found socket by ip=%u, port=%u.\n",ip,port);
    return;
  }
  // добавить заголовки в начало  - 2 байт под начало + 4 байта - размер данных
	THeaderTCP header;
	header.size = bp.GetSize();
	bp.PushFront((char*)&header, sizeof(header));
	bp.Collect();

	mDevice.Send(sock, (char*)bp.GetCollectPtr(), bp.GetSize(), 0, 0);
}
//------------------------------------------------------------------------------
void TNetControlTCP::ReadEvent(INetMakerEventThread* pThread, int sock)
{
  unsigned int   ip;
  unsigned short port;

  int sizeBuffer = pThread->GetSizeBuffer();
  char* pBuffer  = pThread->GetBuffer();
  int readSize   = mDevice.Read(sock, pBuffer, sizeBuffer, ip, port);
  
  THistoryPacketTCP* pH = pThread->GetHistoryBuffer(sock);
  int beginPos = 0;
  while( beginPos < readSize )
  {
    THistoryPacketTCP::TResult res;
    pH->Analiz(beginPos, res, readSize, pBuffer);
    if(res.complete)
      Notify(sock, res.buffer, res.size);
  }
}
//----------------------------------------------------------------------------------
void TNetControlTCP::WriteEvent(INetMakerEventThread* pThreadContext, int sock)
{
}
//----------------------------------------------------------------------------------
void TNetControlTCP::ConnectEvent(INetMakerEventThread* pThreadContext, int sock)
{
}
//----------------------------------------------------------------------------------
void TNetControlTCP::AcceptEvent(INetMakerEventThread* pThreadContext, int sock)		
{
  unsigned int   ip;
  unsigned short port;
  int newSocket = mDevice.Accept(mSocketDown, ip, port);
  if(newSocket==INVALID_SOCKET)
	{
		PRINTF("TNetControlTCP::AcceptEvent FAIL.\n",GET_ERROR);
    return;
	}
  // нельзя добавлять с флагом true, т.к. возможно будем ждать свой собственный поток
  // сначала информация добавится в главный поток, тот в свою очередь обработает это событие
	AddToMakerEvent(newSocket,false);
  GetMakerEvent()->AddIPSock(TIP_Port(ip,port), newSocket);
}
//----------------------------------------------------------------------------------
void TNetControlTCP::CloseEvent(INetMakerEventThread* pThreadContext, int sock)
{
  TIP_Port ip_port;

  bool res = GetMakerEvent()->GetIpPortBySocket(ip_port,sock);
  if(res==false)
  {
    PRINTF("CloseEvent not found socket=%d.\n", sock);
    return;
  }

  GetMakerEvent()->RemoveWithoutDelay(pThreadContext,sock);
  GetMakerEvent()->RemoveSockIP(sock);

  Close(sock);

  if(mSocketUp==sock)
    mSocketUp = INVALID_SOCKET;
  BL_ASSERT(mSocketDown!=sock);
  // теперь уведомить о разрыве соединения
	NotifyDisconnect( &ip_port );
}
//----------------------------------------------------------------------------------
void TNetControlTCP::AddToMakerEvent(int sock, bool without_delay)
{
	bool res = mDevice.SetRecvBuffer(sock, eSystemSizeForRecvBuffer_Socket);
       res = mDevice.SetSendBuffer(sock, eSystemSizeForSendBuffer_Socket);

	// составить список событий
	list<eTypeEvent> lEvent;
	lEvent.push_back(eRead);
	lEvent.push_back(eWrite);
	lEvent.push_back(eAccept);
	lEvent.push_back(eConnect);
	lEvent.push_back(eClose);

	// регистрация на получение событий
  if(without_delay)
  {
    // т.к. Connect блокирующий метод, то надо убедиться что сокет зарегистрировался
    GetMakerEvent()->Sleep();
    GetMakerEvent()->AddWithoutDelay(sock, this, lEvent);
    GetMakerEvent()->WakeUp();
  }
  else
    GetMakerEvent()->Add(sock, this, lEvent);
}
//----------------------------------------------------------------------------------
void TNetControlTCP::Notify(int sock, char* buffer, int size)
{
	TIP_Port ip_port; 
  bool res = GetMakerEvent()->GetIpPortBySocket(ip_port,sock);
  if(res==false)
  {
    PRINTF("Notify not found socket=%d.\n",sock);
    return;
  }

	INetTransport::TDescRecv descRecv;
	descRecv.ip_port      = ip_port;
	descRecv.type					= INetTransport::ePacket;
	descRecv.data         = buffer;
	descRecv.sizeData     = size;
	NotifyRecv(&descRecv);
}
//----------------------------------------------------------------------------------
void TNetControlTCP::Close(int sock)
{
  mDevice.Close(sock);
}
//----------------------------------------------------------------------------------
void TNetControlTCP::Close(unsigned int ip, unsigned short port)
{
  int sock = GetMakerEvent()->GetSocketByIpPort(TIP_Port(ip,port));
  mDevice.Close(sock);
}
//----------------------------------------------------------------------------------
void TNetControlTCP::Done()
{
}
//----------------------------------------------------------------------------------
