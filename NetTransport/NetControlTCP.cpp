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

#define PRINTF(X,F) GetLogger()->Get(STR_NAME_NET_TRANSPORT)->WriteF_time(X,F)
#define PRINTF_0(X) PRINTF(X,0)

#define GET_ERROR WSAGetLastError()

using namespace std;

TNetControlTCP::TNetControlTCP()
{
  mSocketDown = INVALID_SOCKET;
  mSocketUp   = INVALID_SOCKET;
}
//------------------------------------------------------------------------------
TNetControlTCP::~TNetControlTCP()
{

}
//------------------------------------------------------------------------------
void TNetControlTCP::Work(int sock, list<eTypeEvent>& event)
{
  mWorkSocket = sock;
	list<eTypeEvent>::iterator bit = event.begin();
	list<eTypeEvent>::iterator eit = event.end();
	while(bit!=eit)
	{
		switch(*bit)
		{
			case INetControl::eRead:
				ReadEvent();
				//PRINTF_0("C_TCP read\n");
				break;
			case INetControl::eWrite:
				WriteEvent();
				//PRINTF_0("C_TCP write\n");
				break;
			case INetControl::eConnect:
				ConnectEvent();
				PRINTF_0("C_TCP connect\n");
				break;
			case INetControl::eAccept:
				AcceptEvent();
				PRINTF_0("C_TCP accept\n");
				break;
			case INetControl::eClose:
				CloseEvent();
				PRINTF_0("C_TCP close\n");
				break;
		}
		bit++;
	}
  mWorkSocket = INVALID_SOCKET;
}
//------------------------------------------------------------------------------
bool TNetControlTCP::Open( unsigned short port, unsigned char numNetWork )
{
  mSocketDown = mDevice.Open(true,  port, numNetWork);// как сервер, готовы к подсоединению
  mSocketUp   = mDevice.Open(false, port, numNetWork);// а этот как клиент

	AddToMakerEvent(mSocketDown);
  return (mSocketUp!=INVALID_SOCKET) && (mSocketDown!=INVALID_SOCKET);
}
//------------------------------------------------------------------------------
bool TNetControlTCP::Connect(unsigned int ip, unsigned short port)
{
  bool res = mDevice.Connect(mSocketUp, ip, port);
  if(res)
  {
		// а теперь когда есть связь можно настроить на получение событий
		AddToMakerEvent(mSocketUp);
    // запомнить ip и port, ассоциированные с сокетом
		mMapDIPSock.Add(TIP_Port(ip, port), mSocketUp);
  }
	else
	{
    PRINTF("TNetControlTCP::Connect FAIL %u.\n", GET_ERROR);
	}
  return res;
}
//------------------------------------------------------------------------------
void TNetControlTCP::Send(unsigned int ip, unsigned short port, TBreakPacket& bp)
{
  // найти сокет, ассоциированный с ip:port
  TIP_Port ip_port(ip, port);

  lockSA();
  int sock = GetSocketByIpPort(ip_port);
  unlockSA();

	// добавить заголовки в начало  - 2 байт под начало + 4 байта - размер данных
	TBreakPacket bp_out = bp;
	THeader header;
	header.size = bp_out.GetSize();
	bp_out.PushFront((char*)&header, sizeof(header));
	bp_out.Collect();

	mDevice.Send(sock, (char*)bp_out.GetCollectPtr(), bp_out.GetSize(), 0, 0);
}
//------------------------------------------------------------------------------
void TNetControlTCP::Close()
{
}
//------------------------------------------------------------------------------
void TNetControlTCP::ReadEvent()
{
  unsigned int   ip;
  unsigned short port;
  mReadSize = mDevice.Read(mWorkSocket, mBuffer, 
                           sizeof(mBuffer), ip, port);
  Analiz();
}
//----------------------------------------------------------------------------------
void TNetControlTCP::WriteEvent()
{
}
//----------------------------------------------------------------------------------
void TNetControlTCP::ConnectEvent()
{
}
//----------------------------------------------------------------------------------
void TNetControlTCP::AcceptEvent()		
{
  unsigned int   ip;
  unsigned short port;
  int newSocket = mDevice.Accept(mSocketDown, ip, port);

  TIP_Port ip_port(ip, port);
  lockSA();
		mMapDIPSock.Add( TIP_Port(ip, port), newSocket );
  unlockSA();

	AddToMakerEvent(newSocket);
}
//----------------------------------------------------------------------------------
void TNetControlTCP::CloseEvent()
{
	TIP_Port ip_port;
	GetIP_PortBySocket(ip_port, mWorkSocket);
	NotifyDisconnect( (char*)&ip_port, sizeof(ip_port));
}
//----------------------------------------------------------------------------------
void TNetControlTCP::Analiz()
{
  // поиск по сокету истории
  TDescHistoryRead* pH = GetHistoryBuffer(mWorkSocket);
  int beginPos = 0;
  while(beginPos<mReadSize)
  {
    switch(pH->state)
    {
      case eSearchBegin:
        beginPos += SearchBegin(pH, beginPos);
        break;
      case eSearchSize:
        beginPos += SearchSize(pH, beginPos);
        break;
      case eSearchEnd:
        beginPos += SearchEnd(pH, beginPos);
        break;
    }
  }
}
//----------------------------------------------------------------------------------
TNetControlTCP::TDescHistoryRead* TNetControlTCP::GetHistoryBuffer(int sock)
{
  TMapIntDHIt fit = mMapHistory.find(sock);
  if(fit==mMapHistory.end())
  {
    mMapHistory.insert(TMapIntDH::value_type(sock,TDescHistoryRead()));
    fit = mMapHistory.find(sock);
  }
  return &fit->second;
}
//----------------------------------------------------------------------------------
int TNetControlTCP::GetSocketByIpPort( TIP_Port& ip_port )
{
	int sock;
	bool res = mMapDIPSock.GetValueByKey(ip_port, sock);
  if(res==false)
  {
    PRINTF_0("TNetControlTCP::GetSocketByIpPort FAIL.\n");
  }
	return sock;
}
//----------------------------------------------------------------------------------
int TNetControlTCP::SearchBegin(TDescHistoryRead* pH, int beginPos)
{
  if( mReadSize - beginPos < sizeof(THeader) )
  {
    pH->state = eSearchSize;
    pH->c.SetData(&mBuffer[beginPos], mReadSize - beginPos);
    return mReadSize - beginPos;// вернуть сколько истратили
  }
  // ищем в буфере начало
  THeader header;
  THeader* pHeader = (THeader*)&mBuffer[beginPos];
  if(pHeader->header!=header.header)
  {
    PRINTF_0("TNetControlTCP::SearchBegin FAIL.\n");
  }

  pH->state      = eSearchEnd;
  pH->sizePacket = pHeader->size;

  pH->c.SetData(&mBuffer[beginPos], sizeof(THeader));
  return sizeof(THeader);// вернуть сколько истратили
}
//----------------------------------------------------------------------------------
int TNetControlTCP::SearchSize(TDescHistoryRead* pH, int beginPos)
{
  if(mReadSize - beginPos + pH->c.GetSize() < sizeof(THeader))
  {
    // так и не нашли заголовок полностью
    pH->c.AddData(&mBuffer[beginPos], mReadSize - beginPos);
    return mReadSize - beginPos;// вернуть сколько истратили
  }
  int needCopy = sizeof(THeader) - pH->c.GetSize();
  pH->c.AddData(&mBuffer[beginPos], needCopy);

  THeader header;
  THeader* pHeader = (THeader*)pH->c.GetPtr();
  if(pHeader->header!=header.header)
  {
    PRINTF_0("TNetControlTCP::SearchSize FAIL.\n");
  }

  pH->state      = eSearchEnd;
  pH->sizePacket = pHeader->size;

  return needCopy;// вернуть сколько истратили
}
//----------------------------------------------------------------------------------
int TNetControlTCP::SearchEnd(TDescHistoryRead* pH, int beginPos)
{
  int mustSize = pH->sizePacket + sizeof(THeader);// предполагаемый размер
  // не хватает до полного размера
  if( mustSize > mReadSize - beginPos + pH->c.GetSize()/*остаток внутри истории*/)
  {
    // скопировать внутрь и выйти
    pH->c.AddData(&mBuffer[beginPos], mReadSize - beginPos);
    return mReadSize - beginPos;
  }
  if( mustSize == mReadSize - beginPos + pH->c.GetSize())// полный пакет
  {
    pH->c.AddData(&mBuffer[beginPos], mReadSize - beginPos);
		Notify( mWorkSocket, pH->c.GetPtr(), pH->c.GetSize());
    pH->Clear();
    return mReadSize - beginPos;
  }
  int needSize = mustSize - pH->c.GetSize();// не хватает до полного пакета
  pH->c.AddData(&mBuffer[beginPos], needSize );
  Notify( mWorkSocket, pH->c.GetPtr(), pH->c.GetSize());
  pH->Clear();
  return needSize;
}
//----------------------------------------------------------------------------------
void TNetControlTCP::AddToMakerEvent(int sock)
{
	bool res = mDevice.SetRecvBuffer(sock, eSystemSizeForRecvBuffer_Socket);
  res = mDevice.SetSendBuffer(sock, eSystemSizeForSendBuffer_Socket);

	// составить список событий
	list<INetControl::eTypeEvent> lEvent;
	lEvent.push_back(INetControl::eRead);
	lEvent.push_back(INetControl::eWrite);
	lEvent.push_back(INetControl::eAccept);
	lEvent.push_back(INetControl::eConnect);
	lEvent.push_back(INetControl::eClose);

	// регистрация на получение событий
	GetMakerEvent()->Remove(sock);
	GetMakerEvent()->Add(sock, this);
	GetMakerEvent()->SetTypeEvent(sock, lEvent);
}
//----------------------------------------------------------------------------------
void TNetControlTCP::Notify(int sock, char* buffer, int size)
{
	TIP_Port ip_port; 
	GetIP_PortBySocket(ip_port, sock);

	INetTransport::TDescRecv descRecv;
	descRecv.ip_port      = ip_port;
	descRecv.type					= INetTransport::ePacket;
	descRecv.data         = buffer;
	descRecv.sizeData     = size;
	NotifyRecv((char*)&descRecv, sizeof(descRecv));
}
//----------------------------------------------------------------------------------
void TNetControlTCP::GetIP_PortBySocket( TIP_Port& ip_port, int& sock )
{
	bool res = mMapDIPSock.GetKeyByValue(ip_port, sock);
	if(res==false)
  {
    PRINTF_0("TNetControlTCP::GetIP_PortBySocket FAIL.\n");
  }
}
//----------------------------------------------------------------------------------
