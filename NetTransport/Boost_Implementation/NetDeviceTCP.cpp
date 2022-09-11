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

#include "NetDeviceTCP.h"
#include "BL_Debug.h"
#include "HiTimer.h"

#include "Logger.h"
#include "INetTransport.h"
#include "NetSystem.h"
#include <boost/asio/socket_base.hpp>

using namespace boost::asio;
using namespace std;


TNetDeviceTCP::TNetDeviceTCP(boost::asio::io_service& io_service):
mSocket(io_service)
{

}
//--------------------------------------------------------------------------------
TNetDeviceTCP::~TNetDeviceTCP()
{

}
//--------------------------------------------------------------------------------
bool TNetDeviceTCP::Open( unsigned short port, unsigned char numNetWork )
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
    ip::tcp::endpoint endpoint_Local(addr_Local,port);
    mSocket.open(endpoint_Local.protocol());

    SetReUse();
    OffNagl();
    Set_HardClose();
   
    mSocket.bind(endpoint_Local);
    res = true;
  }
  catch(std::exception& e)
  {
    GetLogger()->Get(STR_NAME_NET_TRANSPORT)->
      WriteF_time("Open TCP (%d,%d) FAIL: %s.\n", port, numNetWork, e.what());
  }
  return res;
}
//--------------------------------------------------------------------------------
void TNetDeviceTCP::Close()
{
	if(mSocket.is_open()==false) return;
	try
	{
		mSocket.close();
	}
	catch(std::exception& e)
	{
		GetLogger()->Get(STR_NAME_NET_TRANSPORT)->
			WriteF_time("Close TCP FAIL: %s.\n", e.what());
	}
}
//--------------------------------------------------------------------------------
void TNetDeviceTCP::OffNagl()
{
  boost::system::error_code ec;
  ip::tcp::no_delay option(true);
  mSocket.set_option(option);
}
//--------------------------------------------------------------------------------
void TNetDeviceTCP::SetReUse()
{
  mSocket.set_option(ip::tcp::socket::reuse_address(true));
}
//--------------------------------------------------------------------------------
void TNetDeviceTCP::Set_HardClose()
{
  boost::asio::socket_base::linger option(true, 0);
  mSocket.set_option(option);
}
//--------------------------------------------------------------------------------
bool TNetDeviceTCP::SetRecvBuffer(unsigned int size)
{
  boost::system::error_code ec;
  boost::asio::socket_base::receive_buffer_size option(size);
  mSocket.set_option(option,ec);
  return (ec==0);
}
//--------------------------------------------------------------------------------
bool TNetDeviceTCP::SetSendBuffer(unsigned int size)
{
  boost::system::error_code ec;
  boost::asio::socket_base::send_buffer_size option(size);
  mSocket.set_option(option,ec);
  return (ec==0);
}
//--------------------------------------------------------------------------------
