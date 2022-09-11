/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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

#include "NetDeviceUDP.h"
#include "BL_Debug.h"
#include "HiTimer.h"
#include "Logger.h"
#include "INetTransport.h"
#include "NetSystem.h"
#include <boost\asio\buffer.hpp>

using namespace boost::asio;

TNetDeviceUDP::TNetDeviceUDP(boost::asio::io_service& io_service):
mSocket(io_service)
{

}
//--------------------------------------------------------------------------------
TNetDeviceUDP::~TNetDeviceUDP()
{

}
//--------------------------------------------------------------------------------
bool TNetDeviceUDP::Open( unsigned short port, unsigned char numNetWork )
{
  bool res = false;
#ifdef WIN32
  char* sLocalHost = ns_getSelfIP(numNetWork);
#else
  char sLocalHost[100];
  get_ip_first_eth(sLocalHost);
#endif
  try
  {
    const ip::address_v4 ipv4_address_Local = ip::address_v4::from_string(sLocalHost);
    const ip::address addr_Local(ipv4_address_Local);
    ip::udp::endpoint endpoint_Local(addr_Local,port);
    mSocket.open(endpoint_Local.protocol());
    mSocket.bind(endpoint_Local);

    mIP_Port.Set( ns_inet_addr(sLocalHost), port); 
    res = true;
  }
  catch(std::exception& e)
  {
    GetLogger()->Get(STR_NAME_NET_TRANSPORT)->
      WriteF_time("Open UDP (%d,%d) FAIL: %s.\n", port, numNetWork, e.what());
  }
  return res;
}
//--------------------------------------------------------------------------------
void TNetDeviceUDP::Close()
{
	if(mSocket.is_open()==false) return;
	try
	{
		mSocket.shutdown(boost::asio::socket_base::shutdown_type::shutdown_both);
		mSocket.close();
	}
	catch(std::exception& e)
	{
		GetLogger()->Get(STR_NAME_NET_TRANSPORT)->
			WriteF_time("Close UDP FAIL: %s.\n", e.what());
	}
}
//--------------------------------------------------------------------------------
bool TNetDeviceUDP::SetRecvBuffer(unsigned int size)
{
  boost::system::error_code ec;
  boost::asio::socket_base::receive_buffer_size option(size);
  mSocket.set_option(option,ec);
  return (ec==0);
}
//--------------------------------------------------------------------------------
bool TNetDeviceUDP::SetSendBuffer(unsigned int size)
{
  boost::system::error_code ec;
  boost::asio::socket_base::send_buffer_size option(size);
  mSocket.set_option(option,ec);
  return (ec==0);
}
//--------------------------------------------------------------------------------
