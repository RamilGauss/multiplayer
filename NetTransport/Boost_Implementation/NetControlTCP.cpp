/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
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
  // �������� ��������� � ������  - 2 ���� ��� ��������� + 4 ����� - ������ ������
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
    GetLogger(STR_NAME_NET_TRANSPORT)->
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
      descRecv.type					= INetTransport::eTcp;
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
    GetLogger(STR_NAME_NET_TRANSPORT)->
      WriteF_time("SendEvent TCP error=%s.\n",error.message().data());
}
//----------------------------------------------------------------------------------
void TNetControlTCP::ConnectEvent(const boost::system::error_code& error)
{
  flgResConnect  = (error==0);
  flgWaitConnect = false;

  if(error)
    GetLogger(STR_NAME_NET_TRANSPORT)->
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
    GetLogger(STR_NAME_NET_TRANSPORT)->
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
		GetLogger(STR_NAME_NET_TRANSPORT)->
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
    mConnectEndpoint = boost::asio::ip::tcp::endpoint
      (boost::asio::ip::address_v4(ip_port.ip),ip_port.port);
    mDevice.GetSocket()->connect(mConnectEndpoint);
    flgWaitConnect = false;  
    flgResConnect  = true;
  }
  catch(std::exception& e)
  {
    GetLogger(STR_NAME_NET_TRANSPORT)->
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
