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

#include "NetControlTCP.h"
#include "BL_Debug.h"

#include "Logger.h"
#include "HistoryPacketTCP.h"
#include "HeaderTCP.h"
#include <boost/asio/placeholders.hpp>
#include <boost/smart_ptr/shared_array.hpp>
#include "HiTimer.h"
#include <boost/asio/socket_base.hpp>
#include "NetTransport_Boost.h"

using namespace std;

TNetControlTCP::TNetControlTCP(boost::asio::io_service& io_service):
mDevice(io_service)
{
  mReadSize = 0;
}
//------------------------------------------------------------------------------
TNetControlTCP::~TNetControlTCP()
{
  Done();
}
//------------------------------------------------------------------------------
void TNetControlTCP::Done()
{
  mDevice.Close();
}
//----------------------------------------------------------------------------------
bool TNetControlTCP::Open( unsigned short port, unsigned char numNetWork)
{
  return mDevice.Open(port, numNetWork);
}
//----------------------------------------------------------------------------------
bool TNetControlTCP::Connect(unsigned int ip, unsigned short port)
{
  flgWaitConnect = true;
  flgResConnect  = false;
  RequestConnect(TIP_Port(ip, port));
  
  while(flgWaitConnect)
    ht_msleep(eWaitConnect);
  
  return flgResConnect;
}
//----------------------------------------------------------------------------------
void TNetControlTCP::Send(unsigned int ip, unsigned short port, TBreakPacket bp)
{
  // добавить заголовки в начало  - 2 байт под начало + 4 байта - размер данных
  THeaderTCP header;
  header.size = bp.GetSize();
  bp.PushFront((char*)&header, sizeof(header));
  bp.Collect();

  char* data = (char*)bp.GetCollectPtr();
  int size   = bp.GetSize();

	RequestSend(data,size);
}
//----------------------------------------------------------------------------------
void TNetControlTCP::Close()
{
  mDevice.Close();
}
//----------------------------------------------------------------------------------
void TNetControlTCP::RecvEvent(const boost::system::error_code& error,size_t bytes_transferred)
{
  if(error||bytes_transferred<=0)
  {
    GetLogger()->Get(STR_NAME_NET_TRANSPORT)->
      WriteF_time("RecvEvent TCP disconnect error=%s.\n",error.message().data());

    NotifyDisconnect(mDevice.GetIP_Port());
    mDevice.Close();

		GetNetBoost()->RemoveFromMapTCP(GetDevice()->GetIP_Port(), this);
    return;
  }
  int beginPos = 0;
  mReadSize = bytes_transferred;
  while( beginPos < mReadSize )
  {
    THistoryPacketTCP::TResult res;
    mHistory.Analiz(beginPos, res, mReadSize, mBuffer);
    if(res.complete)
    {
      INetTransport::TDescRecv descRecv;
      descRecv.ip_port      = *(mDevice.GetIP_Port());
      descRecv.type					= INetTransport::ePacket;
      descRecv.data         = res.buffer;
      descRecv.sizeData     = res.size;
      NotifyRecv(&descRecv);
    }
  }
  ReadyRecv();
}
//----------------------------------------------------------------------------------
void TNetControlTCP::SendEvent(const boost::system::error_code& error,size_t bytes_transferred)
{
  if(error)
    GetLogger()->Get(STR_NAME_NET_TRANSPORT)->
      WriteF_time("SendEvent TCP error=%s.\n",error.message().data());
}
//----------------------------------------------------------------------------------
void TNetControlTCP::ConnectEvent(const boost::system::error_code& error)
{
  flgResConnect  = (error==0);
  flgWaitConnect = false;

  if(error)
    GetLogger()->Get(STR_NAME_NET_TRANSPORT)->
      WriteF_time("ConnectEvent TCP error=%s.\n",error.message().data());
}
//----------------------------------------------------------------------------------
void TNetControlTCP::ReadyRecv()
{
  try
  {
    mDevice.GetSocket()->async_receive(boost::asio::buffer(&mBuffer[0], eSizeBuffer),
      boost::bind(&TNetControlTCP::RecvEvent, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
  }
  catch(std::exception& e)
  {
    GetLogger()->Get(STR_NAME_NET_TRANSPORT)->
      WriteF_time("ReadyRecv TCP error=%s.\n",e.what());
  }
}
//----------------------------------------------------------------------------------
void TNetControlTCP::RequestSend(char* data, int size)
{
l_repeat:
  boost::asio::socket_base::message_flags flags = 0;
  boost::system::error_code ec;
  int resSend = mDevice.GetSocket()->send(boost::asio::buffer(data, size), flags, ec);
  if(ec)
	{
		GetLogger()->Get(STR_NAME_NET_TRANSPORT)->
			WriteF_time("RequestSend TCP error=%s.\n",ec.message().data());
		return;
	}
	if(resSend<size)
	{
		ht_msleep(eTimeRepeatSend);
		size -= resSend;
		data += resSend;
		goto l_repeat;
	}
}
//----------------------------------------------------------------------------------
void TNetControlTCP::RequestConnect(TIP_Port& ip_port)
{
  try
  {
    //###
    mConnectEndpoint = boost::asio::ip::tcp::endpoint
      (boost::asio::ip::address_v4(ip_port.ip),ip_port.port);
    mDevice.GetSocket()->connect(mConnectEndpoint);
    flgWaitConnect = false;  
    flgResConnect  = true;
    //###

    //mConnectEndpoint = boost::asio::ip::tcp::endpoint
    //  (boost::asio::ip::address_v4(ip_port.ip),ip_port.port);
    //mDevice.GetSocket()->async_connect(mConnectEndpoint,
    //  boost::bind(&TNetControlTCP::ConnectEvent, this,boost::asio::placeholders::error));
  }
  catch(std::exception& e)
  {
    GetLogger()->Get(STR_NAME_NET_TRANSPORT)->
      WriteF_time("RequestConnect TCP error=%s.\n",e.what());

    flgWaitConnect = false;  
    flgResConnect  = false;
  }
}
//----------------------------------------------------------------------------------
void TNetControlTCP::Init()
{
  ReadyRecv();
}
//--------------------------------------------------------------------------------
